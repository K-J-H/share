#include "../common/BitMap.h"
#include "../common/lcd.h"
#include <stdio.h>
#include <math.h>
#include <unistd.h>

#define MAX_BMP_SIZE 1024
/**
 * 以x0, y0向上旋转rotation（弧度）
*/
int draw_bmp_rotation(char* path_bmp, int jiaodu){
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
            Xnew = i*cos_hudu - j*sin_hudu;
            Ynew = j*cos_hudu + i*sin_hudu;
            if(Xnew>0 && Xnew < PIXELS_COLUMN && Ynew > 0 && Ynew < PIXELS_ROW){
                draw_point(Xnew, Ynew, buff[i+j*length]);
            }
        }
    }
    close_bitmap(fd_bmp);
    close_lcd();
}

int main(int argc, char* argv[]){
    int i;
    for(i = 0; i < 90; i++){
        draw_bmp_rotation(argv[1],i);
        usleep(atoi(argv[2]));
        
    }
}