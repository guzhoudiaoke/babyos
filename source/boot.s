#--------------------------------------------------------------
# guzhoudiaoke@126.com
# 2015-08-30
#--------------------------------------------------------------

.include "include/kernel.inc"

.code16
.section ".text"
.global _start

_start:
	jmp		main

#---------------------------------------------------------------
# 清屏：
#	设置屏幕背景色，调色板的索引0指代的颜色为背景色
#	先不考虑效率，只考虑可读性，故ah，al分开赋值
#---------------------------------------------------------------
clear_screen:				# 清屏函数
	movb	$0x06,	%ah		# 功能号0x06
	movb	$0x00,	%al		# 上卷全部行，即清屏
	movb	$0x00,	%ch		# 左上角行
	movb	$0x00,	%ch		# 左上角列	
	movb	$0x18,	%dh		# 右下角行
	movb	$0x4F,	%dl		# 右下角列
	movb	$0x07,	%bh		# 空白区域属性
	int		$0x10
	
	ret

#--------------------------------------------------------------------
# 设置显示模式：
#	1.检查VBE是否存在，即显卡是否支持VESA BIOS EXTENSION
#	2.检查VBE版本，是否为2.0以上
#	3.检查要设置的mode的一些参数，看是否符合要求
#	4.设置显示模式为VBE 0x103（800*600，256色）
#	5.记录新显示模式的一些参数
#	6.若上面检查或设置失败，则设置显示模式为VGA 0x13（320*200，256色）
#--------------------------------------------------------------------
set_video_mode:
    xorw    %ax,                %ax
    movw    %ax,                %ds
    movw    %ax,                %es
	movw	$0x800,				%di
check_vbe:
	movb	$0x4f,				%ah			# 表示使用VBE标准
	movb	$0x00,				%al			# 功能号
	int		$0x10
	cmp		$0x004f,			%ax			# 若有VBE，AX应该为0x004f
	jne		set_mode_vga_0x13
	movw	0x04(%di),			%ax
	cmp		$0x0200,			%ax			# 若VBE版本不是2.0以上
	jb		set_mode_vga_0x13
check_vbe_mode:								# 检查MODE_VBE_0x13的参数
	movw	$VIDEO_MODE_0x118,	%cx
	movb	$0x4f,				%ah			# 表明VBE标准
	movb	$0x01,				%al			# 子功能号
	int		$0x10
	cmpb	$0x00,				%ah			# 是否调用成功
	jne		set_mode_vga_0x13
	cmpb	$0x4f,				%al			# 是否支持该模式
	jne		set_mode_vga_0x13
    movw    (%di),              %ax
    andw    $0x0080,            %ax
    jz      set_mode_vga_0x13
save_video_mode_info:						# 记录切换到的模式的一些参数信息
    movw    $VIDEO_MODE_0x118,  video_mode
	movw	0x12(%di),			%ax
	movw	%ax,				screen_x
	movw	0x14(%di),			%ax
	movw	%ax,				screen_y
	movb	0x19(%di),			%al
	movb	%al,				bits_per_pixel
	movb	0x1b(%di),			%al
	movb	%al,			    memory_model
	movl	0x28(%di),			%eax
	movl	%eax,				video_ram
	movw	$1,					%ax
set_mode_vbe:								# 下面设置模式
	movw	$VIDEO_MODE_0x118,	%bx
	addw	$0x4000,			%bx			# BX第14个比特表示是否使用大的线性缓存区
	movb	$0x4f,				%ah			# 表示使用VBE标准
	movb	$0x02,				%al			# 功能号，表示设置模式
	int		$0x10
	ret

set_mode_vga_0x13:							# 若不支持VBE则设置为VGA 0x13 mode
	movb	$0,					%ah			# 功能号0x0
	movb	$VIDEO_MODE_0x13,	%al			# 显示模式
	int		$0x10
	movw	$320,				screen_x
	movw	$200,				screen_y
	movl	0x28(%di),			%eax
	movl	%eax,				video_ram
	ret

