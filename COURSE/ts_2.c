#include <stdio.h>		//printf
#include <sys/types.h>	//文件IO
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <strings.h>	//bzero
#include <sys/mman.h>	//mmap
#include <linux/input.h>	//输入子系统

int fd_ts;

int ts_init()
{
	fd_ts = open("/dev/input/event0",O_RDWR);
	if(-1 == fd_ts)
	{
		printf("open ts failed !\n");
		return -1;
	}
}

int ts_uinit()
{
	close(fd_ts);
}

int get_x_y(int *x,int *y)
{
	struct input_event ts;
	
	while(1)
	{
		bzero(&ts,sizeof(ts));
		read(fd_ts,&ts,sizeof(ts));
		printf("type=%d,code=%d,value=%d\n",ts.type,ts.code,ts.value);
	
		if(ts.type==EV_ABS&&ts.code==ABS_X)
		{
			*x=ts.value;
		}
		
		if(ts.type==EV_ABS&&ts.code==ABS_Y)
		{
			*y=ts.value;
		}
		
		if(ts.type==EV_ABS&&ts.code==ABS_PRESSURE&&ts.value==0)
		{
			break;
		}
		else if(ts.type==EV_KEY&&ts.code==BTN_TOUCH&&ts.value==0)
		{
			break;
		}
	}
}

int main()
{	
	int x,y;
	
	ts_init();
	
	get_x_y(&x,&y);
	printf("(%d,%d)\n",x,y);
	
	if(x>0 && x<400 && y>0 && y<240)
	{
		printf("hello ~~\n");
	}
	
	ts_uinit();
}