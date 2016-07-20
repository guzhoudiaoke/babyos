/*
 * guzhoudiaoke@126.com
 * 2015-09-08
 */

#include "harddisk.h"
#include "i8259a.h"
#include "dmfont.h"

u8* hd_read_buffer;
u32 hd_sect_to_read;

void io_outb(u8 value, u16 port);
void io_insl(u32 port, u8* buffer, u32 nr);
void io_halt();
u8	 io_inb(u32 port);

static BOOL do_harddisk_read()
{
#if 0
    static rgb_t color = { 0xff, 0xff, 0x66 };
    static s32 x = 10;
    static s32 y = 10;
    draw_dec(hd_sect_to_read, x, y, color);
    x += 36;
    if (x >= 1000)
    {
        x = 0;
        y += 16;
    }
#endif

    /* read from harddisk */
	io_insl(HD_PORT_DATA, hd_read_buffer, 1 << 7);
    hd_read_buffer += 512;
    hd_sect_to_read--;
    
	/* 设置EOI，通知中断结束，以接收下一次中断 */
	io_outb(0x20, 0x20);
    io_outb(0x20, 0xa0);

    return TRUE;
}

BOOL init_harddisk()
{
	set_irq(IRQ_HD, (u32)&do_harddisk_read);		/* 设置中断处理程序 */
	enable_irq(IRQ_HD);						        /* 通知8259A开启硬盘中断 */

    return TRUE;
}

BOOL harddisk_request(u32 lba, u32 sects_to_read)
{
	u32 cylinder_no, head_no, sect_no, temp;

	cylinder_no = lba / (HD0_HEAD_PER_CYLINDER * HD0_SECT_PER_TRACK);
	temp		= lba % (HD0_HEAD_PER_CYLINDER * HD0_SECT_PER_TRACK);
	head_no		= temp / HD0_SECT_PER_TRACK;
	sect_no		= temp % HD0_SECT_PER_TRACK + 1;

    /* check if harddisk is busy, if busy, wait */
	while ((io_inb(HD_PORT_STATUS) & 0x80) != 0)
		;

    /* set sect number and CHS */
	io_outb(sects_to_read, HD_PORT_SECT_COUNT);
	io_outb(sect_no,       HD_PORT_SECT_NO);
	io_outb(cylinder_no,   HD_PORT_CYLINDER_LOW);

	cylinder_no >>= 8;
	io_outb((cylinder_no), HD_PORT_CYLINDER_HIGH);

    /* 0xa0 = 1010 0000,
     * when set head, bit5 and bit7 should be 1,
     * when bit6 == 0, bit0~bit3 is the head no,
     * bit4 is driver no, 0 is HD0 */
	head_no |= 0xa0;
	io_outb((head_no), HD_PORT_DRIVE_HEAD);

    /* send read command */
	io_outb(HD_CMD_READ, HD_PORT_COMMAND);

	return TRUE;
}

BOOL harddisk_read(u32 lba, u32 sects_to_read, u8* buffer)
{
    hd_read_buffer = buffer;
    hd_sect_to_read = sects_to_read;
    for (u32 i = 0; i < sects_to_read; ++i)
    {
        harddisk_request(lba, 1);
        ++lba;
    }

	return TRUE;
}

