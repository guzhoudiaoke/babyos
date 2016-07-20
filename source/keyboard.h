/*
 * guzhoudiaoke@126.com
 * 2015-09-12 16:09
 */

#ifndef _KEYBOARD_H_
#define _KEYBOARD_H_

#include "types.h"

#define NR_SCAN_CODES		0x80		/* 扫描码个数 */
#define NR_CODE_COL			0x03		/* 扫描码列数 */


/* 简单起见暂时先把键盘上所有键分为7类，分类来给每个键一个唯一标号(一个类号，一个标识号)：
 * 可打印的字符、特殊字符、功能键、转换键、锁定键、小键盘上的键、APIC键 */
#define KT_ASCII			0			/* 可打印字符键 */
#define KT_SPECIAL			1			/* 特殊字符键 */
#define KT_FUNCTION			2			/* 功能键 */
#define KT_SHIFT			3			/* 转换键 */
#define KT_LOCK				4			/* 锁定键 */
#define KT_CONTROL			5			/* 中间方向键等控制键盘 */
#define KT_PAD				6			/* 小键盘 */
#define KT_APIC				7			/* APIC键 */

/* KEY用来构造一个唯一标号
 * KEY_TYPE用来获得键的类型
 * KEY_VALUE用来得到键的值 */
#define KEY(type, value)	(((type) << 8) | (value))
#define KEY_TYPE(x)			((x) >> 8)
#define KEY_VALUE(x)		((x) &  0xff)


/* 功能键 */
#define K_F0				KEY(KT_FUNCTION, 0)
#define K_F1				KEY(KT_FUNCTION, 1)
#define K_F2				KEY(KT_FUNCTION, 2)
#define K_F3				KEY(KT_FUNCTION, 3)
#define K_F4				KEY(KT_FUNCTION, 4)
#define K_F5				KEY(KT_FUNCTION, 5)
#define K_F6				KEY(KT_FUNCTION, 6)
#define K_F7				KEY(KT_FUNCTION, 7)
#define K_F8				KEY(KT_FUNCTION, 8)
#define K_F9				KEY(KT_FUNCTION, 9)
#define K_F10				KEY(KT_FUNCTION, 10)
#define K_F11				KEY(KT_FUNCTION, 11)
#define K_F12				KEY(KT_FUNCTION, 12)

/* 特殊字符键 */
#define K_ESC				KEY(KT_SPECIAL, 0)
#define K_BACKSPACE			KEY(KT_SPECIAL, 1)
#define K_TAB				KEY(KT_SPECIAL, 2)
#define K_ENTER				KEY(KT_SPECIAL, 3)
#define K_GUI_L				KEY(KT_SPECIAL, 4)
#define K_GUI_R				KEY(KT_SPECIAL, 5)
#define K_APPS				KEY(KT_SPECIAL, 6)

/* 转换键 */
#define K_SHIFT_L			KEY(KT_SHIFT, 0)
#define K_SHIFT_R			KEY(KT_SHIFT, 1)
#define K_CTRL_L			KEY(KT_SHIFT, 2)
#define K_CTRL_R			KEY(KT_SHIFT, 3)
#define K_ALT_L				KEY(KT_SHIFT, 4)
#define K_ALT_R				KEY(KT_SHIFT, 5)

/* 锁定键 */
#define K_CAPS_LOCK			KEY(KT_LOCK, 0)
#define	K_NUM_LOCK			KEY(KT_LOCK, 1)
#define K_SCROLL_LOCK		KEY(KT_LOCK, 2)

