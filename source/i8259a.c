/*
 * guzhoudiaoke@126.com
 * 2015-09-12 15:42
 */

#include "i8259a.h"
#include "gate.h"

u32	isr_table[NR_IRQ];

void io_outb(u8 value, u16 port);
u8	 io_inb (u16 port);

void isr_0(void);
void isr_1(void);
void isr_2(void);
void isr_3(void);
void isr_4(void);
void isr_5(void);
void isr_6(void);
void isr_7(void);
void isr_8(void);
void isr_9(void);
void isr_10(void);
void isr_11(void);
void isr_12(void);
void isr_13(void);
void isr_14(void);
void isr_15(void);

void init_8259a(void)
{
	/* ICW1：0x11, 表示边沿触发、多片级联、需要发送ICW4 */
	io_outb(0x11, 0x20);
	io_outb(0x11, 0xa0);

	/* ICW2：
	 * 重新映射 IRQ0~IRQ7 到 0x20~0x27,重新映射 IRQ8~IRQ15 到 0x28~0x2F */
	io_outb(0x20, 0x21);
	io_outb(0x28, 0xa1);
	
	/* ICW3:
	 * 	A0	D7  D6  D5  D4  D3  D2  D1  D0
	 * 主片 1	S7  S6	S5  S4  S3  S2  S1  S0
	 * 从片 1	0   0   0   0   0   ID2 ID1 ID0
	 * 主片：0x04，表示主芯片的IR2引脚连接一个从芯片
	 * 从片：0x02，表示从片连接到主片的IR2 引脚 */
	io_outb(0x04, 0x21);
	io_outb(0x02, 0xa1);
	
	/* 从片连接到主片的 IRQ2 */
	/* ICW4：0x01，表示8259A芯片被设置成普通嵌套、非中断方式、
	 * 用于8086 及其兼容模式 */
	io_outb(0x01, 0x21);
	io_outb(0x01, 0xa1);
	
	/* 关闭 IRQ0~IRQ7 号中断 */
	io_outb(0xff, 0x21);
	/* 关闭 IRQ8~IRQ15 号中断 */
	io_outb(0xff, 0xa1);
}

void init_irq(void)
{
	set_interrupt_gate(32, (u32)&isr_0);
	set_interrupt_gate(33, (u32)&isr_1);
	set_interrupt_gate(34, (u32)&isr_2);
	set_interrupt_gate(35, (u32)&isr_3);
	set_interrupt_gate(36, (u32)&isr_4);
	set_interrupt_gate(37, (u32)&isr_5);
	set_interrupt_gate(38, (u32)&isr_6);
	set_interrupt_gate(39, (u32)&isr_7);
	set_interrupt_gate(40, (u32)&isr_8);
	set_interrupt_gate(41, (u32)&isr_9);
	set_interrupt_gate(42, (u32)&isr_10);
	set_interrupt_gate(43, (u32)&isr_11);
	set_interrupt_gate(44, (u32)&isr_12);
	set_interrupt_gate(45, (u32)&isr_13);
	set_interrupt_gate(46, (u32)&isr_14);
	set_interrupt_gate(47, (u32)&isr_15);
}

void set_irq(u32 irq_no, u32 addr)
{
	isr_table[irq_no] = addr;
}

void enable_irq(u32 irq_no)
{
	u8 mask;
	
	if (irq_no < 8)
	{
		/* 开启irq_no号中断，即将中断屏蔽寄存器相应位置为0 */
		mask = io_inb(0x21) & (0xff ^ (1 << irq_no));
		io_outb(mask, 0x21);
	}
	else if (irq_no < 16)
	{
		mask = io_inb(0x21) & 0xfb;
		io_outb(mask, 0x21);

		mask = io_inb(0xa1) & (0xff ^ (1 << (irq_no-8)));
		io_outb(mask, 0xa1);
	}
}

