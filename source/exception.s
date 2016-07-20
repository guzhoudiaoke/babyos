/*
 * guzhoudiaoke@126.com
 * 2015-09-12 15:30
 */

.include "include/kernel.inc"

# ----------------------------------------------------------------------------------------------------------------------
# 向量号 助记符 说明						类型		错误号	产生源
# --------------------------------------------------------------------------------------------------------------------
# 0		#DE	除出错						故障		无		DIV或IDIV指令
# 1		#DB	调试						故障/陷阱	无		任何代码或数据引用，或是INT 1指令
# 2		--	NMI中断						中断		无		非屏蔽外部中断
# 3		#BP	断点						陷阱		无		INT 3指令
# 4		#OF	溢出						陷阱		无		INTO指令
# 5		#BR	边界范围超出				故障		无		BOUND指令
# 6		#UD	无效操作码（未定义操作码）	故障		无		UD2指令或保留的操作码。（Pentium Pro中加入的新指令）
# 7		#NM	设备不存在（无数学协处理器）故障		无		浮点或WAIT/FWAIT指令
# 8		#DF	双重错误					异常终止	有（0）	任何可产生异常、NMI或INTR的指令
# 9		--	协处理器段超越（保留）		故障		无		浮点指令（386以后的CPU不产生该异常）
# 10		#TS	无效的任务状态段TSS			故障		有		任务交换或访问TSS
# 11		#NP	段不存在					故障		有		加载段寄存器或访问系统段
# 12		#SS	堆栈段错误					故障		有		堆栈操作和SS寄存器加载
# 13		#GP	一般保护错误				故障		有		任何内存引用和其他保护检查
# 14		#PF	页面错误					故障		有		任何内存引用
# 15		--	（Intel保留，请勿使用）					无 
# 16		#MF	x87 FPU浮点错误（数学错误）	故障		无		x87 FPU浮点或WAIT/FWAIT指令
# 17		#AC	对齐检查					故障		有（0）	对内存中任何数据的引用
# 18		#MC	机器检查					异常终止	无		错误码（若有）和产生源与CPU类型有关（奔腾处理器引进）
# 19		#XF	SIMD浮点异常				故障		无		SSE和SSE2浮点指令（PIII处理器引进）
# 20-31	--	（Intel保留，请勿使用）
# 32-255	--	用户定义（非保留）中断		中断				外部中断或者INT n指令
# ---------------------------------------------------------------------------------------------------------------------


.global int_0_divide_error, int_1_debug, int_2_nmi, int_3_break_point, int_4_overflow 
.global int_5_bounds_check, int_6_invalid_opcode, int_7_device_not_available
.global int_8_double_fault, int_9_coprocessor_seg_overrun, int_10_invalid_tss 
.global int_11_segment_not_present, int_12_stack_segment, int_13_general_protection 
.global int_14_page_fault, int_15_reserved, int_16_coprocessor_error

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
# 72 - 原ss
# 66 - 原esp
# -------------------阶段2，控制权交给中断处理程序之前，CPU自动压栈--------------
# 64 - 原eflags
# 60 - cs					<- 代码段选择符
# 56 - eip					<- 返回地址
# 52 - error_code/0			<- 错误码可能没有，若没有中断处理程序自己压入0，见下面
# -------------------阶段3-------------------------------------------------------
# 48 - 中断服务程序地址		<- 由下面各个处理自己压栈
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
int_service_routine:
	pushal
	push	%ds
	push	%es
	push	%fs
	push	%gs

	movl	$DATA_SELECTOR,	%edx
	movw	%dx,			%ds
	movw	%dx,			%es
	movw	%dx,			%fs
	movw	%dx,			%gs

	movl	48(%esp),		%eax
	call	*%eax					# '*'号表示调用操作数指定地址处的函数

	pop		%gs
	pop		%fs
	pop		%es
	pop		%ds
	popal
	addl	$8,				%esp	# 跳过48，52处的中断处理程序地址和错误号
	iret


