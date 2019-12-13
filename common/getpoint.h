#ifndef GET_POINT_H
#define GET_POINT_H

#include <stdio.h>		//printf
#include <sys/types.h>	//文件IO
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <strings.h>	//bzero
#include <sys/mman.h>	//mmap
#include <linux/input.h>	//输入子系统

/**
 * 函数封装，例day4的presspoint：
 * 1. 返回结构体Point
 * 2. 传入地址
 * 有什么区别呢？
 * 
 * 文件读写类型封装：
 * 1.文件开启
 * 2.文件处理（又可细分）
 * 3.关闭
*/
int fd_ts;

int open_tsfile();
int get_xy(int *x, int *y);
int close_tsfile();

// int main(){
//   int x, y;
//   open_tsfile();
//   get_xy(&x,&y);
//   printf("(%d, %d)\n",x,y);
//   close_tsfile();
// }

int open_tsfile_(){
    fd_ts = open("/dev/input/event0",O_RDWR);
	if(-1 == fd_ts)
	{   
		printf("open ts failed !\n");
		return -1;
	}
    return fd_ts;
}

int get_xy(int *x, int *y){

    struct input_event ts;

    while(1)
	{   
		bzero(&ts,sizeof(ts));
		read(fd_ts,&ts,sizeof(ts));
		if(ts.type==EV_ABS&&ts.code==ABS_X)
		{
			*x=ts.value;
		}

		// printf("x: %d\t",*x);
		
		if(ts.type==EV_ABS&&ts.code==ABS_Y)
		{
			*y=ts.value;
		}
		// printf("y: %d\t",*y);
		
		if(ts.type==EV_ABS&&ts.code==ABS_PRESSURE&&ts.value==0  )
		{
			break;
		}
		else if(ts.type==EV_KEY&&ts.code==BTN_TOUCH&&ts.value==0 )
		{
			break;
		}
	}
	return 0;
}

int close_tsfile(){
    close(fd_ts);
}

#endif