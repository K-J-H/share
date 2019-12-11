/**
 * 写了读取bmp文件信息，于写入lcd后，现在结合两者功能：
 * 在lcd上显示bmp文件。那么可以怎么写入呢？
 * 位置变化的有：
 *      1.偏移
 *      2.上下左右翻转
 *      3.旋转
 *      4.放大缩小
 * 连续起来的位置变化：
 *      
 * 颜色变换：
 * 
 * 出场方式：
 *      1.百叶窗
 * 
*/
#include "BitMap.h"
#include "lcd.h"
#include <stdio.h>
#include <math.h>

#define MAX_BMP_SIZE 1024

int draw_bmp_offset(char* path_bmp,int off_x, int off_y){
    int fd_bmp = open_bitmap(path_bmp);
    if(fd_bmp == -1){
        printf("open fd_bmp:%s fail.\n",path_bmp);
        return -1;
    }
    Info_bmp info_bmp;
    read_bmpinfo(fd_bmp,&info_bmp);
    int length = info_bmp.length;
    int high = info_bmp.high;
    if(length> MAX_BMP_SIZE || high > MAX_BMP_SIZE){
        printf("bmp over size! should not be larger than %d",MAX_BMP_SIZE);
        return -2;
    }
    if(off_x>PIXELS_COLUMN || off_x < (-length) || off_y > PIXELS_ROW || off_y < (-high)){
        printf("offset over size!");
        return -3;
    }
    int buff[length*high];
    read_bmp(fd_bmp,buff,length,high);
    //以上把bmp图片信息读入buff中

    //接下来是将buff写入lcd屏幕中
    open_lcd();
    int i,j;
    // printf("off_x:%d\toff_y%d\t\n",off_x,off_y);
    for(i= (off_x<0)? -off_x:0; i<length && (off_x+i)<PIXELS_COLUMN; i++){
        for(j = (off_y<0)?-off_y:0; j < high&& (off_y+j)<PIXELS_ROW; j++){

            draw_point(off_x+i,off_y+j,buff[j*length+i]);
            // printf("(%d, %d): (%d, %d)\t",off_x+i,off_y+j,i,j);
        }
    }

    close_bitmap(fd_bmp);
    close_lcd();
}

int draw_bmp_symmetry(char *path_bmp, int mode){
    int fd_bmp = open_bitmap(path_bmp);
    if(fd_bmp == -1){
        printf("open fd_bmp:%s fail.\n",path_bmp);
        return -1;
    }
    Info_bmp info_bmp;
    read_bmpinfo(fd_bmp,&info_bmp);
    int length = info_bmp.length;
    int high = info_bmp.high;
    if(length> MAX_BMP_SIZE || high > MAX_BMP_SIZE){
        printf("bmp over size! should not be larger than %d",MAX_BMP_SIZE);
        return -2;
    }

    int buff[length*high];
    read_bmp(fd_bmp,buff,length,high);
    //以上把bmp图片信息读入buff中

    //接下来是将buff写入lcd屏幕中
    open_lcd();
    int i,j;
    /*如果是左右对称，（X.new, Y.new) = (*/

    close_bitmap(fd_bmp);
    close_lcd();
}

/**
 * 以x0, y0为旋转中心向上旋转jiaodu
*/
int draw_bmp_rotation(char* path_bmp, int x0, int y0, int jiaodu){
    int fd_bmp = open_bitmap(path_bmp);
    if(fd_bmp == -1){
        printf("open fd_bmp:%s fail.\n",path_bmp);
        return -1;
    }
    Info_bmp info_bmp;
    read_bmpinfo(fd_bmp,&info_bmp);
    int length = info_bmp.length;
    int high = info_bmp.high;
    if(length> MAX_BMP_SIZE || high > MAX_BMP_SIZE){
        printf("bmp over size! should not be larger than %d",MAX_BMP_SIZE);
        return -2;
    }

    int buff[length*high];
    read_bmp(fd_bmp,buff,length,high);
    //以上把bmp图片信息读入buff中

    //接下来是将buff写入lcd屏幕中
    const double pi = 3.141592653589793238;
    open_lcd();
    double hudu = ( (jiaodu%360) * pi )/180.0;
    double sin_hudu = sin(hudu);
    double cos_hudu = cos(hudu);
    int i,j;
    int Xnew, Ynew;
    for(i = 0; i < length; i++){
        for(j = 0; j < high; j++){
            Xnew = (i-x0)*cos_hudu - (j-y0)*sin_hudu + x0;
            Ynew = (j-y0)*cos_hudu + (i-x0)*sin_hudu + y0;
            if(Xnew>0 && Xnew < PIXELS_COLUMN && Ynew > 0 && Ynew < PIXELS_ROW){
                draw_point(Xnew, Ynew, buff[i+j*length]);
            }
        }
    }
    close_bitmap(fd_bmp);
    close_lcd();
}

int draw_color(int color){
    open_lcd();
    int i, j;
    for(i = 0; i < PIXELS_COLUMN; i++){
        for(j = 0; j < PIXELS_ROW; j++){
            draw_point(i,j,color);
        }
    }
    close_lcd();
    return 0;
}






