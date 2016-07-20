/*
 * guzhoudiaoke@126.com
 * 2015-09-12 20:00
 */

#ifndef _TIMER_H_
#define _TIMER_H_

#include "types.h"

#define PIT_COUNTER0_PORT	0x40
#define PIT_COUNTER1_PORT	0x41
#define PIT_COUNTER2_PORT	0x42
#define PIT_CONTROL_PORT	0x43

void init_timer();
void do_timer();
u32  get_tick_count();

#endif

