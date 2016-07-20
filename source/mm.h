/*
 * guzhoudiaoke@126.com
 * 2015-09-14 21:43
 */

#ifndef _MM_H_
#define _MM_H_

#include "types.h"

#define KB              1024
#define MB              1024*KB
#define PAGE_SIZE       4*KB
#define KERNEL_MEM      4*MB

#define ADDR_TO_PAGE(addr)  ((u32)((u32)addr >> 12))
#define PAGE_TO_ADDR(page)  ((u8*)(page << 12))

void init_mm();
u32  get_total_memory_mb();
u8*  alloc_one_page();
void free_one_page(u8* addr);
u8*  alloc_pages(u32 count);
void free_pages(u8* addr, u32 page_count);

#endif
