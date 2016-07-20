/*
 * guzhoudiaoke@126.com
 * 2015-09-08
 */

#ifndef _MATH_H_BABYOS_
#define _MATH_H_BABYOS_

#include "types.h"

#define max(x, y)	(x) > (y) ? (x) : (y)
#define min(x, y)	(x) < (y) ? (x) : (y)

#define abs(x)		(x) > 0 ? (x) : (-(x))
#define pow2(x)     ((x)*(x))

s32 pow(s32 x, u32 e);
double powlf(double x, u32 e);

#endif
