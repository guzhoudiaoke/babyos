/*
 * guzhoudiaoke@126.com
 * 2015-09-06
 */

#ifndef _KERNEL_H_
#define _KERNEL_H_

/* 选择子 */ 
#define	CODE_SELECTOR		0x08
#define	DATA_SELECTOR		0x10 

/* GDT和IDT内存地址和大小 */
#define	IDT_ADDR			(0x90000)	
#define	IDT_SIZE			(256*8)
#define	GDT_ADDR			(IDT_ADDR + IDT_SIZE)
#define	GDT_LEN				(5)
#define	GDT_SIZE			(8 * GDT_LEN)

/* 显示模式的一些信息的内存地址 */
#define	VIDEO_INFO_ADDR		(GDT_ADDR + GDT_SIZE)

/* asc & hzk in floppy */
#define	FONT_ASC16_ADDR		(400 * 512)		/* 暂定前64个扇区放内核，后面放资源如logo、font */
#define FONT_ASC16_SIZE	    (4096)
#define	FONT_HZK16_ADDR		(FONT_ASC16_ADDR + FONT_ASC16_SIZE)
#define FONT_HZK16_SIZE	    (267616)

/* bmp in fda */
#define	BMP_TEST_ADDR		(FONT_HZK16_ADDR + FONT_HZK16_SIZE)
#define BMP_TEST_SIZE	    (3294)

/* background */
#define BMP_BACKGROUND_ADDR (0x100000)
#define BMP_BACKGROUND_SIZE (2359418)

#endif

