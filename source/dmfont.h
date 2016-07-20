/*
 * guzhoudiaoke@126.com
 * 2015-09-11 22:45
 */

#ifndef _DM_FONT_H_
#define _DM_FONT_H_

#include "types.h"

#define ASC16_SIZE      16
#define ASC16_WIDTH     8
#define ASC16_HEIGHT    16

#define HZK16_SIZE      32
#define HZK16_WIDTH     16
#define HZK16_HEIGHT    16
#define HZK_ENC         0xa0

BOOL draw_asc16(char ch, s32 left, s32 top, rgb_t rgb);
BOOL draw_string(char* str, s32 left, s32 top, rgb_t rgb);
BOOL draw_hex(u32 num, s32 left, s32 top, rgb_t rgb);
BOOL draw_hex8(u8 num, s32 left, s32 top, rgb_t rgb);
BOOL draw_dec(u32 num, s32 left, s32 top, rgb_t rgb);

#endif
