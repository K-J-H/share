#include <stdio.h>		//printf
#include <sys/types.h>	//文件IO
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <strings.h>	//bzero
#include <sys/mman.h>	//mmap

int main(){
    int fd_touch = open("/dev/input/event0",O_RDWR);
    if (fd_touch == -1)
    {
        printf("open touch failed.\n");
        return -1;
    }
    
    int buff[1024];
    read(fd_touch,buff, sizeof(buff));

    close(fd_touch);

    int i ;
    for(i = 0; i < 1024 ; i++)
    printf("%02x ",buff+i);
    
    return 0;
    
}

/**
 * 输入子系统：
 * 
*/