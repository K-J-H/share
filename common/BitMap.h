/**
 * 该头文件封装以下常用功能：
 * 1. 打开bitmap
 * 2. 获取bitmap信息，应当存放在一个结构体中
 * 3. 把颜色信息写入一个buff中
*/
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <strings.h> //bzero
#include <stdio.h>

/**
 * BMP图片的结构体。
 * 有两个成员：
 * 横向长度length
 * 纵向高high
*/
typedef struct
{
    int length;
    int high;
} Info_bmp;

/**
 * path：bmp图片的位置
 * 返回值：指向bmp图片的fd
*/
int open_bitmap(char* path){
    int fd_bmp = open(path, O_RDWR);
    return fd_bmp;
}

/**
 * information[18-21] 地位到高位：水平
 * 			  [22-25] 低位到高位：竖直
 * 将图片的长宽信息存放到Info_bmp结构体中
*/
int read_bmpinfo(int fd_bmp, Info_bmp* info_bmp){
    char information[54];
	read(fd_bmp,information,sizeof(information));
	info_bmp->length = information[18] | information[19] << 8 | information[20] << 16 | information[21] << 24;
	info_bmp->high = information[22] | information[23] << 8 | information[24] << 16 | information[25] << 24;
	printf("length:%d\thigh:%d\t\n",info_bmp->length,info_bmp->high);
}

/**
 * 接下来是bmp文件，将像素数据存储到一个buff中。
 * 应该要有动态存储空间的。。。但现在还不是很清楚。
 * 在外部定义变量buff[length][width].那么存储空间是怎样的？
 * fd_bmp:要读取的bmp文件标致fd
 * buff：像素信息存放的地方,二维数组不可用...
 * length：high：长和宽
*/
int read_bmp(int fd_bmp, int *buff,int length, int high){
    int m = (length*3) % 4;//查看是否能被4整除
    printf("yushu: %d\n",m);
    if(m == 0){//能被四整除，则直接转换成int
        char buff_unprocess[length*high*3];
        //lseek(fd_bmp,54,SEEK_SET);
        read(fd_bmp,buff_unprocess,sizeof(buff_unprocess));
        int i, j;
        for(i = 0; i < length; i ++){
            for(j = 0; j < high; j ++){
                *(buff+(high - j - 1)*length+i) = buff_unprocess[(j*length + i)*3 ]         | 
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
                *(buff+(high - j - 1)*length+i)  = buff_unprocess[(j*length + i)*3 + buqi*j]          | 
                                                   buff_unprocess[(j*length + i)*3 + 1 + buqi*j] << 8 |
                                                   buff_unprocess[(j*length + i)*3 + 2 + buqi*j] << 16;
            }
        }
    }
    return 0;
}

/**
 * 关闭bmp图片
*/
int close_bitmap(int fd_bmp){
    close(fd_bmp);
}

