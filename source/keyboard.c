/*
 * guzhoudiaoke@126.com
 * 2015-09-12 16:09
 */

#include "keyboard.h"
#include "i8259a.h"
#include "dmfont.h"
#include "queue.h"

extern u32 isr_table[NR_IRQ];
static queue_t kb_queue;
static BOOL b_shift_l, b_shift_r;
static BOOL b_leading_e0;

/* io functions */
u8	 io_inb (u16 port);
void io_outb(u8 value, u16 port);
void io_cli();
void io_sti();
void io_delay();

void do_keyboard()
{
	u8 scan_code = io_inb(0x60);				    /* 读取扫描码 */
	if (!is_queue_full(&kb_queue))				    /* 若队列未满则入队，否则放弃该扫描码 */
	{
		en_queue(&kb_queue, scan_code);	
	}

	io_outb(0x20, 0x20);
}

void init_keyboard(void)
{
	set_irq(IRQ_KEYBOARD, (u32)&do_keyboard);		/* 设置中断处理程序 */
	enable_irq(IRQ_KEYBOARD);						/* 通知8259A开启键盘中断 */
	init_queue(&kb_queue);						    /* 初始化缓存区队列 */

	b_shift_l = FALSE;
	b_shift_r = FALSE;
	b_leading_e0 = FALSE;
}

void keyboard_read(u32* key)
{
	u8 scan_code;
	u32 col;
	BOOL b_shift;

	if (!is_queue_empty(&kb_queue))
	{
		io_cli();
		de_queue(&kb_queue, &scan_code);
		io_sti();

		/* 首先处理E0 开头的情况，将b_leading_e0置位，然后直接返回 */
		if (scan_code == 0xe0)
		{
			b_leading_e0 = TRUE;
			return;
		}
		
		/* 是键被抬起，暂时将键值设为0 */
		if (scan_code & 0x80)
		{
            if (*key == K_SHIFT_L)
                b_shift_l = 0;
            if (*key == K_SHIFT_R)
                b_shift_r = 0;

			*key = 0;
			return;
		}

		b_shift = b_shift_l || b_shift_r;
		col = b_shift ? 1 : 0;
		
		if (b_leading_e0)
		{
			col = 2;
			b_leading_e0 = FALSE;
		}

		*key = keymap[scan_code & 0x7f][col];
		if (*key == K_SHIFT_L)
			b_shift_l = 1;
		if (*key == K_SHIFT_R)
			b_shift_r = 1;
		
		b_shift = b_shift_l || b_shift_r;
	}
}

