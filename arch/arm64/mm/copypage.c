// SPDX-License-Identifier: GPL-2.0-only
/*
 * Based on arch/arm/mm/copypage.c
 *
 * Copyright (C) 2002 Deep Blue Solutions Ltd, All Rights Reserved.
 * Copyright (C) 2012 ARM Ltd.
 */

#include <linux/mm.h>

#include <asm/page.h>
#include <asm/cacheflush.h>
#include <trace/events/kmem.h>

void __cpu_copy_user_page(void *kto, const void *kfrom, unsigned long vaddr)
{
	struct page *page = virt_to_page(kto);
	copy_page(kto, kfrom);
	flush_dcache_page(page);
}
EXPORT_SYMBOL_GPL(__cpu_copy_user_page);

void __cpu_clear_user_page(void *kaddr, unsigned long vaddr)
{
	clear_page(kaddr);
}
EXPORT_SYMBOL_GPL(__cpu_clear_user_page);

DEFINE_STATIC_KEY_FALSE(fast_copy_page_enabled);
EXPORT_SYMBOL_GPL(fast_copy_page_enabled);
void fast_copy_page_switched(const void *from, void *to)
{
	trace_mm_fast_copy_page_switched((unsigned long)from,
					(unsigned long)to);
}
EXPORT_SYMBOL_GPL(fast_copy_page_switched);