/* 控制键 */
#define K_PRINTSCREEN		KEY(KT_CONTROL, 0)		/* Print Screen	*/
#define K_PAUSEBREAK		KEY(KT_CONTROL, 1)		/* Pause/Break	*/
#define K_INSERT			KEY(KT_CONTROL, 2)		/* Insert	*/
#define K_DELETE			KEY(KT_CONTROL, 3)		/* Delete	*/
#define K_HOME				KEY(KT_CONTROL, 4)		/* Home		*/
#define K_END				KEY(KT_CONTROL, 5)		/* End		*/
#define K_PAGEUP			KEY(KT_CONTROL, 6)		/* Page Up	*/
#define K_PAGEDOWN			KEY(KT_CONTROL, 7)		/* Page Down*/
#define K_UP				KEY(KT_CONTROL, 8)		/* Up		*/
#define K_DOWN				KEY(KT_CONTROL, 9)		/* Down		*/
#define K_LEFT				KEY(KT_CONTROL, 10)		/* Left		*/
#define K_RIGHT				KEY(KT_CONTROL, 11)		/* Right	*/

/* ACPI 键 */
#define K_POWER				KEY(KT_APIC, 0)			/* Power	*/
#define K_SLEEP				KEY(KT_APIC, 1)			/* Sleep	*/
#define K_WAKE				KEY(KT_APIC, 2)			/* Wake Up	*/

/* Num Pad */
#define K_PAD_0				KEY(KT_PAD, 0)			
#define K_PAD_1				KEY(KT_PAD, 1)			
#define K_PAD_2				KEY(KT_PAD, 2)			
#define K_PAD_3				KEY(KT_PAD, 3)			
#define K_PAD_4				KEY(KT_PAD, 4)			
#define K_PAD_5				KEY(KT_PAD, 5)				
#define K_PAD_6				KEY(KT_PAD, 6)				
#define K_PAD_7				KEY(KT_PAD, 7)			
#define K_PAD_8				KEY(KT_PAD, 8)			
#define K_PAD_9				KEY(KT_PAD, 9)			
#define K_PAD_SLASH			KEY(KT_PAD, 10)			/* /		*/
#define K_PAD_STAR			KEY(KT_PAD, 11)			/* *		*/
#define K_PAD_MINUS			KEY(KT_PAD, 12)			/* -		*/
#define K_PAD_PLUS			KEY(KT_PAD, 13)			/* +		*/
#define K_PAD_ENTER			KEY(KT_PAD, 14)			/* Enter	*/
#define K_PAD_DOT			KEY(KT_PAD, 15)			/* .		*/

#define K_PAD_UP			K_PAD_8					/* Up		*/
#define K_PAD_DOWN			K_PAD_2					/* Down		*/
#define K_PAD_LEFT			K_PAD_4					/* Left		*/
#define K_PAD_RIGHT			K_PAD_6					/* Right	*/
#define K_PAD_HOME			K_PAD_7					/* Home		*/
#define K_PAD_END			K_PAD_1					/* End		*/
#define K_PAD_PAGEUP		K_PAD_9					/* Page Up	*/
#define K_PAD_PAGEDOWN		K_PAD_3					/* Page Down*/
#define K_PAD_INS			K_PAD_0					/* Ins		*/
#define K_PAD_MID			K_PAD_5					/* Middle key*/
#define K_PAD_DEL			K_PAD_DOT					/* Del		*/


