#include <stdio.h>		//printf
#include <sys/types.h>	//文件IO
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <strings.h>	//bzero

/**
 * print a rectangle point(x,y)
 * width length
*/
void draw(int color[][480],int x, int y, int length, int width, int value);
int main()
{   
    int color[800][480];
    
	int fd_lcd = open("/dev/fb0",O_RDWR);
	if(-1 == fd_lcd)
	{
		printf("open lcd failed !\n");
		return -1;
	}

    draw(color,200,100,200,100,0x000000ff);

	int *color_pointer = color;
	//void *buff
	int ret = write(fd_lcd,color_pointer,sizeof(color));
	if(-1 == ret)
	{
		printf("write color failed !\n");
		return -1;
	}
	
	close(fd_lcd);
}

void draw(int color[800][480],int x, int y, int length, int width, int value){
    int i=0;
    int j = 0;
    for(i;i < width; i++){ 
        for(j=0; j<length ; j++){
            color[x+i][y+j] = value;
            printf("color[%d][%d] = %d\t",x+i,y+j,value);
        }
        putchar('\n');
    }
    return;
}