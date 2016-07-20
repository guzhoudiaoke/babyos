/*
 * guzhoudiaoke@126.com
 * 2015-09-11 22:45
 */

#include "dmfont.h"
#include "kernel.h"
#include "graphics.h"
#include "encoding.h"

static u8* p_font_asc16_base = (u8*)(FONT_ASC16_ADDR);
static u8* p_font_hzk16_base = (u8*)(FONT_HZK16_ADDR);
static char hex_str[] = "0123456789abcdef";

BOOL draw_asc16(char ch, s32 left, s32 top, rgb_t rgb)
{
    u8* p_asc;
	s32 x, y;

	p_asc = p_font_asc16_base + ch * ASC16_SIZE;
	for (y = 0; y < ASC16_HEIGHT; y++)
	{
		u8 test_bit = 1 << 7;
		for (x = 0; x < ASC16_WIDTH; x++)
		{
			if (*p_asc & test_bit)
				set_pixel(left+x, top+y, rgb.r, rgb.g, rgb.b);

			test_bit >>= 1;
		}
		p_asc++;
	}

	return TRUE;
}

static BOOL draw_hzk16(const u8 ch[2], s32 left, s32 top, rgb_t rgb)
{
	u8 qu_no, wei_no;
	u32 offset;
	s32 y, x;
	u8* p_hzk;

	qu_no  = (u8)ch[0] - HZK_ENC;
	wei_no = (u8)ch[1] - HZK_ENC;
	offset= (94*(qu_no-1) + (wei_no-1)) * HZK16_SIZE;
	p_hzk = p_font_hzk16_base + offset;

	for (y = 0; y < HZK16_HEIGHT; y++)
	{
		u8 test_bit = 1 << 7;
		for (x = 0; x < HZK16_WIDTH; x++)
		{
			if (*(p_hzk + (x & 8 ? 1 : 0)) & test_bit)
				set_pixel(left+x, top+y, rgb.r, rgb.g, rgb.b);	

			if ((test_bit >>= 1) == 0)
				test_bit = 1 << 7;
		}
		p_hzk += 2;
	}

	return TRUE;
}

BOOL draw_string(char* str, s32 left, s32 top, rgb_t rgb)
{
	char* p = str;
	while (*p != '\0')
	{
		if ((*p & 0x80) == 0)
		{
			draw_asc16(*p++, left, top, rgb);
			left += 8;
		}
		else
		{
            const u8* gb2312 = utf8_to_gb2312((u8*)p);
			draw_hzk16(gb2312, left, top, rgb);
            p += 3;
			left += HZK16_WIDTH;
		}
	}

	return TRUE;
}

BOOL draw_hex(u32 number, s32 left, s32 top, rgb_t rgb)
{
	char buf[11];
	s32 i, j;

	buf[0] = '0'; buf[1] = 'x'; buf[10] = '\0';
	j = 2;
	for (i = 7; i >= 0; i--)
	{
		buf[j++] = hex_str[(number >> (i*4)) & 0xf];
	}

	return draw_string(buf, left, top, rgb);
}

BOOL draw_hex8(u8 number, s32 left, s32 top, rgb_t rgb)
{
	char buf[5];

	buf[0] = '0'; buf[1] = 'x'; buf[4] = '\0';
    buf[2] = hex_str[(number >> 4) & 0xf];
    buf[3] = hex_str[(number) & 0xf];

    for (s32 i = 0; i < 5; ++i)
    {
        draw_asc16(buf[i], left, top, rgb);
        left += ASC16_WIDTH;
    }
    return TRUE;
}

BOOL draw_dec(u32 num, s32 left, s32 top, rgb_t rgb)
{
    char buf[11] = {0};
    s32 index = 0;

    if (num == 0)
    {
        buf[0] = hex_str[0];
    }
    else
    {
        while (num > 0)
        {
            buf[index++] = hex_str[num % 10];
            num /= 10;
        }

        s32 left = 0, right = index - 1;
        while (left < right)
        {
            char ch = buf[left];
            buf[left] = buf[right];
            buf[right] = ch;

            left++;
            right--;
        }
    }

    draw_string(buf, left, top, rgb);
    return TRUE;
}

