/*
 * guzhoudiaoke@126.com
 * 2015-09-06
 */

#include "kernel.h"
#include "graphics.h"
#include "harddisk.h"
#include "bitmap.h"
#include "dmfont.h"
#include "traps.h"
#include "i8259a.h"
#include "keyboard.h"
#include "timer.h"
#include "realtime.h"
#include "mouse.h"
#include "mm.h"
#include "ttf.h"

static u8* p_bk_img_mem = NULL;
static u8* p_ttf_mem = NULL;
static u8* p_ttfyh_mem = NULL;
static rgb_t color = {100, 20, 30};

//static const u32 c_bk_img_start_sect = 1;
static const u32 c_bk_img_start_sect = 1024;
static const u32 c_bk_img_sect_num   = 4609;
static const u32 c_ttf_start_sect    = 5633;
static const u32 c_ttf_sect_num      = 20657;
static const u32 c_ttf_mem_size      = 10576012;

static const u32 c_ttfyh_start_sect  = 26293;
static const u32 c_ttfyh_sect_num    = 28275;
static const u32 c_ttfyh_mem_size    = 14476556;

void io_sti();
void io_halt();

static void test_dmfont()
{
    s32 x = 300, y = 200;
    draw_string("baby os, guzhoudiaoke@126.com",            x, y + 20 * 0, color);
    draw_string("天下事有难易乎？",                         x, y + 20 * 1, color);
    draw_string("为之，则难者亦易矣，不为，则易者亦难矣；", x, y + 20 * 2, color);
    draw_string("人之为学有难易乎？",                       x, y + 20 * 3, color);
    draw_string("学之，则难者亦易矣，不学，则易者亦难矣。", x, y + 20 * 4, color);
    draw_hex(0x12345678,                                    x, y + 20 * 5, color);
    draw_dec(sizeof(u32),                                   x, y + 20 * 6, color);
}

static void init_resource()
{
    s32 x = 300, y = 200;
    p_bk_img_mem = alloc_pages(625);
    draw_hex((u32)p_bk_img_mem, x, y + 20 * 7, color);
    harddisk_read(c_bk_img_start_sect, c_bk_img_sect_num, p_bk_img_mem);

    p_ttf_mem = alloc_pages(2582);
    harddisk_read(c_ttf_start_sect, c_ttf_sect_num, p_ttf_mem);

    p_ttfyh_mem = alloc_pages(3535);
    harddisk_read(c_ttfyh_start_sect, c_ttfyh_sect_num, p_ttfyh_mem);
}

static void test_bitmap()
{
    draw_bitmap((bitmap_t*)(p_bk_img_mem), 0, 0, 1024, 768, 0, 0);
    draw_bitmap((bitmap_t*)(BMP_TEST_ADDR), 500, 500, 60, 18, 0, 0);
    draw_mouse();
}

static void test_keyboard()
{
	s32 k_x = 60, k_y = 80;
    while (TRUE)
    {
        if (get_tick_count() % 10)
            io_halt();

        mouse_read();

        u32 key = 0;	
        keyboard_read(&key);
        if (key == 0 || KEY_TYPE(key) != KT_ASCII)
            continue;

        draw_asc16((char)key, k_x, k_y, color);
        k_x += 8;
        if (k_x + 100 > 1000)
        {
            k_x  = 60;
            k_y += 20;
        }
    }
}

static void test_realtime()
{
    return; 
    real_time_t rt;
    get_real_time(&rt);
	
    s32 y = 550;
    draw_hex8(rt.rt_year,	650, y + 20*0, color);
    draw_hex8(rt.rt_month,	650, y + 20*1, color);
    draw_hex8(rt.rt_day,	650, y + 20*2, color);
    draw_hex8(rt.rt_hour,	650, y + 20*3, color);
    draw_hex8(rt.rt_minute,	650, y + 20*4, color);
    draw_hex8(rt.rt_second,	650, y + 20*5, color);
}

static void test_mm()
{
    u32 total_memory_mb = get_total_memory_mb();
    draw_string("total memory(MB): ", 200, 120, color);
    draw_dec(total_memory_mb, 344, 120, color);

    s32 x = 344, y = 140;
    for (s32 i = 0; i < 100; ++i)
    {
        u8* mem = malloc(4 * i);
        draw_hex((u32)mem, x, y, color);
        x += 100;
        if (x >= 1000)
        {
            x = 344;
            y += 16;
        }
    }
}

static void test_ttf()
{
    init_ttf(p_ttfyh_mem, c_ttfyh_mem_size);

    u32 ttf_version = get_ttf_version();
    draw_string("ttf version: ", 200, 150, color);

    u32 ttf_version_major = ttf_version / 65536;
    u32 ttf_version_minor1 = (ttf_version % 65536 + 5) * 10 / 65536;
    u32 ttf_version_minor2 = (ttf_version % 65536 + 50) * 100 / 65536;
    draw_dec(ttf_version_major, 308, 150, color);
    draw_string(".", 316, 150, color);
    draw_dec(ttf_version_minor1, 324, 150, color);
    draw_dec(ttf_version_minor2, 332, 150, color);

    static rgb_t color_ttf = {0xaa, 0x11, 0xbb};
    draw_ttf_char("孤", 000, 600, color_ttf);
    draw_ttf_char("舟", 220, 600, color_ttf);
    draw_ttf_char("钓", 440, 600, color_ttf);
    draw_ttf_char("客", 660, 600, color_ttf);
}

static void test_bezier()
{
    return;
	point_t points[3] = { 
		{200, 400},
		{10,  540},
		{260, 650},
	};

    static rgb_t color_bez = {10, 200, 230};
	draw_bezier3(points, 100, color_bez);
}

static void test_float()
{
    float x = 0;
    for (s32 i = 0; i < 100; ++i)
        x += 0.12;
    s32 y = x;
    draw_dec(y, 332, 750, color);
}

static void do_test()
{
    test_bitmap();
    test_dmfont();
    test_realtime();
    test_mm();
    test_bezier();
    test_ttf();
    test_float();

    test_keyboard();
}

void init(void)
{
    init_video();
    init_mm();
    init_trap();
    init_irq();
    init_8259a();
    init_keyboard();
    init_timer();
    init_harddisk();
    init_mouse();

    io_sti();
    init_resource();

    do_test();
}

