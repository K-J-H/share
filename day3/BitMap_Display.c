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
    //open the lcd file
    int fd = open("/dev/fb0",O_RDWR);
    if(fd == -1)
        printf("open fail!\n");

    int* lcd_pointer = 
    mmap(NULL,ROW_PIXEL*COLUMN_PIXEL*sizeof(int), 
        PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);
    
    if(lcd_pointer == NULL)
        printf("lcd mapping fail!\n");

    //open bitmap file
    int fd_bm = open("/kjh/BitMap/jhsb.bmp",O_RDWR);
    if(fd_bm == -1)
        printf("open fail!\n");

    char* bm_pointer = 
    mmap(NULL,ROW_PIXEL*COLUMN_PIXEL*3, 
        PROT_READ|PROT_WRITE,MAP_SHARED,fd_bm,0);
    
    if(lcd_pointer == NULL)
        printf("bitmap mapping fail!\n");

    int i = 0;
    for(i; i < COLUMN_PIXEL; i++){
        int j = 0;
        for(j; j < ROW_PIXEL; j++){
            //pixle(i, j)
            *(lcd_pointer + i*ROW_PIXEL + j) = 
            (unsigned int)(*(bm_pointer+3*(i*ROW_PIXEL + j) +0)) +
            ((unsigned int)(*(bm_pointer+3*(i*ROW_PIXEL + j) + 1)) ) << 8 +
            ((unsigned int)(*(bm_pointer+3*(i*ROW_PIXEL + j) +2)) ) << 16 ;
        }
        
    }

    
    if(munmap(bm_pointer,ROW_PIXEL*COLUMN_PIXEL*3) == -1)
        printf("bm unmap fail\n");

    if(munmap(lcd_pointer,ROW_PIXEL*COLUMN_PIXEL*sizeof(int)) == -1)
        printf("lcd unmap fail\n");
    
    close(fd);
}