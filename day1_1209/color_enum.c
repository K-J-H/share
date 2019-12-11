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

int fd_lcd;
int *mmap_lcd;

int init(){
    fd_lcd = open("/dev/fb0",O_RDWR);
	if(-1 == fd_lcd)
	{
		printf("open lcd failed !\n");
		return -1;
	}
	
	mmap_lcd = (int *)mmap(NULL,
					       800*480*4,
					       PROT_READ|PROT_WRITE,
					       MAP_SHARED,
					       fd_lcd,
					       0);
	if(mmap_lcd == MAP_FAILED)
	{
		printf("mmap lcd failed !\n");
		return -2;
	}
}

close_lcd(){
    munmap(mmap_lcd,800*480*4);
    close(fd_lcd);
}

void draw_color(int color){
    int i,j;
    for(i = 0; i < COLUMN_PIXEL; i ++){
        for(j = 0; j < ROW_PIXEL; j ++){
            *(mmap_lcd+j*COLUMN_PIXEL+i)= color; 
        }
    }
}

int main(int argc, char* argv[]){
    init();
    int i;
    int begin, end;
    begin=charToint(argv[1]);
    end=charToint(argv[2]);
    if(begin == -1 || end == -1){
        printf("unknow command\n");
    }
    for( i=begin ; i <=end; i++){
        draw_color(i|i<<8|i<<16);
    }
    close_lcd();
}