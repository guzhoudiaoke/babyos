/*************************************************************************
	> File:		fix_72sects.c
	> Describe:	使内核大小为400个扇区
	> Author:	孤舟钓客
	> Mail:		guzhoudiaoke@126.com 
	> Time:		2013年01月01日 星期二 17时46分21秒
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FIX_SECT_NUM    400

int main()
{
	FILE *fp;
	fp = fopen("./kernel", "r");
	if (fp == NULL) {
		printf("Open babyos.img failed!\n");
		exit(1);
	}
	
	long size_of_file, start_pos;
	int i, fix_size, fix_data = 0x90;
	
	start_pos = ftell(fp);
	fseek(fp, 0L, SEEK_END);
	size_of_file = ftell(fp);
	
	fix_size = FIX_SECT_NUM*512 - size_of_file;
	printf("sect %ld, fix %d\n", size_of_file / 512, fix_size);
	fclose(fp);
	
	fp = fopen("./kernel", "a");
	if (fp == NULL) {
		printf("Open babyos.img failed!\n");
		exit(1);
	}

	char *str = "os name: baby os\nauthor: guzhoudiaoke@126.com\nsite: http://blog.csdn.net/guzhou_diaoke/article/category/1269296\n";
	fprintf(fp, "%s", str);
	fix_size -= strlen(str);

	for (i = 0; i < fix_size; i++)
		fprintf(fp, "%c", (char)i);
		
	fclose(fp);
	
	return 0;
}
