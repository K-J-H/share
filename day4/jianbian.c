#include <stdio.h>		//printf
#include <sys/types.h>	//文件IO
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <strings.h>	//bzero
#include <sys/mman.h>	//mmap

#define ROW 480
#define COLUMN 800

int main(){
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

    //希望有以渐变颜色，左上角0，右下角最大，2的8次方

}