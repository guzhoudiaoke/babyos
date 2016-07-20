#-------------------------------------------------------------
# 文件：kernel.inc
# 描述：定义baby os内核使用的一些常量
# 时间：2012-12-29 22：36
# 作者：孤舟钓客（guzhoudiaoke@126.com）
#-------------------------------------------------------------

# 内核常量定义：
.equ    SECT_SIZE,              512     # 一个扇区大小
.equ	KERNEL_SECT_NUM,		1024	# babyos 内核sect数
.equ    TMP_KERNEL_ADDR,        0x10000 # kernel 临时地址

.equ    STACK_BOOT_BOTTOM,      0x1000
.equ	STACK_PM_BOTTOM,		0xa0000

# memory address
.equ	CODE_SELECTOR,			0x08
.equ	DATA_SELECTOR,			0x10
.equ	IDT_ADDR,				0x90000	
.equ	IDT_SIZE,				(256*8)
.equ	GDT_ADDR,				(IDT_ADDR + IDT_SIZE)
.equ	GDT_LEN,				5
.equ	GDT_SIZE,				(8 * GDT_LEN)

# video info
.equ	VIDEO_INFO_ADDR,		(GDT_ADDR + GDT_SIZE)
.equ    VIDEO_MODE_ADDR,        (VIDEO_INFO_ADDR)
.equ	SCREEN_X_ADDR,			(VIDEO_INFO_ADDR + 2)
.equ	SCREEN_Y_ADDR,			(VIDEO_INFO_ADDR + 4)
.equ    BITS_PER_PIXEL_ADDR,    (VIDEO_INFO_ADDR + 6)
.equ    MEMORY_MODEL_ADDR,      (VIDEO_INFO_ADDR + 7)
.equ	VRAM_ADDR,				(VIDEO_INFO_ADDR + 8)
.equ	VIDEO_INFO_SIZE,		(12)

# video mode
.equ	VIDEO_MODE_0x103,		0x103
.equ	VIDEO_MODE_0x118,		0x118
.equ	VIDEO_MODE_0x11B,		0x11B
.equ	VIDEO_MODE_0x13,		0x13

