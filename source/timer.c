/*
 * guzhoudiaoke@126.com
 * 2015-09-12 20:00
 */

#include "timer.h"
#include "i8259a.h"
#include "graphics.h"
#include "dmfont.h"
#include "realtime.h"
#include "keyboard.h"
#include "mouse.h"

u8	 io_inb (u16 port);
void io_outb(u8 value, u16 port);
void io_cli();
void io_sti();

volatile u32 timer_ticks;
volatile u32 time_s;

void init_timer()
{
	/* 8253/8254 在PC上输入频率是1193180 Hz，在每个时钟周期(CLK cycle), 
	 * 计数器会减1, 当减到0时，就会产生一个输出，想让系统每10ms产生一次
	 * 中断，即输入频率为100Hz，需要为计数器赋值为11931 */
	u32 value = 11931;

	/* bit 8~7: 计数器选择位，bit 5~4 读/写/锁位，bit 3～1: 计数器模式位
	 * bit 0: 0计数器使用二进制，1计数器使用BCD码 
	 * 0x36，0011 0110：00计数器0; 11先读写低字节，再读写髙字节; 
	 * 011模式3，square wave rate generator; 0计数器使用二进制 */
	io_outb((u8)0x36, PIT_CONTROL_PORT);

	/* 低字节 */
	io_outb((u8)(value & 0xff), PIT_COUNTER0_PORT);
	/* 髙字节 */
	io_outb((u8)(value >> 8), PIT_COUNTER0_PORT);

	set_irq(IRQ_TIMER, (u32)&do_timer);			/* 设置中断处理程序 */
	enable_irq(IRQ_TIMER);						/* 通知8259A开启键盘中断 */
	
	timer_ticks = 0;
	time_s = 0;
}

void do_timer()
{
	timer_ticks++;
	if (timer_ticks % 100 == 0)
	{
		time_s++;
        real_time_t rt;
		get_real_time(&rt);
		
        const u32 x = 100, y = 50;
        rect_t rect = { x, y, 152, 16 };
        rgb_t color_bk = { 0xff, 0xee, 0xdd };
        fill_rectangle(rect, color_bk);

        rgb_t color_text = { 0xff, 0x99, 0x22 };
		draw_dec(rt.rt_year + 2000,	x+0*8,  y, color_text);
        draw_string("-",            x+4*8,  y, color_text);
		draw_dec(rt.rt_month,	    x+5*8,  y, color_text);
        draw_string("-",            x+7*8,  y, color_text);
		draw_dec(rt.rt_day,	        x+8*8,  y, color_text);
        draw_string(" ",            x+10*8, y, color_text);
		draw_dec(rt.rt_hour,	    x+11*8, y, color_text);
        draw_string(":",            x+13*8, y, color_text);
		draw_dec(rt.rt_minute,	    x+14*8, y, color_text);
        draw_string(":",            x+16*8, y, color_text);
		draw_dec(rt.rt_second,	    x+17*8, y, color_text);

	}
	
	/* 设置EOI，通知中断结束，以接收下一次时钟中断 */
	io_outb(0x20, 0x20);
}

u32 get_tick_count()
{
    return timer_ticks;
}
