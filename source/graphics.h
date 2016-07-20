/*
 * guzhoudiaoke@126.com
 * 2015-09-06
 */

#ifndef _GRAPHICS_H_
#define _GRAPHICS_H_

#include "types.h"

typedef struct vidoe_info_s {
    u16 video_mode;
    u16 cx_screen;
    u16 cy_screen;
    u8  n_bits_per_pixel;
    u8  n_memory_model;
    u8* p_vram_base_addr;
} video_info_t;


BOOL    init_video();
u32     get_screen_width();
u32     get_screen_height();

BOOL    set_pixel(s32 x, s32 y, u8 r, u8 g, u8 b);
rgb_t   get_pixel(s32 x, s32 y);
BOOL    draw_rectangle(rect_t rect, rgb_t rgb);
BOOL    fill_rectangle(rect_t rect, rgb_t rgb);
BOOL    draw_line(s32 start_x, s32 start_y, s32 end_x, s32 end_y, rgb_t color);
void    draw_bezier3(point_t* cp, int m, rgb_t color);

#endif
