#include <stdio.h>		//printf
#include <sys/types.h>	//文件IO
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <strings.h>	//bzero
#include <sys/mman.h>

#define ROW_PIXEL 800
#define COLUMN_PIXEL 480

/**
 * void *mmap(void *addr, size_t length, int prot, int flags,
                  int fd, off_t offset);

*/
int main(){
    int fd = open("/dev/fb0",O_RDWR);
    if(fd == -1)
        printf("open fail!\n");
    
    //draw rectangle (x, y) length:  width
    // int color = 0x00ff00f0;
    // int m = 0, n = 0;
    // for(;m<200;m++){
    //     for(n = 0;n< 400; n++){
    //         lseek(fd,((m+100)*ROW_PIXEL+n+200)*sizeof(int),SEEK_SET);
    //         write(fd,&color,sizeof(color));
    //     }
    // }

    int* lcd_pointer = 
    mmap(NULL,ROW_PIXEL*COLUMN_PIXEL*sizeof(int), 
        PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);
    
    if(lcd_pointer == NULL)
        printf("mapping fail!\n");

    int i = 0;
    while(i++ < COLUMN_PIXEL){
        int j = 0;
        while(j++ < ROW_PIXEL)
            *(lcd_pointer+i*ROW_PIXEL+j) = (i*i+j*j+i+j)%0x00ffffff;
    }
    if(munmap(lcd_pointer,ROW_PIXEL*COLUMN_PIXEL*sizeof(int)) == -1)
        printf("unmap fail\n");
    
    close(fd);
}