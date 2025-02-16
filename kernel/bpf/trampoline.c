// SPDX-License-Identifier: GPL-2.0-only
/* Copyright (c) 2019 Facebook */
#include <linux/hash.h>
#include <linux/bpf.h>
#include <linux/filter.h>
#include <linux/btf.h>

/* dummy _ops. The verifier will operate on target program's ops. */
const struct bpf_verifier_ops bpf_extension_verifier_ops = {
};
const struct bpf_prog_ops bpf_extension_prog_ops = {
};

/* btf_vmlinux has ~22k attachable functions. 1k htab is enough. */
#define TRAMPOLINE_HASH_BITS 10
#define TRAMPOLINE_TABLE_SIZE (1 << TRAMPOLINE_HASH_BITS)

static struct hlist_head trampoline_table[TRAMPOLINE_TABLE_SIZE];

/* serializes access to trampoline_table */
static DEFINE_MUTEX(trampoline_mutex);

void *bpf_jit_alloc_exec_page(void)
{
	void *image;

	image = bpf_jit_alloc_exec(PAGE_SIZE);
	if (!image)
		return NULL;

	set_vm_flush_reset_perms(image);
	/* Keep image as writeable. The alternative is to keep flipping ro/rw
	 * everytime new program is attached or detached.
	 */
	set_memory_x((long)image, 1);
	return image;
}

struct bpf_trampoline *bpf_trampoline_lookup(u64 key)
{
	struct bpf_trampoline *tr;
	struct hlist_head *head;
	void *image;
	int i;

	mutex_lock(&trampoline_mutex);
	head = &trampoline_table[hash_64(key, TRAMPOLINE_HASH_BITS)];
	hlist_for_each_entry(tr, head, hlist) {
		if (tr->key == key) {
			refcount_inc(&tr->refcnt);
			goto out;
		}
	}
	tr = kzalloc(sizeof(*tr), GFP_KERNEL);
	if (!tr)
		goto out;

	/* is_root was checked earlier. No need for bpf_jit_charge_modmem() */
	image = bpf_jit_alloc_exec_page();
	if (!image) {
		kfree(tr);
		tr = NULL;
		goto out;
	}

	tr->key = key;
	INIT_HLIST_NODE(&tr->hlist);
	hlist_add_head(&tr->hlist, head);
	refcount_set(&tr->refcnt, 1);
	mutex_init(&tr->mutex);
	for (i = 0; i < BPF_TRAMP_MAX; i++)
		INIT_HLIST_HEAD(&tr->progs_hlist[i]);
	tr->image = image;
out:
	mutex_unlock(&trampoline_mutex);
	return tr;
}

static struct bpf_tramp_progs *
bpf_trampoline_get_progs(const struct bpf_trampoline *tr, int *total)
{
	const struct bpf_prog_aux *aux;
	struct bpf_tramp_progs *tprogs;
	struct bpf_prog **progs;
	int kind;

	*total = 0;
	tprogs = kcalloc(BPF_TRAMP_MAX, sizeof(*tprogs), GFP_KERNEL);
	if (!tprogs)
		return ERR_PTR(-ENOMEM);

	for (kind = 0; kind < BPF_TRAMP_MAX; kind++) {
		tprogs[kind].nr_progs = tr->progs_cnt[kind];
		*total += tr->progs_cnt[kind];
		progs = tprogs[kind].progs;

		hlist_for_each_entry(aux, &tr->progs_hlist[kind], tramp_hlist)
			*progs++ = aux->prog;
	}
	return tprogs;
}

static int bpf_trampoline_update(struct bpf_trampoline *tr)
{
	void *old_image = tr->image + ((tr->selector + 1) & 1) * PAGE_SIZE/2;
	void *new_image = tr->image + (tr->selector & 1) * PAGE_SIZE/2;
	struct bpf_tramp_progs *tprogs;
	u32 flags = BPF_TRAMP_F_RESTORE_REGS;
	struct bpf_prog_aux *aux;
	int err, total;

	tprogs = bpf_trampoline_get_progs(tr, &total);
	if (IS_ERR(tprogs))
		return PTR_ERR(tprogs);

	if (total == 0) {
		err = bpf_arch_text_poke(tr->func.addr, BPF_MOD_CALL,
					 old_image, NULL);
		tr->selector = 0;
		goto out;
	}

	if (tprogs[BPF_TRAMP_FEXIT].nr_progs ||
	    tprogs[BPF_TRAMP_MODIFY_RETURN].nr_progs)
		flags = BPF_TRAMP_F_CALL_ORIG | BPF_TRAMP_F_SKIP_FRAME;

	err = arch_prepare_bpf_trampoline(new_image, new_image + PAGE_SIZE / 2,
					  &tr->func.model, flags, tprogs,
					  tr->func.addr);
	if (err < 0)
		goto out;

	if (tr->selector)
		/* progs already running at this address */
		err = bpf_arch_text_poke(tr->func.addr, BPF_MOD_CALL,
					 old_image, new_image);
	else
		/* first time registering */
		err = bpf_arch_text_poke(tr->func.addr, BPF_MOD_CALL, NULL,
					 new_image);
	if (err)
		goto out;
	tr->selector++;
out:
	kfree(tprogs);
	return err;
}

