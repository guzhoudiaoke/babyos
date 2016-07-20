/*
 * guzhoudiaoke@126.com
 * 2015-09-12 19:45
 */

#ifndef _REAL_TIME_H_
#define _REAL_TIME_H_

#include "types.h"

#define CMOS_ADDRESS_PORT	0x70
#define CMOS_DATA_PORT		0x71

typedef struct real_time_t {
	u8 rt_year;
	u8 rt_month;
	u8 rt_day;
	u8 rt_hour;
	u8 rt_minute;
	u8 rt_second;
} real_time_t;

void get_real_time(real_time_t *rt);

#endif
