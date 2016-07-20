# guzhoudiaoke@126.com 
# 2015-08-30

.include "include/kernel.inc"

.section .text
.global	_start, idt

.org	0

_start:
    jmp     main

/*
 * this is the first sect of kernel, will load the left kernel(KERNEL_SECT_NUM - 1) sects
 * to address begine with SECT_SIZE(0x200) 
 */
load_lefted_kernel:
	cld
	movl	$TMP_KERNEL_ADDR,       %esi
    addl    $SECT_SIZE,             %esi
	movl	$SECT_SIZE,				%edi
	movl	$(KERNEL_SECT_NUM-1)<<7,%ecx
	rep		movsl
    ret


#----------------------------------------------------------------------------
# 默认的中断处理函数：
#	暂时设为这个函数，后面会修改为正确的函数
#----------------------------------------------------------------------------
.align	2
default_interrupt:
	pushl	%eax
	pushl	%ecx
	pushl	%edx
	push	%ds						# ds，es，fs虽然是16位寄存器，但会以32位形式入栈
	push	%es
	push	%fs

	movl	$DATA_SELECTOR,	%eax
	movw	%ax,			%ds
	movw	%ax,			%es
	movw	%ax,			%fs
	
    push    $0x00                   # b
    push    $0x00                   # g
    push    $0xFF                   # r
	pushl	$200					# 显示字符串的位置top
	pushl	$200					# 显示字符串的位置left
	pushl	$interrupt_msg			# 显示的字符串
	call	draw_string				# 调用显示字符串函数

	popl	%eax
	pop		%fs
	pop		%es
	pop		%ds
	popl	%edx
	popl	%ecx
	popl	%eax

	iret							# 中断返回
	
#----------------------------------------------------------------------------
# 设置GDT：
#	前面boot.s中的GDT所在的位置并不是一个安全的区域，容易被覆盖，此处将设置
#	一个新的GDT
#----------------------------------------------------------------------------
setup_gdt:
	lgdt	gdt_ptr
	ret

#----------------------------------------------------------------------------
# 设置IDT：
#	设置一个新的IDT，它的每一项有8个字节，0-1，6-7字节是偏移量，2-3字节是选择符
#	4-5字节是一些标志。暂时将256个项都指向一个default_int中断门。而真正的中断门
#	将在后面重新安装
#	入口点31...16	|	1000 1110 0000 0000
#	段选择符		|	入口点15...0
#----------------------------------------------------------------------------
setup_idt:
	lea		default_interrupt,		%edx	# edx为默认中断处理程序地址
	movl	$CODE_SELECTOR<<16,		%eax	# eax high 16位为代码段选择符
	movw	%dx,					%ax		# eax low 16位为中断处理程序地址低16位
	movw	$0x8e00,				%dx		# edx low 16位为中断门的一些标志

	movl	$256,					%ecx
	lea		idt,					%edi
loop_set_idt:
	movl	%eax,					(%edi)
	movl	%edx,					4(%edi)
	addl	$8,						%edi
	dec		%ecx
	jne		loop_set_idt

	lidt	idt_ptr							# 加载中断描述符表寄存器值
	ret


main:
	movl	$DATA_SELECTOR,			%eax
	movw	%ax,					%ds
	movw	%ax,					%es
	movw	%ax,					%fs
	movw	%ax,					%gs
	movw	%ax,					%ss
	movl	$STACK_PM_BOTTOM,		%esp

    call    load_lefted_kernel
    call    setup_idt
    call    setup_gdt

	movl	$DATA_SELECTOR,			%eax    # reload all registers after change the gdt
	movw	%ax,					%ds
	movw	%ax,					%es
	movw	%ax,					%fs
	movw	%ax,					%gs
	movw	%ax,					%ss
	movl	$STACK_PM_BOTTOM,		%esp

    call    init
1:
	jmp		1b


interrupt_msg:
	.asciz	"未知的中断"

.align	8
idt:
	.fill	256, 8, 0					# 256项，每项8个字节，都填充为0
idt_ptr:
	.word	256*8-1
	.long	idt

gdt:
	.quad	0x0000000000000000			# 空描述符
	.quad	0x00cf9a000000ffff			# 代码段描述符
	.quad	0x00cf92000000ffff			# 数据段描述符
	.quad	0x0000000000000000			# 留待以后使用
	.quad	0x0000000000000000			# 留待以后使用
gdt_ptr:
	.word	8*5-1
	.long	gdt

