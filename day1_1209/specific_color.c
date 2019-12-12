#include <stdio.h>		//printf
#include <sys/types.h>	//文件IO
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <strings.h>	//bzero
#include <sys/mman.h>	//mmap
#include <unistd.h>

#include "Function.h"

#define ROW_PIXEL 480
#define COLUMN_PIXEL 800


int specific_color(int color){
    // unsigned int max_value = 1 << 24;
    // unsigned int inteval = max_value/(ROW_PIXEL+COLUMN_PIXEL);
    int fd_lcd = open("/dev/fb0",O_RDWR);
	if(-1 == fd_lcd)
	{
		printf("open lcd failed !\n");
		return -1;
	}
	
	int *mmap_lcd = (int *)mmap(NULL,
					       800*480*4,
					       PROT_READ|PROT_WRITE,
					       MAP_SHARED,
					       fd_lcd,
					       0);
	if(mmap_lcd == MAP_FAILED)
	{
		printf("mmap lcd failed !\n");
		return -1;
	}
    int i,j;
    for(i = 0; i < COLUMN_PIXEL; i ++){
        for(j = 0; j < ROW_PIXEL; j ++){
            *(mmap_lcd+j*COLUMN_PIXEL+i)= color; 
        }
    }

    close(fd_lcd);
    return 0;
}

/**
 * 为了多次写入照片，所以封装了打开文件
*/



int main(int argc, char* argv[]){
    int color;

    if(argc == 1){
        color = 0x00000000;
    }else if(argc == 2){
        color = charToint(argv[1]);
    }else{
        printf("unknown command!\n");
        return -1;
    }
    printf("%s: %d",argv[1],charToint(argv[1]));
    specific_color(color);
    return 0;
}
