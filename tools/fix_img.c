/*
 * 修改img 文件大小，使满足512 整数倍
 * guzhoudiaoke@126.com
 * 2012-01-03 14：56
 */
 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main()
{
	FILE *fp;
	fp = fopen("./babyos.img", "r");
	if (fp == NULL) {
		printf("Open babyos.img failed!\n");
		exit(1);
	}
	
	long size_of_file, start_pos;
	int i, fix_size, fix_data = 0x90;
	
	start_pos = ftell(fp);
	fseek(fp, 0L, SEEK_END);
	size_of_file = ftell(fp);
	
	fix_size = 512 - (size_of_file%512);
	printf("sect %ld, fix %d\n", size_of_file / 512, fix_size);
	fclose(fp);
	
	fp = fopen("./babyos.img", "a");
	if (fp == NULL) {
		printf("Open babyos.img failed!\n");
		exit(1);
	}
	for (i = 0; i < fix_size; i++)
		fprintf(fp, "%c", (char)i);
		
	fclose(fp);
	
	return 0;
}
