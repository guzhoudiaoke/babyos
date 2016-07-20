/*
 * guzhoudiaoke@126.com
 * 2015-09-19
 */

#include "ttf.h"
#include "graphics.h"
#include "bits.h"
#include "dmfont.h"
#include "encoding.h"

static ttf_t ttf;

static const u32 ttf_table_names[ec_table_num] = {
    'head', 'cmap', 'loca', 'glyf',
};

static const u16 c_unicode_platform_id = 3;
static const u16 c_unicode_encode_id   = 1;
static const u16 c_cmap_fmt4           = 4;

static rgb_t ttf_debug_color = {0xaa, 0x33, 0xff};
static s32 ttf_debug_x = 600;
static s32 ttf_debug_y = 10;

static void read_tables(u8* p_table_data)
{
    ttf_dir_entry_t* p_data = (ttf_dir_entry_t*)p_table_data;
    u32 tag = revertu32(p_data->tag);

    ttf_dir_entry_t* p_table = NULL;
    for (s32 i = 0; i < ec_table_num; ++i)
    {
        if (tag == ttf_table_names[i])
        {
            p_table = &ttf.p_tables[i];
            break;
        }
    }

	p_table->tag	  = revertu32(p_data->tag);
	p_table->checksum = revertu32(p_data->checksum);
	p_table->offset	  = revertu32(p_data->offset);
	p_table->length	  = revertu32(p_data->length);
}

static u8* get_data(u32 offset)
{
    return (ttf.p_data + offset);
}

static BOOL parse_unicode_cmap()
{
    if ( ttf.p_data == NULL )
		return FALSE;

	ttf_dir_entry_t* p_table_cmap = &ttf.p_tables[ec_index_cmap];
	ttf_cmap_t* p_cmap = (ttf_cmap_t*)get_data(p_table_cmap->offset);
	if (p_cmap == NULL)
		return FALSE;

	int num_cmap = revertu16(p_cmap->numberOfEncodingTables);
	ttf_cmap_entry_t* p_cmap_entry = NULL;
	BOOL bFind = FALSE;
	for (int i = 0; i < num_cmap; i++)
	{
		p_cmap_entry = &(p_cmap->encodingTable[i]);
		if (revertu16(p_cmap_entry->platformID) == c_unicode_platform_id && 
            revertu16(p_cmap_entry->encodingID) == c_unicode_encode_id)
		{
			bFind = TRUE;
			break;
		}
	}

	if ( !bFind )
		return FALSE;

	u32 cmap_fmt4_offset = p_table_cmap->offset + revertu32(p_cmap_entry->offset);
	ttf_cmap_fmt4_t *p_cmap_fmt4 = (ttf_cmap_fmt4_t*)get_data(cmap_fmt4_offset);
	if (p_cmap_fmt4 == NULL || revertu16(p_cmap_fmt4->format) != c_cmap_fmt4)
		return FALSE;

	ttf.p_unicode_cmap = p_cmap_fmt4;
    return TRUE;
}

void init_ttf(u8* p_ttf, u32 n_data_len)
{
    ttf.p_data = p_ttf;
    ttf.n_data_len = n_data_len;

    ttf_directory_t* p_directory = (ttf_directory_t*)ttf.p_data;
    s32 num_tables = revertu16(p_directory->numTables);

    if (num_tables <= 0)
        return;

    /* read tables */
    u8* p_table_data = ttf.p_data + sizeof(ttf_directory_t);
    for (s32 i = 0; i < num_tables; ++i)
    {
        read_tables(p_table_data);
        p_table_data += sizeof(ttf_dir_entry_t);
    }

    /* parse unicode cmap */
    BOOL b_ret = parse_unicode_cmap();
}

u32 get_ttf_version()
{
    if (ttf.p_data == NULL)
        return -1;

    ttf_dir_entry_t* p_table = &ttf.p_tables[ec_index_head];
    ttf_head_t* p_head = (ttf_head_t*)get_data(p_table->offset);
    if (p_head == NULL)
        return -1;

    u32 version = revertu32(p_head->fontRevision);
    return version;
}

