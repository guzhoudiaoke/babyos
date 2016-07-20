/*
 * guzhoudiaoke@126.com
 * 2015-09-08
 */

#ifndef _HARDDISK_H_
#define _HARDDISK_H_


#include <types.h>


/* 硬盘端口号 */
#define HD_PORT_DATA			0x1f0		/* 从该端口读取数据 */
#define HD_PORT_ERROR			0x1f1		/* 若发生错误从该端口获取错误状态 */

#define HD_PORT_SECT_COUNT		0x1f2		/* 读/写的扇区数 */
#define HD_PORT_SECT_NO			0x1f3		/* 扇区号S */
#define HD_PORT_CYLINDER_LOW	0x1f4		/* 柱面号C的低字节 */
#define HD_PORT_CYLINDER_HIGH	0x1f5		/* 柱面号C的髙字节 */
#define HD_PORT_DRIVE_HEAD		0x1f6		/* 驱动器/磁头号H */

#define HD_PORT_STATUS			0x1f7		/* 读时，从此处获取硬盘状态 */
#define HD_PORT_COMMAND			0x1f7		/* 写时，从此处发送命令 */

/* 向HD_PORT_COMMAND写入来控制读/写硬盘 */
#define HD_CMD_READ				0x20		/* 读命令 */
#define HD_CMD_WRITE			0x30		/* 写命令 */

/* 硬盘参数信息 */
#define	HD0_CYlINDER_NUM		162			/* 柱面数 */
#define HD0_HEAD_PER_CYLINDER	16			/* 每个柱面磁头数 */
#define HD0_SECT_PER_TRACK		63			/* 每个磁道扇区数 */

BOOL init_harddisk();
BOOL harddisk_read(u32 lba, u32 sects_to_read, u8* buffer);

#endif
