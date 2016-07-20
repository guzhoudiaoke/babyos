/*
 * guzhoudiaoke@126.com
 * 2015-09-06
 */

#include "kernel.h"
#include "graphics.h"
#include "math.h"

video_info_t* p_video_info = NULL;

static BOOL b_inited = FALSE;
static u32  cx_screen = 0;
static u32  cy_screen = 0;
static u8*  p_vram_base_addr = NULL;
static u8   n_bytes_per_pixel = 0;

BOOL init_video()
{
    p_video_info = (video_info_t*)(VIDEO_INFO_ADDR);
    cx_screen = p_video_info->cx_screen;
    cy_screen = p_video_info->cy_screen;
    p_vram_base_addr = p_video_info->p_vram_base_addr;
    n_bytes_per_pixel = p_video_info->n_bits_per_pixel >> 3;
    b_inited = TRUE;

    return TRUE;
}

u32 get_screen_width()
{
    return cx_screen;
}

u32 get_screen_height()
{
    return cy_screen;
}

static BOOL is_pixel_valid(s32 x, s32 y)
{
    if (!b_inited)
        return FALSE;

    if (x < 0 || y < 0 || (u32)x >= cx_screen || (u32)y >= cy_screen)
        return FALSE;

    return TRUE;
}

BOOL set_pixel(s32 x, s32 y, u8 r, u8 g, u8 b)
{
    u8* pvram = NULL;
    if (!is_pixel_valid(x, y))
        return FALSE;

    pvram = p_vram_base_addr + n_bytes_per_pixel*y*cx_screen + n_bytes_per_pixel*x;
    pvram[0] = b;
    pvram[1] = g;
    pvram[2] = r;

    return TRUE;
}

rgb_t get_pixel(s32 x, s32 y)
{
    rgb_t rgb;
    u8* pvram = NULL;

    rgb.r = rgb.g = rgb.b = 0;
    if (!b_inited)
        return rgb;
    if (!is_pixel_valid(x, y))
        return rgb;

    pvram = p_vram_base_addr + n_bytes_per_pixel*y*cx_screen + n_bytes_per_pixel*x;
    rgb.b = *pvram++;
    rgb.g = *pvram++;
    rgb.r = *pvram++;

    return rgb;
}

/*BOOL draw_rectangle(rect_t rect, rgb_t rgb)
{
    if (!b_inited)
        return FALSE;
    return TRUE;
}
*/

BOOL fill_rectangle(rect_t rect, rgb_t rgb)
{
    if (!b_inited)
        return FALSE;

    for (u32 y = 0; y < rect.height; ++y)
    {
        for (u32 x = 0; x < rect.width; ++x)
            set_pixel(rect.left + x, rect.top + y, rgb.r, rgb.g, rgb.b);
    }

    return TRUE;
}

/* Bresenham line algorithm */
static BOOL bresenham_line(s32 start_x, s32 start_y, s32 end_x, s32 end_y, rgb_t color)
{
	s32 dx, dy, x, y, p, k;

	dx = abs(end_x - start_x);
	dy = abs(end_y - start_y);
	k  = (end_x-start_x) * (end_y-start_y);

	if (dx >= dy)
	{
		p = 2*dy - dx;
		if (start_x < end_x)
		{
			x = start_x;
			y = start_y;
		}
		else
		{
			y = end_y;
			x = end_x;
			end_x = start_x;
		}

		while (x < end_x)
		{
			set_pixel(x, y, color.r, color.g, color.b);
			++x;

			if (p < 0)
			{
				p += 2*dy;
			}
			else
			{
				if (k > 0) ++y;
				else --y;

				p += 2*dy - 2*dx;
			}
		}
	}
	else
	{
		p = 2*dx - dy;
		if (start_y < end_y)
		{
			x = start_x;
			y = start_y;
		}
		else
		{
			y = end_y;
			x = end_x;
			end_y = start_y;
		}

		while (y < end_y)
		{
			set_pixel(x, y, color.r, color.g, color.b);
			++y;

			if (p < 0)
			{
				p += 2*dx;
			}
			else
			{
				if (k > 0) ++x;
				else --x;

				p += 2*dx - 2*dy;
			}
		}
	}

	return TRUE;
}


BOOL draw_line(s32 start_x, s32 start_y, s32 end_x, s32 end_y, rgb_t color)
{
	return bresenham_line(start_x, start_y, end_x, end_y, color);
}

/* draw bezier curve
 * n --- Number of control points
 * cp --- Array of control points
 * m --- Number of subdivision
 */
