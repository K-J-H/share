
#include "../common/getpoint.h"
#include "../common/draw_bmp.h"

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
    draw_bmp_offset("/kjh/PHOTO/PICTURES/home.bmp",0, 0);

    int x, y;//获取点击坐标
    open_tsfile();
    get_xy(&x, &y);
    if(x>CAMERA_LOW_X && x<CAMERA_HIGH_X && y>CAMERA_LOW_Y && y<CAMERA_HIGH_Y ){
        printf("camera\n");
    }
    if(x>MUSIC_LOW_X && x<MUSIC_HIGH_X && y>MUSIC_LOW_Y  && y<MUSIC_HIGH_Y ){
        printf("music\n");
    }
    if(x>PHOTO_LOW_X && x<PHOTO_HIGH_X && y>PHOTO_LOW_Y && y<PHOTO_HIGH_Y ){
        printf("photo\n");
    }
    if(x>VEDIO_LOW_X && x<VEDIO_HIGH_X && y>VEDIO_LOW_Y && y<VEDIO_HIGH_Y ){
        printf("vedio\n");
    }
    if(x>RETURN_LOW_X && x < RETURN_HIGH_X && y>RETURN_LOW_Y&& y<RETURN_HIGH_Y){
        printf("return\n");
    }
    close_tsfile();

}
