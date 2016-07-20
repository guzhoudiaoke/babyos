/*
 * guzhoudiaoke@126.com
 * 2015-09-29
 */

#include "malloc.h"

#define ALIGN_SIZE  (8)
#define BLOCK_SIZE  (4096*1024)
#define MEM_ALIGN(size, align) (((s32)(size) + ((align) - 1)) & ~((align) - 1))

static mem_pool_t pool;

static mem_block_t* alloc_block(u32 size)
{
    u32 minsize = size + sizeof(mem_block_t);
    minsize = MEM_ALIGN(minsize, BLOCK_SIZE);

    u32 pagenum = minsize / (4094 * 1024);
    mem_block_t* block = (mem_block_t*)alloc_pages(pagenum);
    if (block == NULL)
        return NULL;

    block->size = minsize;
    block->current = block + sizeof(mem_block_t);
    block->end = block + minsize;
    block->next = NULL;

    return block;
}

void init_malloc()
{
    mem_block_t* block = alloc_block(BLOCK_SIZE);
    pool.blocks = block;
    pool.current = block;
}

void* malloc(u32 size)
{
    mem_block_t* block = pool.current;
    while (block != NULL)
    {
        u8* mem = MEM_ALIGN(block->current, ALIGN_SIZE);
        if ((u32)(block->end - mem) >= size)
            break;

        block = pool.current->next;
    }

    if (block == NULL)
    {
        block = alloc_block(size);
        pool.last->next = block;
        pool.last = block;
    }

    u8* mem = MEM_ALIGN(block->current, ALIGN_SIZE);
    if ((u32)(block->end - mem) >= size)
    {
        block->current += size;
        return mem;
    }

    return NULL;
}

void free(void* pointer)
{
}

