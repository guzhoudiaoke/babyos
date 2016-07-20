/*
 * guzhoudiaoke@126.com
 * 2015-09-14 21:43
 */

#include "mm.h"
#include "bits.h"
#include "dmfont.h"
#include "malloc.h"

static u32 n_total_memory = 0;
static u8* p_mem_map = (u8*)(1*MB);
static rgb_t color = {0x99, 0x33, 0xff};

u32  io_get_cr0();
void io_set_cr0(u32 cr0);

void check_total_memory()
{
    /* store a copy of cr0 */
    u32 cr0 = io_get_cr0();

    /* invalidate cache */
    __asm__ __volatile__("wbinvd");

    /* cache disable(486+), no-writeback(486+), 32bit mode(386+) */
    io_set_cr0(cr0 | 0x00000001 | 0x40000000 | 0x200000000);

    const u32 test_value = 0x12345678, test_value_revert = 0x87654321;
    register u32* mem;
    u32 mem_mb = 0, addr = 0;

    do {
        ++mem_mb;
        addr += 1024 * 1024;    /* 1MB */
        mem = (u32*)addr;

        s32 old_value = *mem;
        *mem = test_value;

        /* the empty asm calls tell gcc not to rely on what's in it's registers
         * as saved variables(avoid GCC optimisations) */
        __asm__("":::"memory");
        if (*mem != test_value)
            break;

        *mem = test_value_revert;
        __asm__("":::"memory");
        if (*mem != test_value_revert)
            break;
       
        /* restore memory value */
        __asm__("":::"memory");
        *mem = old_value;
    } while (mem_mb < 4096);    /* max 4096MB(4GB) */
    
    /* restore cr0 */
    io_set_cr0(cr0);

    n_total_memory = mem_mb << 20;
}

static void init_mem_map()
{
    if (n_total_memory <= KERNEL_MEM)
        return;

    u32 page_count = n_total_memory >> 12;
    u32 kernel_page_count = KERNEL_MEM >> 12; 

    for (u32 i = 0; i < kernel_page_count; ++i)
        setbit(p_mem_map, i);

    for (u32 i = kernel_page_count; i < page_count; ++i)
        unsetbit(p_mem_map, i);

    init_malloc();
}

void init_mm()
{
    check_total_memory();
    init_mem_map();
}

u32 get_total_memory_mb()
{
    return n_total_memory >> 20;
}

u8* alloc_one_page()
{
    u32 page_count = n_total_memory >> 12;
    u32 kernel_page_count = KERNEL_MEM >> 12; 
    for (u32 i = kernel_page_count; i < page_count; ++i)
    {
        if (!testbit(p_mem_map, i))
        {
            setbit(p_mem_map, i);
            return (u8*)(i << 12);
        }
    }

    return NULL;
}

void free_one_page(u8* addr)
{
    if (((u32)addr & (PAGE_SIZE - 1)) != 0)
        return;

    unsetbit(p_mem_map, ADDR_TO_PAGE(addr));
}

u8* alloc_pages(u32 count)
{
    u32 page_count = n_total_memory >> 12;
    u32 kernel_page_count = KERNEL_MEM >> 12; 
    for (u32 i = kernel_page_count; i < page_count - count; ++i)
    {
        BOOL find = TRUE;
        for (u32 n = 0; n < count; ++n)
        {
            if (testbit(p_mem_map, i+n))
            {
                find = FALSE;
                break;
            }
        }

        if (find)
        {
            for (u32 n = 0; n < count; ++n)
                setbit(p_mem_map, i+n);

            return (u8*)(i << 12);
        }
    }

    return NULL;
}

void free_pages(u8* addr, u32 page_count)
{
    if (((u32)addr & (PAGE_SIZE - 1)) != 0)
        return;

    for (u32 i = 0; i < page_count; ++i)
        unsetbit(p_mem_map, ADDR_TO_PAGE(addr) + i);
}