# int0 - 处理被0除出错的情况，无出错号，自动压入0
# 当0用作除数时发生
# do_divide_error为C程序，在traps.c中实现
int_0_divide_error:
	pushl	$0
	pushl	$do_divide_error
	jmp		int_service_routine

# int1 - debug调试中断入口点，无错误号，压入0代替
# 当eflags中的TF标志位置位时引发（当发现硬件断点、开启了指令跟踪陷阱或任务交换陷阱
# 或者调试寄存器访问无效时，CPU会产生该异常
int_1_debug:
	pushl	$0
	pushl	$do_debug
	jmp		int_service_routine

# int2 - 非屏蔽中断调用入口点， 无错误号
# 仅有的被赋予固定中断向量的硬件中断。每当接收到NMI信号，CPU内部产生中断向量2并执行
# 标准中断应答周期。NMI通常保留为极为重要的硬件事件使用。
# 当CPU收到一个NMI信号并且开始执行其中断处理过程时，随后所有的硬件中断都将被忽略
int_2_nmi:
	pushl	$0
	pushl	$do_nmi
	jmp		int_service_routine

# int3 - 断点指令引起中断的入口点，无错误号
# 与硬件中断无关，通常由调试器插入被调试的程序的代码中，处理过程同debug
int_3_break_point:
	pushl	$0
	pushl	$do_debug
	jmp		int_service_routine


# int4 - 溢出出错处理中断的入口点，无错误号
# EFLAGS中OF标志位置位时CPU执行INTO指令就会引发该中断。通常用于编译器跟踪算数计算溢出
int_4_overflow:
	pushl	$0
	pushl	$do_overflow
	jmp		int_service_routine

# int5 - 边界检查出错中断的入口点，无错误号
# 当操作数在有效范围以外时引发的中断。当BOUND指令测试失败就会产生该中断
# BOUND指令有三个操作数，若第一个不在另外两个之间，引发该异常
int_5_bounds_check:
	pushl	$0
	pushl	$do_bounds_check
	jmp		int_service_routine

# int6 - 无效操作指令出错中断的入口点，无错误号
# CPU执行机构检测到一个无效的操作码而引起的中断
int_6_invalid_opcode:
	pushl	$0
	pushl	$do_invalid_opcode
	jmp		int_service_routine

# int7 - 设备不存在引起中断的入口点，无错误号
int_7_device_not_available:
	pushl	$0
	pushl	$do_device_not_available
	jmp		int_service_routine

# int8 - 双出错故障，有错误号
# 通常CPU在调用前一个异常的处理程序而又检测到一个新的异常时，两个异常会被串行处理，
# 当CPU不能进行这样的串行处理时，会引发该中断
int_8_double_fault:
	pushl	$do_double_fault
	jmp		int_service_routine

# int9 - 协处理器段超出出错引起中断的入口点，无错误号
# 基本上等同于协处理器出错保护
int_9_coprocessor_seg_overrun:
	pushl	$0
	pushl	$do_coprocessor_seg_overrun
	jmp		int_service_routine

# int10 - 无效的任务状态段（TSS），有错误号
# 当CPU企图切换到一个进程，而该进程的TSS无效时引发该异常
int_10_invalid_tss:
	pushl	$do_invalid_tss
	jmp		int_service_routine

# int11 - 段不存在，有错误号
# 被引用的段不在内存中
int_11_segment_not_present:
	pushl	$do_segment_not_present
	jmp		int_service_routine

# int12 - 堆栈段错误，有错误号
int_12_stack_segment:
	pushl	$do_stack_segment
	jmp		int_service_routine

# int13 - 一般保护性错误，有错误号
int_13_general_protection:
	pushl	$do_general_protection
	jmp		int_service_routine

# int14 - 页错误，有错误号
int_14_page_fault:
	pushl	$do_page_fault
	jmp		int_service_routine

# int15 - 其他Intel保留的入口点引起中断的入口点，无错误号
int_15_reserved:
	pushl	$0
	pushl	$do_reserved
	jmp		int_service_routine

# int16 - 协处理器引起中断的入口点，无错误号
int_16_coprocessor_error:
	pushl	$0
	pushl	$do_coprocessor_error
	jmp		int_service_routine

