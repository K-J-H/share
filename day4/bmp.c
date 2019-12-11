#include <stdio.h>		//printf
#include <sys/types.h>	//文件IO
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <strings.h>	//bzero
#include <sys/mman.h>	//mmap

#include<input.h>

#define ROW 480
#define COLUMN 800
/**
 * this function is paint the bmp picture.
 * path: the location of bitmap path, if not specify, it will draw a picture i like.
 * offset_hori: the horizontal offset
 * offset_ver: the vertical offset
 * mode: to symatic to x(y) aixs, or xuanzhuan 
 * the lcd screen like this:
 * suppose lcd has ROW COLUMN
 * +------+------+------+---   -----------------+
 * |(0, 0)|(0, 1)|(0, 2)|   ***  (0, COLUMN - 1)|
 * +------+------+------+---   -----------------+
 * |(1, 0)|(1, 1)|  ***          (1, COLUMN - 1)|              |
 * +------+------+------+---   -----------------+
 * |      |      |      |                       |
 * +------+------+------+---   -----------------+
 * |  row i column j is: (i-1, j-1)             
 * +------+------+------+---   -----------------+
 * 
 * or it can also like this:
 * +------+------+------+---   -----------------+
 * |(0, 0)|(1, 0)|(2, 0)|   ***  (COLUMN - 1, 0)|
 * +------+------+------+---   -----------------+
 * |(0, 1)|(1, 1)|(2, 1)|        (COLUMN - 1, 1)|              |
 * +------+------+------+---   -----------------+
 * |      |      |      |                       |   
 * +------+------+------+---   -----------------+
 * |  row i column j is: (j-1, i-1)                  
 * +------+------+------+---   -----------------+
 * 
 * i think the first one is easier to understand
 * just buff[ROW][COLUMN] not in x,y axies
 * 
 * now back to the function. if the bitmap is bigger than the screen size,
 * should i need a buff to hold the entire bitmap?
*/
int bmp(char* path, int offset_hori, int offset_ver, int mode){

    char* screen_path = "/dev/fb0";
    int buff_lcd[ROW][COLUMN];
    int fd_screen = open(screen_path, O_RDWR);
    if(fd_screen == -1){
        printf("open %s fail",screen_path);
        return -1;
    }
    int* fd_screen_pointer 
         = mmap(NULL, 
                sizeof(buff_lcd), 
                PROT_READ | PROT_WRITE,
                MAP_SHARED,
                fd_screen,
                0);
    if(fd_screen_pointer == MAP_FAILED){
        printf("mapping to file %s fail.\n", screen_path);
        return -2;
    }

    //next is to read bitmap file
    int fd_bmp = open(path, O_RDWR);
    if(fd_bmp == -1){
        printf("open %s fail.", path);
        return -1;
    }
    char header_bmp[54];
    read(fd_bmp, header_bmp, sizeof(header_bmp));
    //high:header_bmp[18-21] ; length: [22-25]
    int length_bmp =    header_bmp[18] 
                 |  header_bmp[19] << 8
                 |  header_bmp[20] << 16
                 |  header_bmp[21] << 24;
    int high_bmp =      header_bmp[22] 
                 |  header_bmp[23] << 8
                 |  header_bmp[24] << 16
                 |  header_bmp[25] << 24;     
    printf("length:%d\t high:%d\t",length,high);

    char buff_bmp[high_bmp][length_bmp][3];
    if(length_bmp % 4){
        //不能被4整除
        int buqi = 4 - length_bmp % 4;
        int i,j;
        for(i = 0; i < high_bmp; i++){
            for(j = 0; j < length_bmp; j++){
                read(fd_bmp,buff_bmp[i][j],3);
                lseek(fd_bmp,buqi,SEEK_CUR);
            }
        }
    }else{
        //没有冗余
    }

    int 
    for()
    //char to int

    

    if(munmap(fd_screen_pointer,sizeof(buff_lcd)) -1){
        printf("unmapping file %s fail.",path);
        return -3;
    }
    close(fd_screen);
}

int main(int argc, char* argv){
    char* path_bmp;
    path_bmp = (argc == 1) ? "/kjh/BitMap/yuan.bmp" : argv[1] ;
    bmp(path_bmp,0,0,0);
}
