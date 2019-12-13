/****************************************************************************************
 *文件名称:lcd.c
 *说	明:	1)支持显示任意大小的位图
			2)自动识别位图格式	
  -------------------------------------------------------------------------------------
 *修改日期:2015-6-5
			1)添加了jpg文件、jpg流显示的功能
****************************************************************************************/
#include <stdio.h>   	//printf scanf
#include <fcntl.h>		//open write read lseek close  	 
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <stdlib.h>
#include "libjpeg/jpeglib.h"

#define LCD_WIDTH  			800
#define LCD_HEIGHT 			480
#define FB_SIZE				(LCD_WIDTH * LCD_HEIGHT * 4)
#define EN_LCD_SHOW_JPG		1

#define MAX_BMP_SIZE 1024
#define PIXELS_COLUMN 800
#define PIXELS_ROW 480


static char g_color_buf[FB_SIZE]={0};

static int  g_fb_fd;
static int *g_pfb_memory;


/*画中画显示的坐标 */
volatile int g_jpg_in_jpg_x;
volatile int g_jpg_in_jpg_y;



/****************************************************
 *函数名称:file_size_get
 *输入参数:pfile_path	-文件路径
 *返 回 值:-1		-失败
		   其他值	-文件大小
 *说	明:获取文件大小
 ****************************************************/
unsigned long file_size_get(const char *pfile_path)
{
	unsigned long filesize = -1;	
	struct stat statbuff;
	
	if(stat(pfile_path, &statbuff) < 0)
	{
		return filesize;
	}
	else
	{
		filesize = statbuff.st_size;
	}
	
	return filesize;
}

//初始化LCD
int lcd_open(const char *str)
{
	g_fb_fd = open(str, O_RDWR);
	
	if(g_fb_fd<0)
	{
			printf("open lcd error\n");
			return -1;
	}

	g_pfb_memory  = (int *)mmap(	NULL, 					//映射区的开始地址，设置为NULL时表示由系统决定映射区的起始地址
									FB_SIZE, 				//映射区的长度
									PROT_READ|PROT_WRITE, 	//内容可以被读取和写入
									MAP_SHARED,				//共享内存
									g_fb_fd, 				//有效的文件描述词
									0						//被映射对象内容的起点
								);

	return g_fb_fd;

}

//LCD关闭
void close_lcd(void)
{
	
	/* 取消内存映射 */
	munmap(g_pfb_memory, FB_SIZE);
	
	/* 关闭LCD设备 */
	close(g_fb_fd);
}

//LCD画点
void lcd_draw_point(unsigned int x,unsigned int y, unsigned int color)
{
	*(g_pfb_memory+y*800+x)=color;
}

/**
 * BMP图片的结构体。
 * 有两个成员：
 * 横向长度length
 * 纵向高high
*/
typedef struct
{
    int length;
    int high;
} Info_bmp;

/**
 * path：bmp图片的位置
 * 返回值：指向bmp图片的fd
*/
int open_bitmap(char* path){
    int fd_bmp = open(path, O_RDWR);
    return fd_bmp;
}

/**
 * information[18-21] 地位到高位：水平
 * 			  [22-25] 低位到高位：竖直
 * 将图片的长宽信息存放到Info_bmp结构体中
*/
int read_bmpinfo(int fd_bmp, Info_bmp* info_bmp){
    char information[54];
	read(fd_bmp,information,sizeof(information));
	info_bmp->length = information[18] | information[19] << 8 | information[20] << 16 | information[21] << 24;
	info_bmp->high = information[22] | information[23] << 8 | information[24] << 16 | information[25] << 24;
	//printf("length:%d\thigh:%d\t\n",info_bmp->length,info_bmp->high);
}

