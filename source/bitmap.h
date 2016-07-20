/*
 * guzhoudiaoke@126.com
 * 2015-09-08
 */

#ifndef _BITMAP_H_
#define _BITMAP_H_

#include "types.h"
#include "graphics.h"


#pragma pack(1)
typedef struct bitmap_file_header_t {
	u16	bf_type;
	u32 bf_size;
	u16 bf_reserved1;
	u16 bf_reserved2;
	u32 bf_offset_bits;
} bitmap_file_header_t;

typedef struct bitmap_info_header_t {
	u32 bi_size;
	s32 bi_width;
	s32 bi_height;
	u16 bi_planes;
	u16 bi_bit_count;
	u32 bi_compression;
	u32 bi_size_image;
	s32 bi_xpels_per_meter;
	s32 bi_ypels_per_meter;
	u32 bi_clr_used;
	u32 bi_clr_important;
} bitmap_info_header_t;
#pragma pack()

typedef struct bitmap24_t {
	bitmap_file_header_t file_head;
	bitmap_info_header_t info_head;
} bitmap_t;

BOOL draw_bitmap(bitmap_t* p_bmp, s32 dst_x, s32 dst_y, u32 width, u32 height, s32 src_x, s32 src_y);

#endif
