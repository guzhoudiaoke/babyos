#AS=as -gstabs --32 -I ../
AS=as --32 -I ../
CC=gcc -m32
LD=ld -m elf_i386

# -E 仅运行C预处理器
# -nostdinc 不要在标准系统目录中寻找头文件，只搜索-I选项指定的目录及当前目录
CDEP=gcc -E -nostdinc -I./
CFLAGS= -Wall -pedantic -W -nostdlib -nostdinc -Wno-long-long -I./ -fomit-frame-pointer -std=c99

KERNEL_OBJECTS = setup.o init.o math.o graphics.o bitmap.o io.o harddisk.o dmfont.o encoding.o gate.o traps.o exception.o i8259a.o isr.o queue.o keyboard.o realtime.o timer.o mouse.o mm.o bits.o ttf.o malloc.o

all: babyos
	rm *.o baby_resource

babyos: boot kernel
	cat ../resource/font/ASC16 ../resource/font/HZK16 ../resource/bitmap/w_buttons.bmp > baby_resource
	cat boot kernel baby_resource > babyos.img
	../tools/fix_img
	#dd if=babyos.img of=baby_hd.img ibs=512 conv=notrunc
	dd if=boot of=baby_hd.img ibs=416 count=1 conv=notrunc
	dd if=kernel of=baby_hd.img ibs=512 seek=1 conv=notrunc
	dd if=baby_resource of=baby_hd.img ibs=512 seek=401 conv=notrunc
	dd if=../resource/bitmap/1024_768.bmp of=baby_hd.img ibs=512 seek=1024 conv=notrunc
	dd if=../resource/font/simfang.ttf of=baby_hd.img ibs=512 seek=5633 conv=notrunc
	dd if=../resource/font/YaHei.Consolas.1.12.ttf of=baby_hd.img ibs=512 seek=26293 conv=notrunc

boot: boot.o
	${LD} --oformat binary -N -Ttext 0x7c00 -o $@ $<

kernel: ${KERNEL_OBJECTS}
	${LD} --oformat binary -N -Ttext 0x0000 -o $@ ${KERNEL_OBJECTS}
	../tools/fix_sects

clean:
	rm -f boot kernel *.o babyos.img baby_resource


# 用于生成依赖关系，删除Makefile 中babyos_dependencies 后面的内容，然后对每个.c 文件生成它的依赖关系
# 使用方法： make dep
dep:
	sed '/\#\#\# babyos_dependencies/q' < Makefile > tmp_make
	(for i in *.c; do ${CDEP} -M $$i; done) >> tmp_make
	mv tmp_make Makefile

### babyos_dependencies:
bitmap.o: bitmap.c bitmap.h types.h graphics.h math.h
bits.o: bits.c bits.h types.h
dmfont.o: dmfont.c dmfont.h types.h kernel.h graphics.h encoding.h
encoding.o: encoding.c encoding.h types.h
gate.o: gate.c gate.h types.h kernel.h
graphics.o: graphics.c kernel.h graphics.h types.h math.h
harddisk.o: harddisk.c harddisk.h types.h i8259a.h types.h dmfont.h
i8259a.o: i8259a.c i8259a.h types.h gate.h
init.o: init.c kernel.h graphics.h types.h harddisk.h types.h bitmap.h \
 dmfont.h traps.h i8259a.h keyboard.h timer.h realtime.h mouse.h mm.h \
 ttf.h
keyboard.o: keyboard.c keyboard.h types.h i8259a.h dmfont.h queue.h \
 types.h
malloc.o: malloc.c malloc.h types.h mm.h
math.o: math.c math.h types.h
mm.o: mm.c mm.h types.h bits.h dmfont.h
mouse.o: mouse.c mouse.h types.h graphics.h i8259a.h dmfont.h queue.h \
 types.h
queue.o: queue.c queue.h types.h
realtime.o: realtime.c realtime.h types.h
timer.o: timer.c timer.h types.h i8259a.h graphics.h dmfont.h realtime.h \
 keyboard.h mouse.h
traps.o: traps.c traps.h types.h gate.h dmfont.h
ttf.o: ttf.c ttf.h types.h graphics.h bits.h dmfont.h encoding.h