/**
 * 接下来是bmp文件，将像素数据存储到一个buff中。
 * 应该要有动态存储空间的。。。但现在还不是很清楚。
 * 在外部定义变量buff[length][width].那么存储空间是怎样的？
 * fd_bmp:要读取的bmp文件标致fd
 * buff：像素信息存放的地方,二维数组不可用...
 * length：high：长和宽
*/
int read_bmp(int fd_bmp, int *buff,int length, int high){
    int m = (length*3) % 4;//查看是否能被4整除
    //printf("yushu: %d\n",m);
    if(m == 0){//能被四整除，则直接转换成int
        char buff_unprocess[length*high*3];
        //lseek(fd_bmp,54,SEEK_SET);
        read(fd_bmp,buff_unprocess,sizeof(buff_unprocess));
        int i, j;
        for(i = 0; i < length; i ++){
            for(j = 0; j < high; j ++){
                *(buff+(high - j - 1)*length+i) = buff_unprocess[(j*length + i)*3 ]         | 
                                                  buff_unprocess[(j*length + i)*3 + 1] << 8 |
                                                  buff_unprocess[(j*length + i)*3 + 2] << 16;
            }
        }
        
    }else{//不能被四整除，说明每一行都有补齐
        int buqi = 4 - m;//每一行都有buqi个字节冗余
        //在这里直接定义一个数组全盘接受
        char buff_unprocess[(length*3 + buqi)*high];
        read(fd_bmp,buff_unprocess,sizeof(buff_unprocess));
        int i, j;
        for(i = 0; i < length; i++){
            for(j = 0; j < high; j++){
                *(buff+(high - j - 1)*length+i)  = buff_unprocess[(j*length + i)*3 + buqi*j]          | 
                                                   buff_unprocess[(j*length + i)*3 + 1 + buqi*j] << 8 |
                                                   buff_unprocess[(j*length + i)*3 + 2 + buqi*j] << 16;
            }
        }
    }
    return 0;
}

/**
 * 关闭bmp图片
*/
int close_bitmap(int fd_bmp){
    close(fd_bmp);
}



int lcd_draw_bmp(char* path_bmp,int off_x, int off_y){
    int fd_bmp = open_bitmap(path_bmp);
    if(fd_bmp == -1){
        printf("open fd_bmp:%s fail.\n",path_bmp);
        return -1;
    }
    Info_bmp info_bmp;
    read_bmpinfo(fd_bmp,&info_bmp);
    int length = info_bmp.length;
    int high = info_bmp.high;
    if(length> MAX_BMP_SIZE || high > MAX_BMP_SIZE){
        printf("bmp over size! should not be larger than %d",MAX_BMP_SIZE);
        return -2;
    }
    if(off_x>PIXELS_COLUMN || off_x < (-length) || off_y > PIXELS_ROW || off_y < (-high)){
        printf("offset over size!");
        return -3;
    }
    int buff[length*high];
    read_bmp(fd_bmp,buff,length,high);
    //以上把bmp图片信息读入buff中

    //接下来是将buff写入lcd屏幕中
    int i,j;
    // printf("off_x:%d\toff_y%d\t\n",off_x,off_y);
    for(i= (off_x<0)? -off_x:0; i<length && (off_x+i)<PIXELS_COLUMN; i++){
        for(j = (off_y<0)?-off_y:0; j < high&& (off_y+j)<PIXELS_ROW; j++){

            lcd_draw_point(off_x+i,off_y+j,buff[j*length+i]);
            // printf("(%d, %d): (%d, %d)\t",off_x+i,off_y+j,i,j);
        }
    }

    close_bitmap(fd_bmp);

}