#----------------------------------------------------------------
# 读取软盘一个扇区：
#	使用BIOS INT 0x13中断读软盘，使用前需要设置ES：BX作为缓冲区
#	AX为相对扇区号，基于相对扇区号，为学习软盘的知识，使用了由
#	相对扇区号来读软盘的方式，也可以直接设置读取扇区数而读连续的
#	多个扇区。但好像有不能跨越磁道、不能超过64KB等限制，要小心。
#	柱面号、磁头号、扇区号计算公式如下：
#	柱面号CH = N / 36，令x = N % 36
#	磁头号DH = x / 18，扇区号CL = x % 18 + 1（因为从1开始，故加1）
#-----------------------------------------------------------------
#read_a_sect:
#	movb	$36,	%dl
#	divb	%dl
#	movb	%al,	%ch		# 柱面号=N / 36, 假设x = N % 36
#	movb	%ah,	%al		# AL = N % 36
#	xorb	%ah,	%ah		# AH = 0, 则AX = AL = N % 36
#	movb	$18,	%dl
#	divb	%dl
#	movb	%al,	%dh		# 磁头号DH = x / 18
#	movb	%ah,	%cl		# CL = x % 18
#	incb	%cl				# 扇区号CL = x % 18 + 1
#
#	movb	$0x00,	%dl		# 驱动器号DL = 0，表示第一个软盘即floppya
#	movb	$0x02,	%ah		# 功能号0x02表示读软盘
#	movb	$0x01,	%al		# 读取一个扇区数
#
#re_read:					# 若调用失败（可能是软盘忙损坏等）则重新调用
#	int		$0x13
#	jc		re_read			# 若进位位（CF）被置位，表示调用失败
#	
#	ret
#
#-------------------------------------------------------------------
# 读取内核到内存
#	该函数读取baby OS 的内核到内存，第一个扇区为引导扇区，需要读取
#	的是从第二个扇区（相对扇区号1）开始的KERNEL_SECT_NUM个扇区
#	ES：BX为缓冲区，为读取内核的临时位置0x10000
#-------------------------------------------------------------------
#read_kernel:
#	movw	$TMP_KERNEL_ADDR>>4,%si		
#	movw	%si,				%es		# ES:BX 为缓冲区地址
#	xorw	%bx,				%bx
#	movw	$0x01,				%di		# 相对扇区号
#1:	
#	movw	%di,				%ax		# 将相对扇区号传给AX作为参数
#	call	read_a_sect
#
#	addw	$SECT_SIZE>>4,		%si
#	movw	%si,				%es
#	incw	%di
#	cmpw	$KERNEL_SECT_NUM+1,	%di
#	jne		1b
#
#	ret
#
#-------------------------------------------------------------------
# 从硬盘读取内核到内存
#-------------------------------------------------------------------
disk_addr_packet:
    .byte   0x10                        # [0] size of packet 16 bytes
    .byte   0x00                        # [1] reserved always 0
    .word   0x01                        # [2] blocks to read
    .word   0x00                        # [4] transfer buffer(16 bit offset)
    .word   0x00                        # [6] transfer buffer(16 bit segment)
    .long   0x01                        # [8] starting LBA
    .long   0x00                        # [12]used for upper part of 48 bit LBAs

read_a_sect_hd:
    lea     disk_addr_packet,   %si
    movb    $0x42,              %ah
    movb    $0x80,              %dl
    int     $0x13

    ret


read_kernel_hd:
    lea     disk_addr_packet,   %si
    movw    $TMP_KERNEL_ADDR>>4,6(%si)
    xorw    %cx,                %cx

1:
    call    read_a_sect_hd

    lea     disk_addr_packet,   %si
    movl    8(%si),             %eax
    addl    $0x01,              %eax
    movl    %eax,               (disk_addr_packet + 8)

    movl    6(%si),             %eax
    addl    $512>>4,            %eax
    movl    %eax,               (disk_addr_packet + 6)

	incw	%cx
	cmpw	$KERNEL_SECT_NUM+1,	%cx
	jne		1b

    ret

