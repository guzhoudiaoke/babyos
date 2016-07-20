/*
 * guzhoudiaoke@126.com
 * 2015-09-29
 */

#ifndef _MALLOC_H_
#define _MALLOC_H_

#include "types.h"
#include "mm.h"

typedef struct mem_block_s {
    s32                     size;
    u8*                     current;
    u8*                     end;
    struct mem_block_s*     next;
} mem_block_t;

typedef struct mem_pool_s {
    mem_block_t*    blocks;
    mem_block_t*    current;
    mem_block_t*    last;
} mem_pool_t;

void  init_malloc();
void* malloc(u32 size);
void  free(void* pointer);

#endif