#if 0
void draw_bezier(int n, point_t* cp, int m, rgb_t color)
{
    s32 c[3], p;
    float bezBlendFcn, u;
    CPoint2D bez;
    p = n - 1;

    //c[i] = n!/( i!(n-i)! )
    for ( int i = 0; i <= p; i ++ )
    {
        c[i] = 1;
        for ( int j = p; j > i; j -- )
            c[i] *= j;
        for ( int j = p - i; j > 1; j -- )
            c[i] /= j;
    }

    for ( int i = 0; i <= m; i ++ )
    {
        u = float( i ) / m ;
        bez.x = 0;
        bez.y = 0;

        for ( int j = 0; j < n; j ++ )
        {
            bezBlendFcn = c[j] * pow( (double)u, (int)j ) * 
                pow( (double)(1 - u) , (int)(p - j) );
            bez.x += cp[j].x * bezBlendFcn;
            bez.y += cp[j].y * bezBlendFcn;
        }

        set_pixel(bez.x, bez.y, color.r, color.g, color.b);
    }
}
#endif

#if 0
void draw_bezier3(point_t* cp, int m, rgb_t color)
{
    /* c[i] = n! / i!(n-i)! */
    s32 c[3] = {1, 2, 1};

    point_t pt_pre = {0, 0};
    for (s32 i = 0; i <= m; ++i)
    {
        point_t bez = {0, 0};

        bez.x += cp[0].x * c[0] * pow(i, 0) * pow(m-i , 2) / pow(m, 2);
        bez.y += cp[0].y * c[0] * pow(i, 0) * pow(m-i , 2) / pow(m, 2);

        bez.x += cp[1].x * c[1] * pow(i, 1) * pow(m-i , 1) / pow(m, 2);
        bez.y += cp[1].y * c[1] * pow(i, 1) * pow(m-i , 1) / pow(m, 2);

        bez.x += cp[2].x * c[2] * pow(i, 2) * pow(m-i , 0) / pow(m, 2);
        bez.y += cp[2].y * c[2] * pow(i, 2) * pow(m-i , 0) / pow(m, 2);

        if (i == 0)
        {
            pt_pre.x = bez.x, pt_pre.y = bez.y;
        }
        else
        {
            draw_line(pt_pre.x, pt_pre.y, bez.x, bez.y, color);
            pt_pre.x = bez.x, pt_pre.y = bez.y;
        }
    }
}
#else
void draw_bezier3(point_t* cp, int m, rgb_t color)
{
    s32 c[3] = {1, 2, 1};
    float u;
    point_t bez = {0, 0}, pt_pre = {0, 0};

    for (int i = 0; i <= m; i++)
    {
        u = (float)i / m ;
        bez.x = 0;
        bez.y = 0;

        for (int j = 0; j < 3; j++)
        {
            float bezBlendFcn = c[j] * powlf(u, j) * powlf(1 - u, 2 - j);
            bez.x += cp[j].x * bezBlendFcn;
            bez.y += cp[j].y * bezBlendFcn;
        }

        if (i == 0)
        {
            pt_pre.x = bez.x, pt_pre.y = bez.y;
        }
        else
        {
            draw_line(pt_pre.x, pt_pre.y, bez.x, bez.y, color);
            pt_pre.x = bez.x, pt_pre.y = bez.y;
        }
    }
}
#endif 

/* draw bezier curve
 * n --- Number of control points
 * cp --- Array of control points
 * m --- Number of subdivision
 */
void draw_bezier(int n, point_t* cp, int m, rgb_t color)
{
    s32* c = (s32*)malloc(sizeof(s32) * n);
    float bezBlendFcn, u;
    point_t bez = {0, 0}, pt_pre = {0, 0};
    s32 p = n - 1;

    // c[i] = n! / (i!(n-i)!)
    for (int i = 0; i <= p; i++)
    {
        c[i] = 1;
        for ( int j = p; j > i; j -- )
            c[i] *= j;
        for ( int j = p - i; j > 1; j -- )
            c[i] /= j;
    }

    for (int i = 0; i <= m; i++)
    {
        u = (float)i / m;
        bez.x = 0;
        bez.y = 0;

        for (int j = 0; j < n; j++)
        {
            bezBlendFcn = c[j] * pow((double)u, j) * pow((double)(1 - u) , (p-j));
            bez.x += cp[j].x * bezBlendFcn;
            bez.y += cp[j].y * bezBlendFcn;
        }

        if (i == 0)
        {
            pt_pre.x = bez.x, pt_pre.y = bez.y;
        }
        else
        {
            draw_line(pt_pre.x, pt_pre.y, bez.x, bez.y, color);
            pt_pre.x = bez.x, pt_pre.y = bez.y;
        }
    }

    free(c);
}