/* 将0x80个扫描码一一映射成上面的标识号 */
static const u32 keymap[NR_SCAN_CODES][NR_CODE_COL] = {
	/* scancode	!shift				shift			0xE0 0x??	*/
	/* -------------------------------------------------------- */
	/* 0x00 */	{ 0,				0,				0 },
	/* 0x01 */	{ K_ESC,			K_ESC,			0 },
	/* 0x02 */	{ '1',				'!',			0 },
	/* 0x03 */	{ '2',				'@',			0 },
	/* 0x04 */	{ '3',				'#',			0 },
	/* 0x05 */	{ '4',				'$',			0 },
	/* 0x06 */	{ '5',				'%',			0 },
	/* 0x07 */	{ '6',				'^',			0 },
	/* 0x08 */	{ '7',				'&',			0 },
	/* 0x09 */	{ '8',				'*',			0 },
	/* 0x0A */	{ '9',				'(',			0 },
	/* 0x0B */	{ '0',				')',			0 },
	/* 0x0C */	{ '-',				'_',			0 },
	/* 0x0D */	{ '=',				'+',			0 },
	/* 0x0E */	{ K_BACKSPACE,		K_BACKSPACE,	0 },
	/* 0x0F */	{ K_TAB,			K_TAB,			0 },
	/* 0x10 */	{ 'q',				'Q',			0 },
	/* 0x11 */	{ 'w',				'W',			0 },
	/* 0x12 */	{ 'e',				'E',			0 },
	/* 0x13 */	{ 'r',				'R',			0 },
	/* 0x14 */	{ 't',				'T',			0 },
	/* 0x15 */	{ 'y',				'Y',			0 },
	/* 0x16 */	{ 'u',				'U',			0 },
	/* 0x17 */	{ 'i',				'I',			0 },
	/* 0x18 */	{ 'o',				'O',			0 },
	/* 0x19 */	{ 'p',				'P',			0 },
	/* 0x1A */	{ '[',				'{',			0 },
	/* 0x1B */	{ ']',				'}',			0 },
	/* 0x1C */	{ K_ENTER,			K_ENTER,		K_PAD_ENTER },
	/* 0x1D */	{ K_CTRL_L,			K_CTRL_L,		K_CTRL_R },
	/* 0x1E */	{ 'a',				'A',			0 },
	/* 0x1F */	{ 's',				'S',			0 },
	/* 0x20 */	{ 'd',				'D',			0 },
	/* 0x21 */	{ 'f',				'F',			0 },
	/* 0x22 */	{ 'g',				'G',			0 },
	/* 0x23 */	{ 'h',				'H',			0 },
	/* 0x24 */	{ 'j',				'J',			0 },
	/* 0x25 */	{ 'k',				'K',			0 },
	/* 0x26 */	{ 'l',				'L',			0 },
	/* 0x27	*/	{ ';',				':',			0 },
	/* 0x28	*/	{ '\'',				'"',			0 },
	/* 0x29	*/	{ '`',				'~',			0 },
	/* 0x2A	*/	{ K_SHIFT_L,		K_SHIFT_L,		0 },
	/* 0x2B	*/	{ '\\',				'|',			0 },
	/* 0x2C */	{ 'z',				'Z',			0 },
	/* 0x2D */	{ 'x',				'X',			0 },
	/* 0x2E */	{ 'c',				'C',			0 },
	/* 0x2F */	{ 'v',				'V',			0 },
	/* 0x30	*/	{ 'b',				'B',			0 },
	/* 0x31 */	{ 'n',				'N',			0 },
	/* 0x32 */	{ 'm',				'M',			0 },
	/* 0x33 */	{ ',',				'<',			0 },
	/* 0x34 */	{ '.',				'>',			0 },
	/* 0x35 */	{ '/',				'?',			K_PAD_SLASH },
	/* 0x36 */	{ K_SHIFT_R,		K_SHIFT_R,		0 },
	/* 0x37 */	{ '*',				'*',			0 },
	/* 0x38 */	{ K_ALT_L,			K_ALT_L,		K_ALT_R },
	/* 0x39	*/	{ ' ',				' ',			0 },
	/* 0x3A */	{ K_CAPS_LOCK,		K_CAPS_LOCK,	0 },
	/* 0x3B */	{ K_F1,				K_F1,			0 },
	/* 0x3C */	{ K_F2,				K_F2,			0 },
	/* 0x3D */	{ K_F3,				K_F3,			0 },
	/* 0x3E */	{ K_F4,				K_F4,			0 },
	/* 0x3F */	{ K_F5,				K_F5,			0 },
	/* 0x40	*/	{ K_F6,				K_F6,			0 },
	/* 0x41 */	{ K_F7,				K_F7,			0 },
	/* 0x42	*/	{ K_F8,				K_F8,			0 },
	/* 0x43 */	{ K_F9,				K_F9,			0 },
	/* 0x44 */	{ K_F10,			K_F10,			0 },
	/* 0x45 */	{ K_NUM_LOCK,		K_NUM_LOCK,		0 },
	/* 0x46 */	{ K_SCROLL_LOCK,	K_SCROLL_LOCK,	0 },
	/* 0x47 */	{ K_PAD_HOME,		'7',			K_HOME },
	/* 0x48 */	{ K_PAD_UP,			'8',			K_UP },
	/* 0x49 */	{ K_PAD_PAGEUP,		'9',			K_PAGEUP },
	/* 0x4A */	{ K_PAD_MINUS,		'-',			0 },
	/* 0x4B */	{ K_PAD_LEFT,		'4',			K_LEFT },
	/* 0x4C */	{ K_PAD_MID,		'5',			0 },
	/* 0x4D */	{ K_PAD_RIGHT,		'6',			K_RIGHT },
	/* 0x4E	*/	{ K_PAD_PLUS,		'+',			0 },
	/* 0x4F */	{ K_PAD_END,		'1',			K_END },
	/* 0x50	*/	{ K_PAD_DOWN,		'2',			K_DOWN },
	/* 0x51 */	{ K_PAD_PAGEDOWN,	'3',			K_PAGEDOWN },
	/* 0x52	*/	{ K_PAD_INS,		'0',			K_INSERT },
	/* 0x53 */	{ K_PAD_DOT,		'.',			K_DELETE },
	/* 0x54	*/	{ 0,				0,				0 },
	/* 0x55 */	{ 0,				0,				0 },
	/* 0x56	*/	{ 0,				0,				0 },
	/* 0x57	*/	{ K_F11,			K_F11,			0 },	
	/* 0x58	*/	{ K_F12,			K_F12,			0 },	
	/* 0x59 */	{ 0,				0,				0 },	
	/* 0x5A */	{ 0,				0,				0 },	
	/* 0x5B */	{ 0,				0,				K_GUI_L },	
	/* 0x5C	*/	{ 0,				0,				K_GUI_R },	
	/* 0x5D */	{ 0,				0,				K_APPS },	
	/* 0x5E */	{ 0,				0,				0 },	
	/* 0x5F	*/	{ 0,				0,				0 },
	/* 0x60 */	{ 0,				0,				0 },
	/* 0x61	*/	{ 0,				0,				0 },	
	/* 0x62 */	{ 0,				0,				0 },	
	/* 0x63 */	{ 0,				0,				0 },	
	/* 0x64	*/	{ 0,				0,				0 },	
	/* 0x65	*/	{ 0,				0,				0 },	
	/* 0x66	*/	{ 0,				0,				0 },	
	/* 0x67 */	{ 0,				0,				0 },	
	/* 0x68	*/	{ 0,				0,				0 },	
	/* 0x69	*/	{ 0,				0,				0 },	
	/* 0x6A */	{ 0,				0,				0 },	
	/* 0x6B */	{ 0,				0,				0 },	
	/* 0x6C	*/  { 0,				0,				0 },	
	/* 0x6D */	{ 0,				0,				0 },	
	/* 0x6E	*/	{ 0,				0,				0 },	
	/* 0x6F */	{ 0,				0,				0 },	
	/* 0x70	*/	{ 0,				0,				0 },	
	/* 0x71 */	{ 0,				0,				0 },	
	/* 0x72 */	{ 0,				0,				0 },	
	/* 0x73 */	{ 0,				0,				0 },	
	/* 0x74	*/	{ 0,				0,				0 },	
	/* 0x75 */  { 0,				0,				0 },	
	/* 0x76 */	{ 0,				0,				0 },	
	/* 0x77	*/	{ 0,				0,				0 },	
	/* 0x78 */	{ 0,				0,				0 },	
	/* 0x78 */	{ 0,				0,				0 },	
	/* 0x7A */	{ 0,				0,				0 },		
	/* 0x7B */	{ 0,				0,				0 },	
	/* 0x7C */	{ 0,				0,				0 },	
	/* 0x7D	*/	{ 0,				0,				0 },	
	/* 0x7E */	{ 0,				0,				0 },
	/* 0x7F	*/	{ 0,				0,				0 },
};

void init_keyboard(void);
void keyboard_read(u32* key);

#endif
