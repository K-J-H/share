#include <stdio.h>		//printf
#include <sys/types.h>	//文件IO
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <strings.h>	//bzero

/**
 * 矩形：接受参数：左上顶点，长度，宽度
 * (200,100), 200, 100
*/


int main()
{
	int fd_lcd = open("/dev/fb0",O_RDWR);
	if(-1 == fd_lcd)
	{
		printf("open lcd failed !\n");
		return -1;
	}
    int buf[200] = {0x000000ff};
    lseek(fd_lcd,((100-1)*800+200)*sizeof(int),SEEK_SET);
    
    int i = 0;
    while(i++ < 100){
    write(fd_lcd,buf,sizeof(buf));
    lseek(fd_lcd,(800-200)*sizeof(int),SEEK_CUR);
    }


	close(fd_lcd);
}