//显示jpeg图片的代码
//显示的起始位置的X轴坐标
//显示的起始位置的Y轴坐标
//显示的图片的路径，与第4、5两个参数互斥
//颜色数据流的地址
//颜色数据的大小
//是否缩放显示，只能缩放1半（宽度和高度都缩小一半），只能选择真假，不能选择缩放倍数
int lcd_draw_jpg(unsigned int x,unsigned int y,const char *pjpg_path,char *pjpg_buf,unsigned int jpg_buf_size,unsigned int jpg_half)  
{
	/*定义解码对象，错误处理对象*/
	struct 	jpeg_decompress_struct 	cinfo;
	struct 	jpeg_error_mgr 			jerr;	
	
	char 	*pcolor_buf = g_color_buf;
	char 	*pjpg;
	
	unsigned int 	i=0;
	unsigned int	color =0;
	unsigned int	count =0;
	
	unsigned int 	x_s = x;
	unsigned int 	x_e ;	
	unsigned int 	y_e ;
	
			 int	jpg_fd;
	unsigned int 	jpg_size;
	
	unsigned int 	jpg_width;
	unsigned int 	jpg_height;

	if(pjpg_path!=NULL)
	{
		/* 申请jpg资源，权限可读可写 */	
		jpg_fd=open(pjpg_path,O_RDWR);
		
		if(jpg_fd == -1)
		{
		   printf("open %s error\n",pjpg_path);
		   
		   return -1;	
		}	
		
		/* 获取jpg文件的大小 */
		jpg_size=file_size_get(pjpg_path);	

		/* 为jpg文件申请内存空间 */	
		pjpg = malloc(jpg_size);

		/* 读取jpg文件所有内容到内存 */		
		read(jpg_fd,pjpg,jpg_size);
	}
	else
	{
		jpg_size = jpg_buf_size;
		
		pjpg = pjpg_buf;
	}

	/*注册出错处理*/
	cinfo.err = jpeg_std_error(&jerr);

	/*创建解码*/
	jpeg_create_decompress(&cinfo);

	/*直接解码内存数据*/		
	jpeg_mem_src(&cinfo,pjpg,jpg_size);
	
	/*读文件头*/
	jpeg_read_header(&cinfo, TRUE);

	/*开始解码*/
	jpeg_start_decompress(&cinfo);	
	
	
	if(jpg_half)
	{
		x_e	= x_s+(cinfo.output_width/2);
		y_e	= y  +(cinfo.output_height/2);		
		
		/*读解码数据*/
		while(cinfo.output_scanline < cinfo.output_height)
		{		
			pcolor_buf = g_color_buf;
			
			/* 读取jpg一行的rgb值 */
			jpeg_read_scanlines(&cinfo,(JSAMPARRAY)&pcolor_buf,1);			
			
			/* 再读取jpg一行的rgb值 */
			jpeg_read_scanlines(&cinfo,(JSAMPARRAY)&pcolor_buf,1);

			for(i=0; i<(cinfo.output_width/2); i++)
			{
				/* 获取rgb值 */
				color = 		*(pcolor_buf+2);
				color = color | *(pcolor_buf+1)<<8;
				color = color | *(pcolor_buf)<<16;
				
				/* 显示像素点 */
				lcd_draw_point(x,y,color);
				
				pcolor_buf +=6;
				
				x++;
			}
			
			/* 换行 */
			y++;					
			
			
			x = x_s;	

			
		}
	}
	else
	{
		x_e	= x_s+cinfo.output_width;
		y_e	= y  +cinfo.output_height;	

		/*读解码数据*/
		while(cinfo.output_scanline < cinfo.output_height )
		{		
			pcolor_buf = g_color_buf;
			
			/* 读取jpg一行的rgb值 */
			jpeg_read_scanlines(&cinfo,(JSAMPARRAY)&pcolor_buf,1);
			
			for(i=0; i<cinfo.output_width; i++)
			{
				/* 获取rgb值 */
				color = 		*(pcolor_buf+2);
				color = color | *(pcolor_buf+1)<<8;
				color = color | *(pcolor_buf)<<16;
				
				/* 显示像素点 */
				lcd_draw_point(x,y,color);
				
				pcolor_buf +=3;
				
				x++;
			}
			
			/* 换行 */
			y++;			
			
			x = x_s;
			
		}		
	}
				
	/*解码完成*/
	jpeg_finish_decompress(&cinfo);
	jpeg_destroy_decompress(&cinfo);

	if(pjpg_path!=NULL)
	{
		/* 关闭jpg文件 */
		close(jpg_fd);	
		
		/* 释放jpg文件内存空间 */
		free(pjpg);		
	}
	
	return 0;
}