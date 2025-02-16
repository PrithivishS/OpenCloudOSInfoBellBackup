// SPDX-License-Identifier: GPL-2.0
#define _GNU_SOURCE
#include <pthread.h>
#include <sched.h>
#include <sys/socket.h>
#include <test_progs.h>
#include "test_perf_buffer.skel.h"
#include "libbpf_internal.h"

static void on_sample(void *ctx, int cpu, void *data, __u32 size)
{
	int cpu_data = *(int *)data, duration = 0;
	cpu_set_t *cpu_seen = ctx;

	if (cpu_data != cpu)
		CHECK(cpu_data != cpu, "check_cpu_data",
		      "cpu_data %d != cpu %d\n", cpu_data, cpu);

	CPU_SET(cpu, cpu_seen);
}

void test_perf_buffer(void)
{
	int err, on_len, nr_on_cpus = 0,  nr_cpus, i, duration = 0;
	struct perf_buffer_opts pb_opts = {};
	struct test_perf_buffer *skel;
	cpu_set_t cpu_set, cpu_seen;
	struct perf_buffer *pb;
	bool *online;

	nr_cpus = libbpf_num_possible_cpus();
	if (CHECK(nr_cpus < 0, "nr_cpus", "err %d\n", nr_cpus))
		return;

	err = parse_cpu_mask_file("/sys/devices/system/cpu/online",
				  &online, &on_len);
	if (CHECK(err, "nr_on_cpus", "err %d\n", err))
		return;

	for (i = 0; i < on_len; i++)
		if (online[i])
			nr_on_cpus++;

	/* load program */
	skel = test_perf_buffer__open_and_load();
	if (CHECK(!skel, "skel_load", "skeleton open/load failed\n"))
		goto out_close;

	/* attach probe */
	err = test_perf_buffer__attach(skel);
	if (CHECK(err, "attach_kprobe", "err %d\n", err))
		goto out_close;

	/* set up perf buffer */
	pb_opts.sample_cb = on_sample;
	pb_opts.ctx = &cpu_seen;
	pb = perf_buffer__new(bpf_map__fd(skel->maps.perf_buf_map), 1, &pb_opts);
	if (CHECK(IS_ERR(pb), "perf_buf__new", "err %ld\n", PTR_ERR(pb)))
		goto out_close;

	/* trigger kprobe on every CPU */
	CPU_ZERO(&cpu_seen);
	for (i = 0; i < nr_cpus; i++) {
		if (i >= on_len || !online[i]) {
			printf("skipping offline CPU #%d\n", i);
			continue;
		}

		CPU_ZERO(&cpu_set);
		CPU_SET(i, &cpu_set);

		err = pthread_setaffinity_np(pthread_self(), sizeof(cpu_set),
					     &cpu_set);
		if (err && CHECK(err, "set_affinity", "cpu #%d, err %d\n",
				 i, err))
			goto out_close;

		usleep(1);
	}

	/* read perf buffer */
	err = perf_buffer__poll(pb, 100);
	if (CHECK(err < 0, "perf_buffer__poll", "err %d\n", err))
		goto out_free_pb;

	if (CHECK(CPU_COUNT(&cpu_seen) != nr_on_cpus, "seen_cpu_cnt",
		  "expect %d, seen %d\n", nr_on_cpus, CPU_COUNT(&cpu_seen)))
		goto out_free_pb;

out_free_pb:
	perf_buffer__free(pb);
out_close:
	test_perf_buffer__destroy(skel);
	free(online);
}
