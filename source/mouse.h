/*
 * guzhoudiaoke@126.com
 * 2015-09-13 23:32
 */

#ifndef _MOUSE_H_
#define _MOUSE_H_

#include "types.h"

/* 鼠标控制电路包含在键盘控制电路中 */
#define KB_PORT_DATA            0x60
#define KB_PORT_STATE           0x64
#define KB_PORT_CMD             0x64

#define KB_STATE_SEND_NOT_READY 0x02

/* 模式设定指令 */
#define KB_CMD_SET_MODE         0x60

/* 鼠标模式 */
#define KB_CONTROLLER_MODE      0x47

/* 如果往键盘控制电路发送指令0xd4, 下一个数据会自动发送给鼠标 */
#define KB_CMD_SENDTO_MOUSE     0xd4

/* 激活鼠标指令 */
#define MOUSE_CMD_ENABLE        0xf4

#define MOUSE_IMG_WIDTH         16
#define MOUSE_IMG_HEIGHT        22

typedef struct mouse_data_s {
	u8	data[3];
	u8	phase;
	u8	button;
	s32 x;
	s32 y;
} mouse_data_t;

void init_mouse();
void do_mouse();
void mouse_read();
void draw_mouse();

#endif
