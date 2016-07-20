/*
 * guzhoudiaoke@126.com
 * 2015-09-08
 */

#include "bitmap.h"
#include "math.h"

BOOL draw_bitmap(bitmap_t* p_bmp, s32 dst_x, s32 dst_y, u32 width, u32 height, s32 src_x, s32 src_y)
{
    s32 x, y, bmp_cx, bmp_cy, bmp_x, bmp_y, line_byte_count;
	u8* p_bmp_data_addr = (u8*)p_bmp + p_bmp->file_head.bf_offset_bits;
	u8* p_data;

	/* 不是24位位图 */
	if (p_bmp->info_head.bi_bit_count != 24)
		return FALSE;

	/* 图像的宽、髙 */
	bmp_cx = abs(p_bmp->info_head.bi_width);
	bmp_cy = abs(p_bmp->info_head.bi_height);

	if (src_x > bmp_cx || src_y > bmp_cy || (s32)(src_x+width) < 0 || (s32)(src_y+height) < 0)
		return FALSE;

	line_byte_count = (((bmp_cx*p_bmp->info_head.bi_bit_count + 31) >> 5) << 2);
	for (y = dst_y; y < (s32)(dst_y + height); y++)
	{
		bmp_y = (bmp_cy - 1) - (y - dst_y) - src_y;
		
		if (bmp_y < 0) break;
		if (bmp_y == bmp_cy) continue;

		for (x = dst_x; x < (s32)(dst_x + width); x++)
		{
			bmp_x = src_x + x - dst_x;
			if (bmp_x < 0) continue;
			if (bmp_x == bmp_cx) break;
			
			p_data = p_bmp_data_addr + line_byte_count*bmp_y + 3*bmp_x;
			set_pixel(x, y, p_data[2], p_data[1], p_data[0]);
		}
	}

	return TRUE;
}

