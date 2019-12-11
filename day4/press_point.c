#include <stdio.h>		//printf
#include <sys/types.h>	//文件IO
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <strings.h>	//bzero
#include <sys/mman.h>	//mmap
#include <linux/input.h>	//输入子系统

int fd_ts;

typedef struct Point_
{
    int x;
    int y;
} Point;

Point press_point()
{
	fd_ts = open("/dev/input/event0",O_RDWR);
	if(-1 == fd_ts)
	{   
        Point temp = {-1,-1};
		printf("open ts failed !\n");
		return temp;
	}
	
	struct input_event ts;
    Point point;
	
    int x, y;
    int flag_x = 0;
    int flag_y = 0;
	while(1)
	{   
		bzero(&ts,sizeof(ts));
		read(fd_ts,&ts,sizeof(ts));
		// printf("type=%d,code=%d,value=%d\n",ts.type,ts.code,ts.value);
        //printf("--------begin------------\n");

		if(ts.type==EV_ABS&&ts.code==ABS_X)
		{
			x=ts.value;
            flag_x = 1;
		}
        //printf("flag_x:%d;\t",flag_x);
		
		if(ts.type==EV_ABS&&ts.code==ABS_Y)
		{
			y=ts.value;
            flag_y = 1;
		}
        //printf("flag_y:%d\n",flag_y);
		
		if(ts.type==EV_ABS&&ts.code==ABS_PRESSURE&&ts.value==0 && flag_x ==1 && flag_y == 1)
		{
            flag_x = 0;
            flag_y = 0;
			break;
		}
		else if(ts.type==EV_KEY&&ts.code==BTN_TOUCH&&ts.value==0 && flag_x ==1 && flag_y == 1)
		{
            flag_x = 0;
            flag_y = 0;
			break;
		}
        
	}	
	
    printf("(%d, %d)\t",x,y);

    point.x = x;
    point.y = y;
	
	close(fd_ts);

    return point;
}

int main(){
    Point point;
    while(1){
        point = press_point();
        printf("POINT:(%d, %d)\n",point.x,point.y);
    }
}