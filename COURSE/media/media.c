
#include "../../common/getpoint.h"
#include "../../common/draw_bmp.h"
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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


extern int camera();

int main(){
    // draw_bmp(path);//背景
    int x, y;//获取点击坐标
    open_tsfile_();
    open_lcd();
    while(1){
    draw_bmp_offset("/kjh/PHOTO/PICTURES_SYSTEM/home.bmp",0, 0);
    get_xy(&x, &y);
    if(x>CAMERA_LOW_X && x<CAMERA_HIGH_X && y>CAMERA_LOW_Y && y<CAMERA_HIGH_Y ){
        camera();
    }
    else if(x>MUSIC_LOW_X && x<MUSIC_HIGH_X && y>MUSIC_LOW_Y  && y<MUSIC_HIGH_Y ){
        printf("music\n");
    }
    else if(x>PHOTO_LOW_X && x<PHOTO_HIGH_X && y>PHOTO_LOW_Y && y<PHOTO_HIGH_Y ){
        photo();
    }
    else if(x>VEDIO_LOW_X && x<VEDIO_HIGH_X && y>VEDIO_LOW_Y && y<VEDIO_HIGH_Y ){
        printf("vedio\n");
    }
    else if(x>RETURN_LOW_X && x < RETURN_HIGH_X && y>RETURN_LOW_Y&& y<RETURN_HIGH_Y){
        return 0;
    }
    }
    close_tsfile();
    close_lcd();

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
/**
 * 以上是粗略的相册模块，有很多不足：
 * 第一，draw_bmp函数封装了打开和关闭的过程，每画一张照片都要打开和关闭资源。
 *      改进方法：打开和关闭资源不要封装在一个函数，封装应该是：打开，处理，关闭
 * 第二，没有实现转场效果，所以函数要再加个功能draw_bmp(path, 转场)，当然，这个也只是
 *      如何显示一张图片。还可以连续画多帧实现动态效果。
 *      此外，还应该有颜色变化。初步想法是初始态到终止态步步增长（多帧显示)
 * 第三，如果可以，还可以加上音效
*/
