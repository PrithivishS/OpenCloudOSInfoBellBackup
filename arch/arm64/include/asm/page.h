/* SPDX-License-Identifier: GPL-2.0-only */
/*
 * Based on arch/arm/include/asm/page.h
 *
 * Copyright (C) 1995-2003 Russell King
 * Copyright (C) 2012 ARM Ltd.
 */
#ifndef __ASM_PAGE_H
#define __ASM_PAGE_H

#include <asm/page-def.h>

#ifndef __ASSEMBLY__

#include <linux/hardirq.h>
#include <linux/personality.h> /* for READ_IMPLIES_EXEC */
#include <asm/pgtable-types.h>

extern void __cpu_clear_user_page(void *p, unsigned long user);
extern void __cpu_copy_user_page(void *to, const void *from,
				 unsigned long user);
extern void slow_copy_page(void *to, const void *from);
#ifdef CONFIG_KERNEL_MODE_NEON
extern struct static_key_false fast_copy_page_enabled;
extern void fast_copy_page_switched(const void *from, void *to);
extern int fast_copy_page(void *to, const void *from);
extern void pagefault_disable_wrap(void);
extern void pagefault_enable_wrap(void);
static inline void copy_page(void *to, const void *from)
{
	long ret;

	if (!static_branch_unlikely(&fast_copy_page_enabled))
		return slow_copy_page(to, from);

	if (unlikely(in_interrupt()))
		return slow_copy_page(to, from);

	pagefault_disable_wrap();
	ret = fast_copy_page(to, from);
	pagefault_enable_wrap();
	if (ret) {
		fast_copy_page_switched(from, to);
		slow_copy_page(to, from);
	}
}
#else
static inline void copy_page(void *to, const void *from)
{
	slow_copy_page(to, from);
}
#endif
extern void clear_page(void *to);

#define clear_user_page(addr,vaddr,pg)  __cpu_clear_user_page(addr, vaddr)
#define copy_user_page(to,from,vaddr,pg) __cpu_copy_user_page(to, from, vaddr)

typedef struct page *pgtable_t;

extern int pfn_valid(unsigned long);

#include <asm/memory.h>

#endif /* !__ASSEMBLY__ */

#define VM_DATA_DEFAULT_FLAGS \
	(((current->personality & READ_IMPLIES_EXEC) ? VM_EXEC : 0) | \
	 VM_READ | VM_WRITE | VM_MAYREAD | VM_MAYWRITE | VM_MAYEXEC)

#include <asm-generic/getorder.h>

#endif

