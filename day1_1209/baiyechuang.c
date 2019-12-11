#include <stdio.h>		//printf
#include <sys/types.h>	//文件IO
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <strings.h>	//bzero
#include <sys/mman.h>	//mmap
#include <unistd.h>

int *mmap_lcd;

//lcd画点
void lcd_draw_point(int x,int y,int color)
{
	*(mmap_lcd+y*800+x) = color;
}

/**
 * 输入：show path x y 
*/
int draw_bmp(char *path, int offset_x, int offset_y, int haomiao)
{
    
	int fd_lcd = open("/dev/fb0",O_RDWR);
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
		return -1;
	}
	
	int fd_bmp = open(path,O_RDWR);
	if(-1 == fd_bmp)
	{
		printf("open %s failed !\n",path);
		return -1;
	}
	
	char information[54];
	read(fd_bmp,information,sizeof(information));
	/**
	 * information[18-21] 地位到高位：宽度
	 * 			  [22-25] 低位大高位：长度
	*/
	int length = information[18] | information[19] << 8 | information[20] << 16 | information[21] << 24;
	int width = information[22] | information[23] << 8 | information[24] << 16 | information[25] << 24;
	printf("width:%d\tlength:%d\t\n",width,length);

	char buf_bmp[length*width*3];
    
    int buqi = 4 - (length*3)%4;
    if((length*3)%4 == 0)
	    {read(fd_bmp,buf_bmp,sizeof(buf_bmp));}
    else{
        int i,j;
        for(i = 0; i < width; i++){
                read(fd_bmp,buf_bmp+i*length*3,length*3);
                lseek(fd_bmp,buqi,SEEK_CUR);
        }
    }	
    /**
     *假设现在buf_amp已经读取了对应的像素点 
     *而要在（x, y)
    */

	int buf_lcd[width*length];
	int i;
	/*
	//ARGB			B			G			R
	buf_lcd[0] = buf_bmp[0] buf_bmp[1] buf_bmp[2];
	buf_lcd[1] = buf_bmp[3] buf_bmp[4] buf_bmp[5];
	buf_lcd[2] = buf_bmp[6] buf_bmp[7] buf_bmp[8];
	//				B				G					R				A
	buf_lcd[i] = buf_bmp[3*i] | buf_bmp[3*i+1]<<8 | buf_bmp[3*i+2]<<16 | 0x00<<24;
	*/
	for(i=0;i<width*length;i++)
	{
		buf_lcd[i] = buf_bmp[3*i] | buf_bmp[3*i+1]<<8 | buf_bmp[3*i+2]<<16 | 0x00<<24;
	}
	
    int buff[width][length];
    int temp0, temp1;
    for(temp0 = 0; temp0 < width; temp0 ++){
        for(temp1 = 0; temp1 < length; temp1 ++){
            buff[temp0][temp1] = buf_lcd[(width-1-temp0)*length+temp1];
        }
    }

	// int x,y;
	// for(x = 0; x < length && x + offset_x < 800; x++){
    //     for( y = 0; y<width && y + offset_y < 480; y++){
    //         lcd_draw_point(x+offset_x, y + offset_y, buff[y][x]);
    //     }
    // }
    int a1,a2,a3;
    // for(a1 = 0; a1 < 48; a1++){
    //     for(a2 = 0; a2 < 10; a2++){
    //         for(a3 = 0; a3 < 800; a3++){
    //             lcd_draw_point(a3,a2*48+a1,buff[a2*48+a1][a3]);
    //         }
    //         usleep(haomiao);
    //     }
    // }
    
	close(fd_lcd);
}

int main(int argc, char* argv[]){
    	if(argc > 5 ){
		printf("command unknown!");
		return 0;
	}
    draw_bmp("black.bmp",0,0,1);
    draw_bmp(argv[1], 0,0,atoi(argv[2]));
}