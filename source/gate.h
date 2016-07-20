/*
 * guzhoudiaoke@126.com
 * 2015-09-12 12:37
 */ 

#ifndef _GATE_H_
#define _GATE_H_

#include "types.h"

void set_trap_gate(u32 index, u32 addr);
void set_interrupt_gate(u32 index, u32 addr);

#endif
