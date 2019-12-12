#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <unistd.h>
#include <linux/input.h>
#include <pthread.h>
/*
extern int lcd_open(const char *str);
extern void close_lcd(void);
extern int lcd_draw_jpg(unsigned int x,unsigned int y,const char *pjpg_path,char *pjpg_buf,unsigned int jpg_buf_size,unsigned int jpg_half);

int main()
{
	lcd_open("/dev/fb0");
	
	lcd_draw_jpg(0,0,"1.jpg",NULL,0,0);
	
	close_lcd();
}
*/





extern int lcd_open(const char *str);
extern void close_lcd(void);
extern int lcd_draw_jpg(unsigned int x,unsigned int y,const char *pjpg_path,char *pjpg_buf,unsigned int jpg_buf_size,unsigned int jpg_half);

extern int get_picture();

int flag_cam = 1;

void *func(void *arg)
{
	get_picture();
}

int main()
{
	lcd_open("/dev/fb0");
	
	// pthread_t pid;
	// //开启摄像头
	// pthread_create(&pid,NULL,func,NULL);
	
	// //关闭摄像头
	// //获取触摸屏的坐标点，直接设置flag_cam = 0;
	// scanf("%d",&flag_cam);
	// sleep(2);
	lcd_draw_jpg(0,0,"/kjh/CAMERA/PICTURES/20361226091756.jpg" ,NULL,0,0);
	
	close_lcd();
	
	return 0;
}
