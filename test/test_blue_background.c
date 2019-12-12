#include "../common/BitMap.h"
#include "../common/lcd.h"

read_bmp_blue(int fd_bmp, int *buff,int length, int high){
    int m = (length*3) % 4;//查看是否能被4整除
    printf("yushu: %d\n",m);
    if(m == 0){//能被四整除，则直接转换成int
        char buff_unprocess[length*high*3];
        //lseek(fd_bmp,54,SEEK_SET);
        read(fd_bmp,buff_unprocess,sizeof(buff_unprocess));
        int i, j;
        for(i = 0; i < length; i ++){
            for(j = 0; j < high; j ++){
                *(buff+(high - j - 1)*length+i) = buff_unprocess[(j*length + i)*3]        | 
                                                  buff_unprocess[(j*length + i)*3 + 1] << 8 |
                                                  buff_unprocess[(j*length + i)*3 + 2] << 16;
            }
        }
        
    }else{//不能被四整除，说明每一行都有补齐
        int buqi = 4 - m;//每一行都有buqi个字节冗余
        //在这里直接定义一个数组全盘接受
        char buff_unprocess[(length*3 + buqi)*high];
        read(fd_bmp,buff_unprocess,sizeof(buff_unprocess));
        int i, j;
        for(i = 0; i < length; i++){
            for(j = 0; j < high; j++){
                *(buff+(high - j - 1)*length+i)  = buff_unprocess[(j*length + i)*3  + buqi*j]| 
                                                   buff_unprocess[(j*length + i)*3 + 1 + buqi*j] << 8 |
                                                   buff_unprocess[(j*length + i)*3 + 2 + buqi*j] << 16;
            }
        }
    }
    return 0;
}

int main(int argc, char* argv[]){

    int fd_bmp = open_bitmap(argv[1]);

    Info_bmp info_bmp;

    read_bmpinfo(fd_bmp,&info_bmp);

    int length = info_bmp.length;
    int high = info_bmp.high;
    int buff[length*high];

    read_bmp_blue(fd_bmp,buff,length,high);

    close_bitmap(fd_bmp);

    open_lcd();
    int i, j;
    for(i = 0; i < length; i ++){
        for(j = 0; j < high; j++){
            draw_point(i,j,buff[j*length+i]);
        }
    }
    close_lcd();
}