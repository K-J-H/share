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

    int* lcd_pointer = 
    mmap(NULL,ROW_PIXEL*COLUMN_PIXEL*sizeof(int), 
        PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);
    
    if(lcd_pointer == NULL)
        printf("mapping fail!\n");


    //move a line
    int i = 0;
    while(i < COLUMN_PIXEL){
        int j = ROW_PIXEL;
        while(j > 0){
            int temp = (ROW_PIXEL*COLUMN_PIXEL);
            //initializ
            int temp1=0;
            while(temp1 < ROW_PIXEL){
                int temp2 = 0;
                while(temp2 < COLUMN_PIXEL){
                    lcd_pointer[temp1*ROW_PIXEL+temp2] = 0x00000000;
                    temp2++;
                }
                temp1++;
            }
            lcd_pointer[(j*j+i+i*j)%temp] = 0x00f0f0f0;
            lcd_pointer[(j*j+i+i*j+1)%temp] = 0x00f0f0f0;
            lcd_pointer[(j*j+i+i*j+ROW_PIXEL)%temp] = 0x00f0f0f0;
            lcd_pointer[(j*j+i+i*j+ROW_PIXEL+1)%temp] = 0x00f0f0f0;
            j--;
            //there should be nop...
        }
        i ++;
    }

    if(munmap(lcd_pointer,ROW_PIXEL*COLUMN_PIXEL*sizeof(int)) == -1)
        printf("unmap fail\n");
    
    close(fd);
}

