/*
 * guzhoudiaoke@126.com
 * 2015-09-06
 */

#ifndef _TYPES_H_BABYOS_
#define _TYPES_H_BABYOS_

typedef int     BOOL;
#define TRUE    1
#define FALSE   0
#define NULL    ((void*) 0)

/* basic types */
typedef unsigned char       u8;
typedef unsigned short      u16;
typedef unsigned int        u32;
typedef unsigned long long  u64;

typedef signed char         s8;
typedef signed short        s16;
typedef signed int          s32;
typedef signed long long    s64;

/* color */
typedef struct rgb_s {
    u8  r;
    u8  g;
    u8  b;
} rgb_t;


/* point */
typedef struct point_s {
    s32 x;
    s32 y;
} point_t;


/* rect */
typedef struct rect_s {
    s32 left;
    s32 top;
    u32 width;
    u32 height;
} rect_t;

#endif
