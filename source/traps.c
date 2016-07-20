/*
 * guzhoudiaoke@126.com 
 * 2015-09-12 15:26
 */

#include "traps.h"
#include "types.h"
#include "gate.h"
#include "dmfont.h"

static char* exception_msg[] = {
	"int0  #DE 除出错",
	"int1  #DB 调试",
	"int2  --  NMI中断",
	"int3  #BP 断点",
	"int4  #OF 溢出",
	"int5  #BR 边界范围超出",
	"int6  #UD 无效操作码",
	"int7  #NM 设备不存在",
	"int8  #DF 双重错误",
	"int9  --  协处理器段超越",
	"int10 #TS 无效TSS",
	"int11 #NP 段不存在",
	"int12 #SS 堆栈段错误",
	"int13 #GP 一般保护错误",
	"int14 #PF 页面错误",
	"int15 -- （Intel保留）",
	"int16 #MF x87 FPU浮点错误",
	"int17 #AC 对齐检查",
};

/* 声明一些函数原型，定义在exceptino.s中 */
void int_0_divide_error();
void int_1_debug();
void int_2_nmi();
void int_3_break_point();
void int_4_overflow();
void int_5_bounds_check();
void int_6_invalid_opcode();
void int_7_device_not_available();
void int_8_double_fault();
void int_9_coprocessor_seg_overrun();
void int_10_invalid_tss();
void int_11_segment_not_present();
void int_12_stack_segment();
void int_13_general_protection();
void int_14_page_fault();
void int_15_reserved();
void int_16_coprocessor_error();

void io_halt();

static void disp_exception_info(char* str)
{
    rgb_t color = { 0xFF, 0x10, 0x20 };
	draw_string(str, 300+20, 250+50, color);
	io_halt();
}

void do_divide_error()
{
	disp_exception_info(exception_msg[0]);
}

void do_debug()
{
	disp_exception_info(exception_msg[1]);
}

void do_nmi()
{
	disp_exception_info(exception_msg[2]);
}

void do_overflow()
{
	disp_exception_info(exception_msg[4]);
}

void do_bounds_check()
{
	disp_exception_info(exception_msg[5]);
}

void do_invalid_opcode()
{
	disp_exception_info(exception_msg[6]);
}

void do_device_not_available()
{
	disp_exception_info(exception_msg[7]);
}

void do_double_fault()
{
	disp_exception_info(exception_msg[8]);
}

void do_coprocessor_seg_overrun()
{
	disp_exception_info(exception_msg[9]);
}

void do_invalid_tss()
{
	disp_exception_info(exception_msg[10]);
}

void do_segment_not_present()
{
	disp_exception_info(exception_msg[11]);
}

void do_stack_segment()
{
	disp_exception_info(exception_msg[12]);
}

void do_general_protection()
{
	disp_exception_info(exception_msg[13]);
}

void do_page_fault()
{
	disp_exception_info(exception_msg[14]);
}

void do_reserved()
{
	disp_exception_info(exception_msg[15]);
}

void do_coprocessor_error()
{
	disp_exception_info(exception_msg[16]);
}


void init_trap()
{
 	set_trap_gate(0,  (u32)&int_0_divide_error);
 	set_trap_gate(1,  (u32)&int_1_debug); 
 	set_trap_gate(2,  (u32)&int_2_nmi); 
 	set_trap_gate(3,  (u32)&int_3_break_point); 
 	set_trap_gate(4,  (u32)&int_4_overflow);
 	set_trap_gate(5,  (u32)&int_5_bounds_check); 
 	set_trap_gate(6,  (u32)&int_6_invalid_opcode); 
 	set_trap_gate(7,  (u32)&int_7_device_not_available);
 	set_trap_gate(8,  (u32)&int_8_double_fault);
 	set_trap_gate(9,  (u32)&int_9_coprocessor_seg_overrun); 
 	set_trap_gate(10, (u32)&int_10_invalid_tss);
 	set_trap_gate(11, (u32)&int_11_segment_not_present); 
 	set_trap_gate(12, (u32)&int_12_stack_segment);
 	set_trap_gate(13, (u32)&int_13_general_protection);
 	set_trap_gate(14, (u32)&int_14_page_fault);
 	set_trap_gate(15, (u32)&int_15_reserved);
 	set_trap_gate(16, (u32)&int_16_coprocessor_error);

	for (int i = 17; i < 31; i++)
		set_trap_gate(i, (u32)&int_15_reserved);
}

