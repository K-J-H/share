#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <unistd.h>
#include <linux/input.h>
#include <pthread.h>
#include <dirent.h>
#include "../../common/getpoint.h"
#include "../../common/draw_bmp.h"

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
extern void close_lcd_(void);
extern int lcd_draw_jpg(unsigned int x,unsigned int y,const char *pjpg_path,char *pjpg_buf,unsigned int jpg_buf_size,unsigned int jpg_half);

extern int get_picture();

int flag_cam = 1;

void *func(void *arg)
{
	get_picture();
}

int flag_save_photo = 0;

int filter(const struct dirent *file_info){
    int length = 0;
    while(file_info->d_name[length]){
        length ++;
    }
    char* last_four_character = file_info->d_name + length - 4;
    if(strcmp(last_four_character,".jpg") == 0){
        return 1;
    }
    return 0;
}
int camera_photos(){
	    struct dirent **namelist;
    int n;
    n = scandir("/kjh/CAMERA/PICTURES/", &namelist, filter, alphasort);
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
        sprintf(path_picture,"/kjh/CAMERA/PICTURES/%s",namelist[pointer_picture]->d_name);
		lcd_draw_jpg(0,0,path_picture,NULL,0,0);
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

int camera()
{
	int getpoint_x, getpoint_y;
	open_tsfile();
	
	draw_bmp_offset("/kjh/CAMERA/RESOURCES/camera_toolbar.bmp",640,0);

	lcd_open("/dev/fb0");
	pthread_t pid;
	//开启摄像头
	pthread_create(&pid,NULL,func,NULL);
	while(1){
		get_xy(&getpoint_x,&getpoint_y);
		if(getpoint_x > 640){
			if(getpoint_y < 160){
				printf("main:return\n");
				flag_cam = 0;
				return 0;
			}else if(getpoint_y >= 320){
				pthread_cancel(pid);
				camera_photos();
				pthread_create(&pid,NULL,func,NULL);
			}else{
				flag_save_photo = 1;
			}
		}
	}
	//关闭摄像头
	//获取触摸屏的坐标点，直接设置flag_cam = 0;
	scanf("%d",&flag_cam);
	sleep(2);
	
	close_lcd_();
	close_tsfile();
	return 0;
}


int main(){
	camera();
}