static enum bpf_tramp_prog_type bpf_attach_type_to_tramp(struct bpf_prog *prog)
{
	switch (prog->expected_attach_type) {
	case BPF_TRACE_FENTRY:
		return BPF_TRAMP_FENTRY;
	case BPF_MODIFY_RETURN:
		return BPF_TRAMP_MODIFY_RETURN;
	case BPF_TRACE_FEXIT:
		return BPF_TRAMP_FEXIT;
	case BPF_LSM_MAC:
		if (!prog->aux->attach_func_proto->type)
			/* The function returns void, we cannot modify its
			 * return value.
			 */
			return BPF_TRAMP_FEXIT;
		else
			return BPF_TRAMP_MODIFY_RETURN;
	default:
		return BPF_TRAMP_REPLACE;
	}
}

int bpf_trampoline_link_prog(struct bpf_prog *prog)
{
	enum bpf_tramp_prog_type kind;
	struct bpf_trampoline *tr;
	int err = 0;
	int cnt;

	tr = prog->aux->trampoline;
	kind = bpf_attach_type_to_tramp(prog);
	mutex_lock(&tr->mutex);
	if (tr->extension_prog) {
		/* cannot attach fentry/fexit if extension prog is attached.
		 * cannot overwrite extension prog either.
		 */
		err = -EBUSY;
		goto out;
	}
	cnt = tr->progs_cnt[BPF_TRAMP_FENTRY] + tr->progs_cnt[BPF_TRAMP_FEXIT];
	if (kind == BPF_TRAMP_REPLACE) {
		/* Cannot attach extension if fentry/fexit are in use. */
		if (cnt) {
			err = -EBUSY;
			goto out;
		}
		tr->extension_prog = prog;
		err = bpf_arch_text_poke(tr->func.addr, BPF_MOD_JUMP, NULL,
					 prog->bpf_func);
		goto out;
	}
	if (cnt >= BPF_MAX_TRAMP_PROGS) {
		err = -E2BIG;
		goto out;
	}
	if (!hlist_unhashed(&prog->aux->tramp_hlist)) {
		/* prog already linked */
		err = -EBUSY;
		goto out;
	}
	hlist_add_head(&prog->aux->tramp_hlist, &tr->progs_hlist[kind]);
	tr->progs_cnt[kind]++;
	err = bpf_trampoline_update(prog->aux->trampoline);
	if (err) {
		hlist_del(&prog->aux->tramp_hlist);
		tr->progs_cnt[kind]--;
	}
out:
	mutex_unlock(&tr->mutex);
	return err;
}

/* bpf_trampoline_unlink_prog() should never fail. */
int bpf_trampoline_unlink_prog(struct bpf_prog *prog)
{
	enum bpf_tramp_prog_type kind;
	struct bpf_trampoline *tr;
	int err;

	tr = prog->aux->trampoline;
	kind = bpf_attach_type_to_tramp(prog);
	mutex_lock(&tr->mutex);
	if (kind == BPF_TRAMP_REPLACE) {
		WARN_ON_ONCE(!tr->extension_prog);
		err = bpf_arch_text_poke(tr->func.addr, BPF_MOD_JUMP,
					 tr->extension_prog->bpf_func, NULL);
		tr->extension_prog = NULL;
		goto out;
	}
	hlist_del(&prog->aux->tramp_hlist);
	tr->progs_cnt[kind]--;
	err = bpf_trampoline_update(prog->aux->trampoline);
out:
	mutex_unlock(&tr->mutex);
	return err;
}

void bpf_trampoline_put(struct bpf_trampoline *tr)
{
	if (!tr)
		return;
	mutex_lock(&trampoline_mutex);
	if (!refcount_dec_and_test(&tr->refcnt))
		goto out;
	WARN_ON_ONCE(mutex_is_locked(&tr->mutex));
	if (WARN_ON_ONCE(!hlist_empty(&tr->progs_hlist[BPF_TRAMP_FENTRY])))
		goto out;
	if (WARN_ON_ONCE(!hlist_empty(&tr->progs_hlist[BPF_TRAMP_FEXIT])))
		goto out;
	bpf_jit_free_exec(tr->image);
	hlist_del(&tr->hlist);
	kfree(tr);
out:
	mutex_unlock(&trampoline_mutex);
}

/* The logic is similar to BPF_PROG_RUN, but with explicit rcu and preempt that
 * are needed for trampoline. The macro is split into
 * call _bpf_prog_enter
 * call prog->bpf_func
 * call __bpf_prog_exit
 */
u64 notrace __bpf_prog_enter(void)
{
	u64 start = 0;

	rcu_read_lock();
	preempt_disable();
	if (static_branch_unlikely(&bpf_stats_enabled_key))
		start = sched_clock();
	return start;
}

void notrace __bpf_prog_exit(struct bpf_prog *prog, u64 start)
{
	struct bpf_prog_stats *stats;

	if (static_branch_unlikely(&bpf_stats_enabled_key) &&
	    /* static_key could be enabled in __bpf_prog_enter
	     * and disabled in __bpf_prog_exit.
	     * And vice versa.
	     * Hence check that 'start' is not zero.
	     */
	    start) {
		stats = this_cpu_ptr(prog->aux->stats);
		u64_stats_update_begin(&stats->syncp);
		stats->cnt++;
		stats->nsecs += sched_clock() - start;
		u64_stats_update_end(&stats->syncp);
	}
	preempt_enable();
	rcu_read_unlock();
}

int __weak
arch_prepare_bpf_trampoline(void *image, void *image_end,
			    const struct btf_func_model *m, u32 flags,
			    struct bpf_tramp_progs *tprogs,
			    void *orig_call)
{
	return -ENOTSUPP;
}

static int __init init_trampolines(void)
{
	int i;

	for (i = 0; i < TRAMPOLINE_TABLE_SIZE; i++)
		INIT_HLIST_HEAD(&trampoline_table[i]);
	return 0;
}
late_initcall(init_trampolines);
