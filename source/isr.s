/*
 * guzhoudiaoke@126.com
 * 2015-09-12 15:34
 */


.include "include/kernel.inc"


.global	isr_0, isr_1,  isr_2,  isr_3,  isr_4,  isr_5,  isr_6, isr_7, isr_8
.global isr_9, isr_10, isr_11, isr_12, isr_13, isr_14, isr_15

# 中断处理过程中，若优先级发生变化，会将原ss,esp压入栈中
# 用户程序（进程）将控制权交给中断处理程序之前CPU将至少12字节（EFLAGS、CS、EIP）
# 压入中断处理程序（而不是被中断代码）的堆栈中，即进程的内核态栈中，这种情况与远调用相似
# 有些异常引起中断时，CPU内部会产生一个出错码压入堆栈
# 然后将中断处理程序地址入栈，堆栈指针指向esp_isr处
# 然后所有32位寄存器入栈
# 然后ds，es，fs，gs入栈，此时堆栈指针指向esp_push_all_regs
# 
# 堆栈内容：
# -------------------阶段1，优先级改变，保护原来的堆栈---------------------------
# 78 - 原ss
# 64 - 原esp
# -------------------阶段2，控制权交给中断处理程序之前，CPU自动压栈--------------
# 60 - 原eflags
# 66 - cs					<- 代码段选择符
# 52 - eip					<- 返回地址
# -------------------阶段3-------------------------------------------------------
# 48 - 中断服务程序地址index
# -------------------阶段4-------------------------------------------------------
# 44 - eax
# 40 - ecx
# 36 - edx
# 32 - ebx
# 28 - esp
# 24 - ebp
# 20 - esi
# 16 - edi					<- pushad
# 12 - ds
# 08 - es
# 04 - fs
# 00 - gs
isr_common:
	pushal
	push	%ds
	push	%es
	push	%fs
	push	%gs

	movl	$DATA_SELECTOR,			%edx
	movw	%dx,					%ds
	movw	%dx,					%es
	movw	%dx,					%fs
	movw	%dx,					%gs

	movl	48(%esp),				%eax
	movl	isr_table(, %eax, 4),	%ebx
	
	call	*%ebx							# '*'号表示调用操作数指定地址处的函数

	pop		%gs
	pop		%fs
	pop		%es
	pop		%ds
	popal
	addl	$4,				%esp			# 跳过48，52处的中断处理程序地址和错误号
	iret

isr_0:
	pushl	$0
	jmp		isr_common

isr_1:
	pushl	$1
	jmp		isr_common
	
isr_2:
	pushl	$2
	jmp		isr_common

isr_3:
	pushl	$3
	jmp		isr_common

isr_4:
	pushl	$4
	jmp		isr_common

isr_5:
	pushl	$5
	jmp		isr_common

isr_6:
	pushl	$6
	jmp		isr_common

isr_7:
	pushl	$7
	jmp		isr_common

isr_8:
	pushl	$8
	jmp		isr_common

isr_9:
	pushl	$9
	jmp		isr_common

isr_10:
	pushl	$10
	jmp		isr_common

isr_11:
	pushl	$11
	jmp		isr_common

isr_12:
	pushl	$12
	jmp		isr_common

isr_13:
	pushl	$13
	jmp		isr_common

isr_14:
	pushl	$14
	jmp		isr_common

isr_15:
	pushl	$15
	jmp		isr_common

