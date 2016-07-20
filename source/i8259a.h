/*
 * guzhoudiaoke@126.com
 * 2015-09-12 15:42
 */

#ifndef _I8259A_H_
#define _I8259A_H_

#include "types.h"

#define NR_IRQ          32
#define IRQ_TIMER		0
#define IRQ_KEYBOARD	1
#define IRQ_MOUSE		12
#define IRQ_HD          14


void init_8259a(void);
void init_irq(void);

void set_irq(u32 irq_no, u32 addr);
void enable_irq(u32 irq_no);

#endif
