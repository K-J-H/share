
#include "../common/getpoint.h"
#include "../common/draw_bmp.h"
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>	//线程
#include <sys/types.h>	//文件IO
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include <sys/mman.h>	//mmap
#include <linux/input.h>	//输入子系统


#define CAMERA_LOW_X 80
#define CAMERA_HIGH_X 320
#define CAMERA_LOW_Y 50
#define CAMERA_HIGH_Y 230
 
#define MUSIC_LOW_X 460
#define MUSIC_HIGH_X 700
#define MUSIC_LOW_Y 50
#define MUSIC_HIGH_Y 230

#define VEDIO_LOW_X 80
#define VEDIO_HIGH_X 320
#define VEDIO_LOW_Y 260
#define VEDIO_HIGH_Y 460

#define PHOTO_LOW_X 460
#define PHOTO_HIGH_X 700
#define PHOTO_LOW_Y 260
#define PHOTO_HIGH_Y 460

#define RETURN_LOW_X 730
#define RETURN_HIGH_X 800
#define RETURN_LOW_Y 0
#define RETURN_HIGH_Y 60




int main(){
    // draw_bmp(path);//背景
    int x, y;//获取点击坐标
    open_tsfile();
    while(1){
    draw_bmp_offset("/kjh/PHOTO/PICTURES_SYSTEM/home.bmp",0, 0);
    get_xy(&x, &y);
    if(x>CAMERA_LOW_X && x<CAMERA_HIGH_X && y>CAMERA_LOW_Y && y<CAMERA_HIGH_Y ){
        printf("camera\n");
    }
    if(x>MUSIC_LOW_X && x<MUSIC_HIGH_X && y>MUSIC_LOW_Y  && y<MUSIC_HIGH_Y ){
        printf("music\n");
		music();
    }
    if(x>PHOTO_LOW_X && x<PHOTO_HIGH_X && y>PHOTO_LOW_Y && y<PHOTO_HIGH_Y ){
        photo();
    }
    if(x>VEDIO_LOW_X && x<VEDIO_HIGH_X && y>VEDIO_LOW_Y && y<VEDIO_HIGH_Y ){
        printf("vedio\n");
    }
    if(x>RETURN_LOW_X && x < RETURN_HIGH_X && y>RETURN_LOW_Y&& y<RETURN_HIGH_Y){
        return 0;
    }
    }
    close_tsfile();

}

/**
 * 相册展示：
 * 首先有个地方存放图片
 * 点击photo后，显示第一张图片
 * 等待用户点击，获取坐标get_xy()
 * if(左边){
 *  present_bmp_path = bmp_path->next
 * }else if右边）{
 *  present_bmp_path = bmp_path->forword
 * }else{
 *  推出循环，返回上一层
 * }
 * show(present_bmp_path, 专场效果);
 * 继续等待用户点击
*/
int filter(const struct dirent *file_info){
    int length = 0;
    while(file_info->d_name[length]){
        length ++;
    }
    char* last_four_character = file_info->d_name + length - 4;
    if(strcmp(last_four_character,".bmp") == 0){
        return 1;
    }
    return 0;
}
int photo(){
    struct dirent **namelist;
    int n;
    n = scandir("/kjh/PHOTO/PICTURES/", &namelist, filter, alphasort);
    if (n < 0)
        perror("scandir");
    // else {
    //     while (n--) {
        
    //         printf("%d:%s\n", n,namelist[n]->d_name);
    //         free(namelist[n]);
    //     }
    //     free(namelist);
    // }
    int count = 0;
    char path_picture[256] ;
    int pointer_picture;
    int x, y;
    char* path_return = "/kjh/PHOTO/PICTURES_SYSTEM/return.bmp";

    do{
        pointer_picture = (int)fabs(count) % n;
        sprintf(path_picture,"/kjh/PHOTO/PICTURES/%s",namelist[pointer_picture]->d_name);
        draw_bmp_offset(path_picture,0,0);
        draw_bmp_offset(path_return,0,0);
        get_xy(&x,&y);
        if(x>400){
            count++;
        }else if(x>60 && x < 400 || x<60 && y > 45){
            count--;
        }else{
            return 0;
    }
    } while(1) ;


}



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

int myfilter_mp3(const struct dirent *filename) 
{
    size_t len;
    len = strlen(filename->d_name);  //strlen（）获取字符串长度
    if (len >= 4
        && filename->d_name[len - 4] == '.'
        && filename->d_name[len - 3] == 'm'
		&& filename->d_name[len - 2] == 'p'
        && filename->d_name[len - 1] == '3')
        return 1;
 
    return 0;
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
			break;
		}
		
	}
}

int get_relative(int *x,int *y)
{
	struct input_event ts;
	
	while(1)
	{
		bzero(&ts,sizeof(ts));
		read(fd_ts,&ts,sizeof(ts));
		printf("type=%d,code=%d,value=%d\n",ts.type,ts.code,ts.value);
	
		if(ts.type==EV_REL&&ts.code==ABS_X)
		{
			*x=ts.value;
		}
		
		if(ts.type==EV_REL&&ts.code==ABS_Y)
		{
			*y=ts.value;
			break;
		}
		
	
	}
}
      int flag = -1;
      int i =0;
      void *func(void* arg)
      {
		   int pid = *(int*)arg;
		  if(pid = 1)
		  {
			  if(flag=-1)
			  {
				  system("mplayer /music/deskmate.mp3");
				  draw_bmp_offset("/zjf/music/pause.bmp",300, 280);
			  }
			  else
			  {
				  system("pause");
				  draw_bmp_offset("/zjf/music/start.bmp",300, 280);
			  }
			  flag = -1 * flag;
			  
		  }
		  if(pid = 2)
		  {
			  i++;
			  if(i>2)
			  {
				  i = 0;
			  }
			  system("killall -9 mplayer");
			  system("mplayer /music/alive.mp3");
			  
			  
		  }
		  if(pid = 3)
		  {
			  i--;
			  if(i<0)
			  {
				  i = 2;
			  }
			  system("killall -9 mplayer");
			  system("mplayer /music/seven.mp3");
			  			  
		  }

	
      }
	
	int music(){
	draw_bmp_offset("/zjf/music.bmp",0, 0);
	ts_init();
	int x,y;
    do{
		get_x_y(&x,&y);
		if(x>0 && x<80&& y>0 && y<60)
		{
			printf("return");
			draw_bmp_offset("/kjh/PHOTO/PICTURES_SYSTEM/home.bmp",0, 0);
			main();
		}
		
		if(x>300 && x<500&& y>280 && y<480)
		{
			pthread_t pid = 1;
			int a = 1;
            pthread_create(&pid,NULL,func,&a);
		}
		
		if(x>500 && x<700&& y>280 && y<480)
		{
			pthread_t pid = 2;
			int a = 2;
            pthread_create(&pid,NULL,func,&a);
		}
		
		if(x>100 && x<300&& y>280 && y<480)
		{
			pthread_t pid = 3;
			int a = 3;
            pthread_create(&pid,NULL,func,&a);
		}
		
	}while(1);  		

	}
