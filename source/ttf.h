/*
 * guzhoudiaoke@126.com
 * 2015-09-19
 */

#ifndef _TTF_H_
#define _TTF_H_

#include "types.h"

typedef s16 f16;
typedef u16 uf16;

#pragma pack(1)

typedef struct ttf_dir_entry_s {
	u32	tag;
	u32	checksum;
	u32	offset;
	u32	length;
} ttf_dir_entry_t;

typedef struct ttf_directory_s {
	u32	sfntVersion;
	u16	numTables;
	u16	searchRange;
	u16	entrySelector;
	u16	rangeShift;
} ttf_directory_t;

typedef struct ttf_head_s {
	u32	version;
	u32	fontRevision;
	u32	checksumAdjust;
	u32	magicNo;
	u16	flags;
	u16	unitsPerEm;
	s8	created[8];
	s8	modified[8];
	f16	xMin, yMin, xMax, yMax;
	u16	macStyle, lowestRecPPEM;
	s16	fontDirection, indexToLocFormat, glyphDataFormat;
} ttf_head_t;

typedef struct ttf_cmap_fmt4_s {
	u16	format;
	u16	length;
	u16	version;
	u16	segCountX2;
	u16	searchRange;
	u16	entrySelector;
	u16	rangeShift;
} ttf_cmap_fmt4_t;

typedef struct ttf_cmap_entry_s {
	u16	platformID;
	u16	encodingID;
	u32	offset;
} ttf_cmap_entry_t;

typedef struct ttf_cmap_s {
	u16			        version;
	u16			        numberOfEncodingTables;
	ttf_cmap_entry_t	encodingTable[1];
} ttf_cmap_t;

typedef struct ttf_glyf_s {
	s16	numberOfContours;
	f16	xMin, yMin, xMax, yMax;
} ttf_glyf_t;

#pragma pack()


enum ttf_table_index_e {
    ec_index_head = 0,
    ec_index_cmap,
    ec_index_loca,
    ec_index_glyf,
    ec_table_num,
};

enum flag_glyph_e
{
	ec_oncurve = 1,
	ec_xshort = 2,
	ec_yshort = 4,
	ec_repeat = 8,
	ec_xsame = 16,
	ec_ysame = 32,
};

typedef struct ttf_s {
    u8*                 p_data;
    u32                 n_data_len;
    ttf_dir_entry_t     p_tables[ec_table_num];
    ttf_head_t*         p_head;
    ttf_cmap_fmt4_t*    p_unicode_cmap;
} ttf_t;

void init_ttf(u8* p_ttf, u32 n_data_len);
u32  get_ttf_version();
void draw_ttf_char(u8* utf8, s32 x, s32 y, rgb_t color);

#endif

