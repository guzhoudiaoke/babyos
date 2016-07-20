/*
 * guzhoudiaoke@126.com
 * 2015-09-18
 */

#ifndef _BITS_H_
#define _BITS_H_

#include "types.h"

/* 将s 第i 位设1 */
void setbit(void *s, u32 i);

/* 将s 中第i 位清零 */
void unsetbit(void *s, u32 i);

/* 测试第i 位 */
s32 testbit(void *s, u32 i);

u32 revertu32(u32 val);
s32 reverts32(s32 val);
u16 revertu16(u16 val);
s16 reverts16(s16 val);

#endif
