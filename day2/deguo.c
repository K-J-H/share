#include <stdio.h>		//printf
#include <sys/types.h>	//文件IO
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <strings.h>	//bzero

int main()
{
	int fd_lcd = open("/dev/fb0",O_RDWR);
	if(-1 == fd_lcd)
	{
		printf("open lcd failed !\n");
		return -1;
	}

	int color[480][800];
	int m, n;
	for(m=0;m<480/3*1;m++)	//行
	{
		for(n=0;n<800;n++)	//列
		{
			color[m][n] = 0x00000000;	//黑色
		}
	}

	for(;m<480/3*2;m++)	//行
	{
		for(n=0;n<800;n++)	//列
		{
			color[m][n] = 0x00FF0000;	//红色
		}
	}
	
	for(;m<480/3*3;m++)	//行
	{
		for(n=0;n<800;n++)	//列
		{
			color[m][n] = 0x00FFFF00;	//黄色
		}
	}
	
	int *color_pointer = color;
	//void *buff
	int ret = write(fd_lcd,color_pointer,sizeof(color));
	if(-1 == ret)
	{
		printf("write color failed !\n");
		return -1;
	}
	
	close(fd_lcd);
}