u16 get_glyph_index(u16 ucode)
{
    if (ttf.p_data == NULL || ttf.p_unicode_cmap == NULL)
        return 0;

	u32 seg_count       = (revertu16(ttf.p_unicode_cmap->segCountX2) >> 1);
	u16* p_end_count    = (u16 *)((u8*)ttf.p_unicode_cmap + sizeof(ttf_cmap_fmt4_t));
	u16* p_start_count  = p_end_count   + seg_count + 1;
	u16* p_id_delta     = p_start_count + seg_count;			/* Delta for all character codes in segment */
	u16* p_id_offset    = p_id_delta    + seg_count;			/* Offsets into glyphIdArray or 0 */
	if ((u8*)p_id_offset > ttf.p_data + ttf.n_data_len)
		return 0;

	u32 index = -1;
	for (u32 i = 0; i < seg_count; ++i)
	{
		if (revertu16(p_start_count[i]) <= ucode && ucode <= revertu16(p_end_count[i]))
		{
			index = i;
			break;
		}
	}
	if (index == -1)
		return 0;

	/* 
	 * 1) 如果idRangeOffset数组中的第i个数为0，则该汉字的glyphindex为其unicode码
     *    加上idDelta数组中的第i个数，然后对65536取余。
	 * 2) 如果idRangeOffset数组中的第i个数不为0，则该汉字的glyphindex存放在glyphIdArray数组中，
	 *    并且存放位置与当前位置相差idRangeOffset[i]/2+(c-startCount[i])个u16，
	 *    即要求glyphindex的存放地址为&idRangeOffset[i]+idRangeOffset[i]+2*(c-startCount[i])。
	 */
    u16 glyph_index = 0;
	if (p_id_offset[index] == 0)
	{
		glyph_index = (ucode + revertu16(p_id_delta[index])) & 0xFFFF;
	}
	else
	{
		/* *(idRangeOffset[i]/2 + (c - startCount[i]) + &idRangeOffset[i]) */
		glyph_index = revertu16(*(p_id_offset + index + revertu16(p_id_offset[index]) / 2 + 
            (ucode - revertu16(p_start_count[index]))));
	}

	return glyph_index;
}

static BOOL get_loc_format()
{
    if (ttf.p_data == NULL)
        return TRUE;

    ttf_dir_entry_t* p_table = &ttf.p_tables[ec_index_head];
    ttf_head_t* p_head = (ttf_head_t*)get_data(p_table->offset);
    if (p_head == NULL)
        return TRUE;

    return (p_head->indexToLocFormat == 0);
}

static ttf_glyf_t* get_glyph(u16 ucode)
{
    u16 glyph_index = get_glyph_index(ucode);
    BOOL is_loc_format_short = get_loc_format();
    ttf_dir_entry_t* p_table_loca = &ttf.p_tables[ec_index_loca];
    ttf_dir_entry_t* p_table_glyf = &ttf.p_tables[ec_index_glyf];

    u32 offset_to_glyph = 0;
    u8* p_loca_data = get_data(p_table_loca->offset);
    if (p_loca_data == NULL)
        return NULL;

    if (is_loc_format_short)
    {
        u16* p_loc_to_index = (u16*)p_loca_data;
        offset_to_glyph = p_loc_to_index[glyph_index];
    }
    else
    {
        u32* p_loc_to_index = (u32*)p_loca_data;
        offset_to_glyph = p_loc_to_index[glyph_index];
    }

    ttf_glyf_t* p_glyf = (ttf_glyf_t*)get_data(p_table_glyf->offset);
    if (p_glyf == NULL)
        return NULL;

    return (ttf_glyf_t*)((u8*)p_glyf + revertu32(offset_to_glyph));
}

u16 get_units_per_em()
{
    if (ttf.p_data == NULL)
        return 0xffff;

    ttf_dir_entry_t* p_table = &ttf.p_tables[ec_index_head];
    ttf_head_t* p_head = (ttf_head_t*)get_data(p_table->offset);
    if (p_head == NULL)
        return 0xffff;

    return (p_head->unitsPerEm == 0);
}

static point_t translate_point(s32 base_x, s32 base_y, s32 x, s32 y, u16 units_per_em)
{
    point_t pt = {base_x + x/10, base_y + units_per_em - y/10};
    //point_t pt = {base_x + x, base_y + units_per_em - y};
    return pt;
}

static point_t ttf_draw_pixel(s32 x, s32 y, rgb_t color)
{
    rect_t rc = {x - 2, y - 2, 4, 4};
    fill_rectangle(rc, color);
}


