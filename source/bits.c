/*
 * guzhoudiaoke@126.com
 * 2015-09-18
 */

#include "bits.h"

/* 将s 第i 位设1 */
void setbit(void *s, u32 i)
{
    u8* v = s;
    v += i >> 3;
    *v |= 1 << (7 - (i%8));
}

/* 将s 中第i 位清零 */
void unsetbit(void *s, u32 i)
{
    u8* v = s;
    /* i为要置位的位，i>>3 为i位所在字节 */
    v += i >> 3;
    /* i%8 为i位在所在字节中的位置 */
    *v &= ~(1 << (7 - (i%8)));
}

/* 测试第i 位 */
s32 testbit(void *s, u32 i)
{
    u8* v = s;
    v += i >> 3;
    return (*v & (1 << (7 - (i%8)))) != 0;
}

u32 revertu32(u32 val)
{
    u8* p_val = (u8*)&val;
    p_val[0] = p_val[0] ^ p_val[3];
    p_val[3] = p_val[0] ^ p_val[3];
    p_val[0] = p_val[0] ^ p_val[3];

    p_val[1] = p_val[1] ^ p_val[2];
    p_val[2] = p_val[1] ^ p_val[2];
    p_val[1] = p_val[1] ^ p_val[2];

    return val;
}

s32 reverts32(s32 val)
{
    u8* p_val = (u8*)&val;
    p_val[0] = p_val[0] ^ p_val[3];
    p_val[3] = p_val[0] ^ p_val[3];
    p_val[0] = p_val[0] ^ p_val[3];

    p_val[1] = p_val[1] ^ p_val[2];
    p_val[2] = p_val[1] ^ p_val[2];
    p_val[1] = p_val[1] ^ p_val[2];

    return val;
}

u16 revertu16(u16 val)
{
    u8* p_val = (u8*)&val;
    p_val[0] = p_val[0] ^ p_val[1];
    p_val[1] = p_val[0] ^ p_val[1];
    p_val[0] = p_val[0] ^ p_val[1];
    return val;
}

s16 reverts16(s16 val)
{
    u8* p_val = (u8*)&val;
    p_val[0] = p_val[0] ^ p_val[1];
    p_val[1] = p_val[0] ^ p_val[1];
    p_val[0] = p_val[0] ^ p_val[1];
    return val;
}

