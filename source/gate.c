/*
 * guzhoudiaoke@126.com
 * 2015-09-12 12:37
 */ 

#include "gate.h"
#include "kernel.h"

#define TRAP_GATE_FLAG      0x00008f0000000000ULL
#define INTERRUPT_GATE_FLAG 0x00008e0000000000ULL

extern u64 idt;

static void set_gate(u32 index, u32 addr, u64 flag)
{
    u64 idt_item;

	idt_item = flag;                						/* 门的标志 */
	idt_item |= (u64)CODE_SELECTOR << 16;					/* 代码段选择子 */
	idt_item |= ((u64)addr << 32) & 0xffff000000000000ULL;	/* 地址high16位 */
	idt_item |= ((u64)addr) & 0xffff;						/* 地址low16位 */

	*((u64*)((u32)&idt + index*8)) = idt_item;
}

/* set idt by trap gate */
void set_trap_gate(u32 index, u32 addr)
{
    set_gate(index, addr, TRAP_GATE_FLAG);
}

/* set idt by interrupt gate */
void set_interrupt_gate(u32 index, u32 addr)
{
    set_gate(index, addr, INTERRUPT_GATE_FLAG);
}