static BOOL ttf_bezier_to(point_t pt0, point_t pt1, point_t pt2, rgb_t color)
{
	point_t points[3] = { 
		pt0,
		pt1,
		pt2,
	};

	draw_bezier3(points, 10, color);
    return TRUE;
}

static BOOL ttf_bezier4_to(point_t pt0, point_t pt1, point_t pt2, point_t pt3, rgb_t color)
{
	point_t points[4] = { 
		pt0,
		pt1,
		pt2,
		pt3,
	};

	draw_bezier(4, points, 10, color);
    return TRUE;
}

static void draw_glyph_char(u16 ucode, s32 x, s32 y, rgb_t color)
{
    ttf_glyf_t* p_glyph = get_glyph(ucode);

    /* parse glyph data */
    static const u32 c_glyf_size = 2000;
    u8 flags[c_glyf_size];

	s16  n_contours = reverts16(p_glyph->numberOfContours);
	u16* p_end_pts_of_contours = (u16*)((u8*)p_glyph + sizeof(ttf_glyf_t));
	u16* p_instruction_length = p_end_pts_of_contours + n_contours;
	u16* p_instructions = p_instruction_length + 1;
	u8*  p_data = ((u8*)(p_instructions) + revertu16(*p_instruction_length));

	s32 n_total_pt_num = revertu16(p_end_pts_of_contours[n_contours - 1]);
	s32 point = 0;
	s32 data_offset = 0;
	while (point <= n_total_pt_num)
	{
		u8 flag = p_data[data_offset++];
		flags[point++] = flag;
		if (flag & ec_repeat)
		{
			s32 repeat = p_data[data_offset++];
			for (s32 i = 0; i < repeat; ++i)
				flags[point++] = flag;
		}
	}

	/* the position of a point is not stored in absolute terms but 
     * as a vector relative to the previous point. 
     */
    s16 x_coord_absolute[c_glyf_size];
	for (s32 i = 0; i <= n_total_pt_num; ++i)
	{
		u8 flag = flags[i];
		s16 x_relative, x_absolute;
		
		/* If set, the corresponding x-coordinate is 1 byte long, not 2. */
		if (flag & ec_xshort)
		{
			/* This flag has two meanings, depending on how the x-Short Vector flag is set. 
			 * If x-Short Vector is set, this bit describes the sign of the value, 
			 * with 1 equalling positive and 0 negative. 
			 */
			if (flag & ec_xsame)
				x_relative = p_data[data_offset++];
			else
				x_relative = -p_data[data_offset++];
		}
		else
		{
			/* If the x-Short Vector bit is not set and this bit is set, 
			 * then the current x-coordinate is the same as the previous x-coordinate. 
			 * If the x-Short Vector bit is not set and this bit is also not set, 
			 * the current x-coordinate is a signed 16-bit delta vector.  
			 */
			if (flag & ec_xsame )
            {
				x_relative = 0;
            }
			else
            {
				x_relative = (s16)p_data[data_offset] * 256 + p_data[data_offset + 1];
				data_offset += 2;
			}
		}

		if (i == 0)
			x_absolute = x_relative;
		else
			x_absolute = x_relative + x_coord_absolute[i - 1];

		x_coord_absolute[i] = x_absolute;
	}

    s16 y_coord_absolute[c_glyf_size];
	for (int i = 0; i <= n_total_pt_num; ++i)
	{
		u8 flag = flags[i];
		s16 y_relative = 0, y_absolute = 0;
		
		if (flag & ec_yshort)
		{
			if ( flag & ec_ysame )
				y_relative = p_data[data_offset++];
			else
				y_relative = -p_data[data_offset++];
		}
		else
		{
			if (flag & ec_ysame)
			{
				y_relative = 0;
			}
			else
			{
				y_relative = (s16)p_data[data_offset] * 256 + p_data[data_offset + 1];
				data_offset += 2;
			}
		}

		if (i == 0)
			y_absolute = y_relative;
		else
			y_absolute = y_relative + y_coord_absolute[i - 1];

		y_coord_absolute[i] = y_absolute;
	}

    /* draw glyph */
	u16 units_per_em = 100;
	//u16 units_per_em = get_units_per_em();
	if (units_per_em == 0xFFFF)
		return;

	s32 pt_index = 0;		/* 当前点的索引 */
	for (int i = 0; i < n_contours; ++i)
	{
		s32 contour_start_pt = pt_index;
		s32 contour_end_pt = revertu16(p_end_pts_of_contours[i]);
		BOOL is_first_pt = TRUE;

        point_t pt_pre = {0, 0};
		while (pt_index <= contour_end_pt)
		{
			u8 flag = flags[pt_index];
			point_t pt = translate_point(x, y, x_coord_absolute[pt_index], y_coord_absolute[pt_index], units_per_em);

			if (is_first_pt)
			{
				is_first_pt = FALSE;
                pt_pre.x = pt.x, pt_pre.y = pt.y;
			}
			else if (flag & ec_oncurve)
			{
                draw_line(pt_pre.x, pt_pre.y, pt.x, pt.y, color);
                pt_pre.x = pt.x, pt_pre.y = pt.y;
			}
			else
			{
				s32 guide_pt_num = 0, start = pt_index - 1, end = 0;
				while (TRUE)
				{
					if (pt_index == contour_end_pt + 1)
					{
						end = contour_start_pt;
						break;
					}
					else
					{
						u8 flag = flags[pt_index];
						if (flag & ec_oncurve)
						{
							end = pt_index;
							break;
						}
						
						pt_index++;
						guide_pt_num++;
					}
				}

				switch (guide_pt_num)
				{
				case 0:
					{
                        point_t pt = translate_point(x, y, x_coord_absolute[end], y_coord_absolute[end], units_per_em);
                        draw_line(pt_pre.x, pt_pre.y, pt.x, pt.y, color);
                        pt_pre.x = pt.x, pt_pre.y = pt.y;
					}
					break;
				case 1:
                    {
                        point_t pt0, pt1, pt2;
                        pt0 = translate_point(x, y, 
                                              x_coord_absolute[start+0], 
                                              y_coord_absolute[start+0],
                                              units_per_em);
                        pt1 = translate_point(x, y, 
                                              x_coord_absolute[start+1], 
                                              y_coord_absolute[start+1], 
                                              units_per_em);
                        pt2 = translate_point(x, y, 
                                              x_coord_absolute[end],     
                                              y_coord_absolute[end],     
                                              units_per_em);
                        ttf_bezier_to(pt0, pt1, pt2, color);
                        pt_pre = pt2;
                    }
                    break;
                case 2:
					{
                        point_t pt0, pt1, pt2, pt3;
                        pt0 = translate_point(x, y, 
                                              x_coord_absolute[start+0], 
                                              y_coord_absolute[start+0],
                                              units_per_em);
                        pt1 = translate_point(x, y, 
                                              x_coord_absolute[start+0], 
                                              y_coord_absolute[start+0],
                                              units_per_em);
                        pt2 = translate_point(x, y, 
                                              x_coord_absolute[start+2], 
                                              y_coord_absolute[start+2], 
                                              units_per_em);
                        pt3 = translate_point(x, y, 
                                              x_coord_absolute[end],     
                                              y_coord_absolute[end],     
                                              units_per_em);
                        ttf_bezier4_to(pt0, pt1, pt2, pt3, color);
                        pt_pre = pt3;
					}
					break;
				default:
					break;
				}
			}

			rgb_t color_pt = {0xff, 0xff, 0};
			if (pt_index == contour_start_pt)
            {
                color_pt.r = 0xff, color_pt.g = 0x00, color_pt.b = 0x00;
            }
            //ttf_draw_pixel(pt.x, pt.y, color_pt);

			pt_index++;
		}

        point_t pt = translate_point(x, y, x_coord_absolute[contour_start_pt], y_coord_absolute[contour_start_pt], units_per_em);
        draw_line(pt_pre.x, pt_pre.y, pt.x, pt.y, color);
        pt_pre.x = pt.x, pt_pre.y = pt.y;

		pt_index = contour_end_pt + 1;
	}
}

void draw_ttf_char(u8* utf8, s32 x, s32 y, rgb_t color)
{
    u8 unicode[2];
    utf8_to_unicode(utf8, unicode);

    u16 ucode = (unicode[0] << 8) | unicode[1];
    draw_glyph_char(ucode, x, y, color);
}

