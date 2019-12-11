#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>

#define PIXELS_COLUMN 800
#define PIXELS_ROW 480

int fd_lcd;
int *mmap_lcd;

int open_lcd(){
    fd_lcd = open("/dev/fb0",O_RDWR);
    if(fd_lcd == -1){
        printf("open file fail!");
        return -1;
    }
    mmap_lcd = (int *)mmap(NULL,
					       PIXELS_COLUMN*PIXELS_ROW*sizeof(int),
					       PROT_READ|PROT_WRITE,
					       MAP_SHARED,
					       fd_lcd,
					       0);
	if(mmap_lcd == MAP_FAILED)
	{
		printf("mmap lcd failed !\n");
		return -1;
	}
}

int close_lcd(){
    munmap(mmap_lcd,PIXELS_COLUMN*PIXELS_ROW*sizeof(int));
    close(fd_lcd);
}

int draw_point(int x, int y, int color){
    *(mmap_lcd + y*PIXELS_COLUMN + x) = color;
}

int color_at_point(int x, int y){
    return *(mmap_lcd+y*PIXELS_COLUMN+x);
}


