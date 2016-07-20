/*
 * guzhoudiaoke@126.com
 * 2015-09-20
 */

#include "math.h"

s32 pow(s32 x, u32 e)
{
    s32 ret = 1;
    for (s32 i = 0; i < e; ++i)
        ret *= x;

    return ret;
}

double powlf(double x, u32 e)
{
    double ret = 1.0;
    for (s32 i = 0; i < e; ++i)
        ret *= x;

    return ret;
}