#--------------------------------------------------------------------
# 移动内核第一个扇区：
#	内核从软盘读取到内存的一个临时位置，现在将第一个扇区移动到内存
#	0x0000处，第一个扇区即load.s，它将会把内核剩余部分移动到它的后面，
#	之所以分两次移动，是因为若内核较大，一次移动会覆盖0x7c00处的代码，
#	即引导扇区的代码，导致运行出错。
#--------------------------------------------------------------------
move_loader:
	cli									# 指明SI，DI递增
	movw	$TMP_KERNEL_ADDR>>4,%ax
	movw	%ax,				%ds		# DS:SI 为源地址
	xorw	%si,				%si
	movw	$0x00,				%ax
	movw	%ax,				%es		# ES:DI 为目标地址
	xorw	%di,				%di
	movw	$SECT_SIZE >> 2,	%cx		# 移动512/4 次
	rep		movsl						# 每次移动4个byte

	ret


#--------------------------------------------------------------------
# 移动GDT及新显示模式的参数信息到指定位置
#	该函数把GDT及参数信息移动到指定的位置，以便于以后使用
#--------------------------------------------------------------------
move_gdt_and_video_info:
	xorw	%ax,						%ax
	movw	%ax,						%ds		# DS:SI 为源地址
	leaw	gdt,						%si
	movw	$GDT_ADDR >> 4,				%ax		# 由要保存的地址来计算段基址
	movw	%ax,						%es		# ES:DI 为目的地址
	xorw	%di,						%di
	movw	$(GDT_SIZE+VIDEO_INFO_SIZE),%cx		# 移动的双字个数
	rep		movsb

	ret

#--------------------------------------------------------------------
# 开启保护模式：	
#	1.关中断
#	2.加载GDT
#	3.开启A20总线，置cr0的PE位，切换到保护模式
#	4.far jmp/call，用一个CS Selector 更新CS 寄存器，开始执行新代码
#--------------------------------------------------------------------
enter_protected_mode:
	cli									# 关中断
	lgdt	gdt_ptr						# 加载GDT

enable_a20:	
	inb		$0x64,			%al			# 从端口0x64读取数据
	testb	$0x02,			%al			# 测试读取数据第二个bit
	jnz		enable_a20					# 忙等待

	movb	$0xdf,			%al
	outb	%al,			$0x60		# 将0xdf写入端口0x60

	movl	%cr0,			%eax		# 读取cr0寄存器
	orl		$0x01,			%eax		# 置位最后以为即PE位
	movl	%eax,			%cr0		# 写cr0寄存器

	ljmp	$CODE_SELECTOR,	$0x00		# 跳转到代码段，即load.s处开始执行
	
	ret 

#--------------------------------------------------------------------
# 开始执行后，会跳转到此处开始执行
#--------------------------------------------------------------------	
main:
	movw	%cs,		        %ax
	movw	%ax,		        %ds
	movw	%ax,		        %es
	movw	%ax,		        %ss
	movw	$STACK_BOOT_BOTTOM,	%sp

	call	clear_screen				# 清屏
	call	set_video_mode				# 设置显示模式
	#call	read_kernel					# 从软盘读取内核
	call	read_kernel_hd				# 从硬盘读取内核
	call	move_loader	                # 将内核第一个扇区load.s移动到0x0000
	call	move_gdt_and_video_info		# 将GDT和显示模式信息保存起来
	call	enter_protected_mode		# 进入保护模式

1:
	jmp		1b

gdt:
	.quad	0x0000000000000000			# 空描述符
	.quad	0x00cf9a000000ffff			# 代码段描述符
	.quad	0x00cf92000000ffff			# 数据段描述符
	.quad	0x0000000000000000			# 留待以后使用
	.quad	0x0000000000000000			# 留待以后使用

video_mode:
    .short  0
screen_x:								# 水平分辨率
    .short	0 
screen_y:								# 垂直分辨率
	.short	0	
bits_per_pixel:
    .byte   0
memory_model:
    .byte   0
video_ram:								# video_ram地址
	.long	0
gdt_ptr:								# 用与lgdt 加载GDT
	.word	video_mode - gdt - 1		# GDT段限长
	.long	GDT_ADDR					# GDT基地址

	.org	0x1fe,	0x90				# 用nop 指令填充
	.word	0xaa55						# 引导扇区标志

