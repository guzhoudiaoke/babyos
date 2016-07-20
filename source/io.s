/*
 * guzhoudiaoke@126.com
 * 2015-09-08
 */

.global io_outb, io_inb, io_insl, 
.global io_halt, io_delay
.global io_sti, io_cli
.global io_get_cr0, io_set_cr0

/* void io_outb(u8 value, u16 port) */
io_outb:
	movl	4(%esp),	%eax
	movl	8(%esp),	%edx
	outb	%al,		%dx
	ret

/* u8 io_inb(u16 port) */
io_inb:
	movl	4(%esp),	%edx
	xor		%eax,		%eax
	inb		%dx,		%al
	ret

/* int io_insl(u16 port, u8* buffer, u32 nr) */
io_insl:
	movl	12(%esp),	%ecx
	movl	8(%esp),	%edi
	movl	4(%esp),	%edx
	cld
	rep		insl
	ret

io_halt:
	hlt
	ret

io_sti:
	sti
	ret

io_cli:
	cli
	ret

io_delay:
	nop
	nop
	nop
	nop
	nop
	ret

io_get_cr0:
    movl    %cr0,       %eax
    ret

io_set_cr0:
	movl	4(%esp),	%eax
    movl    %eax,       %cr0
    ret

