/*************************************************************************
	> File:		write_to_hd.c
	> 描述：	将数据写入虚拟硬盘
	> Author:	孤舟钓客
	> Mail:		guzhoudiaoke@126.com 
	> Time:		2013年01月06日 星期日 21时34分43秒
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>

int main()
{
	FILE* fp_in;
	FILE* fp_out;

	long file_size;
	char* buffer;
    int i = 0;

	fp_out = fopen("../harddisk/baby_hd.img", "wb");
	if (fp_out == NULL)
	{
		printf("open baby_hd.img failed");
		exit(1);
	}

	fp_in = fopen("../resource/1024_768.bmp", "rb");
	if (fp_in == NULL)
	{
		printf("open baby.bmp failed");
		exit(1);
	}

	fseek(fp_in, 0L, SEEK_END);
	file_size = ftell(fp_in);
	fseek(fp_in, 0L, SEEK_SET);

	printf("in file size: %ld\n", file_size);

	buffer = (char*)malloc(sizeof(char) * file_size);

	fread(buffer, sizeof(char), file_size, fp_in);

	printf("%c%c%c\n", buffer[0], buffer[1], buffer[2]);
	
	fseek(fp_out, 512, SEEK_SET);

	fwrite(buffer, sizeof(char), file_size, fp_out); 
    free(buffer);

    long fix_size = 80*1024*1024 - file_size - 512;
    for (i = 0; i < fix_size; i++)
		fprintf(fp_out, "%c", (char)i);

	return 0;
}
