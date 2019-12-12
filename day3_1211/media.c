
#include "../common/getpoint.h"
#include "../common/draw_bmp.h"
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
        photo();
    }
    if(x>VEDIO_LOW_X && x<VEDIO_HIGH_X && y>VEDIO_LOW_Y && y<VEDIO_HIGH_Y ){
        printf("vedio\n");
    }
    if(x>RETURN_LOW_X && x < RETURN_HIGH_X && y>RETURN_LOW_Y&& y<RETURN_HIGH_Y){
        printf("return\n");
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
 label1:   get_xy(&x,&y);
    if(x<480){
        count++;
    }else{
        count--;
    }
    pointer_picture = (int)fabs(count) % n;
    sprintf(path_picture,"/kjh/PHOTO/PICTURES/%s",namelist[pointer_picture]->d_name);
    // printf("pointer: %d\t%s\n",pointer_picture,path_picture);
    draw_bmp_offset(path_picture,0,0);
    goto label1;
    // while(pointer_picture < 10){
    //         sprintf(path_picture,"/kjh/PHOTO/PICTURES/%s",namelist[pointer_picture%n]->d_name);
    //         // strcat(path_picture,namelist[pointer_photo%n]->d_name);
    //         printf("%s\n",path_picture);
    //         pointer_photo++;
    // }

}
