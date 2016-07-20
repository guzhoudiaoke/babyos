/*
 * guzhoudiaoke@126.com
 * 2015-09-12 19:45
 */

#include "realtime.h"

u8	 io_inb (u16 port);
void io_outb(u8 value, u16 port);
void io_cli();
void io_sti();

u8 cmos_read(u32 address)
{
	io_outb(0x80 | address, CMOS_ADDRESS_PORT);
	return io_inb(CMOS_DATA_PORT);
}

u8 bcd_to_binary(u8 bcd)
{
	return (bcd >> 4) * 10 + (bcd & 0xf);
}

void get_real_time(real_time_t *rt)
{
	rt->rt_year	  = bcd_to_binary(cmos_read(9));
	rt->rt_month  = bcd_to_binary(cmos_read(8));
	rt->rt_day	  = bcd_to_binary(cmos_read(7));
	rt->rt_hour	  = bcd_to_binary(cmos_read(4));
	rt->rt_minute = bcd_to_binary(cmos_read(2));
	rt->rt_second = bcd_to_binary(cmos_read(0));
}

