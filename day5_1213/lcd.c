/****************************************************************************************
 *�ļ�����:lcd.c
 *˵	��:	1)֧����ʾ�����С��λͼ
			2)�Զ�ʶ��λͼ��ʽ	
  -------------------------------------------------------------------------------------
 *�޸�����:2015-6-5
			1)�����jpg�ļ���jpg����ʾ�Ĺ���
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


/*���л���ʾ������ */
volatile int g_jpg_in_jpg_x;
volatile int g_jpg_in_jpg_y;



/****************************************************
 *��������:file_size_get
 *�������:pfile_path	-�ļ�·��
 *�� �� ֵ:-1		-ʧ��
		   ����ֵ	-�ļ���С
 *˵	��:��ȡ�ļ���С
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

//��ʼ��LCD
int lcd_open(const char *str)
{
	g_fb_fd = open(str, O_RDWR);
	
	if(g_fb_fd<0)
	{
			printf("open lcd error\n");
			return -1;
	}

	g_pfb_memory  = (int *)mmap(	NULL, 					//ӳ�����Ŀ�ʼ��ַ������ΪNULLʱ��ʾ��ϵͳ����ӳ��������ʼ��ַ
									FB_SIZE, 				//ӳ�����ĳ���
									PROT_READ|PROT_WRITE, 	//���ݿ��Ա���ȡ��д��
									MAP_SHARED,				//�����ڴ�
									g_fb_fd, 				//��Ч���ļ�������
									0						//��ӳ��������ݵ����
								);

	return g_fb_fd;

}

//LCD�ر�
void close_lcd(void)
{
	
	/* ȡ���ڴ�ӳ�� */
	munmap(g_pfb_memory, FB_SIZE);
	
	/* �ر�LCD�豸 */
	close(g_fb_fd);
}

//LCD����
void lcd_draw_point(unsigned int x,unsigned int y, unsigned int color)
{
	*(g_pfb_memory+y*800+x)=color;
}

/**
 * BMPͼƬ�Ľṹ�塣
 * ��������Ա��
 * ���򳤶�length
 * �����high
*/
typedef struct
{
    int length;
    int high;
} Info_bmp;

/**
 * path��bmpͼƬ��λ��
 * ����ֵ��ָ��bmpͼƬ��fd
*/
int open_bitmap(char* path){
    int fd_bmp = open(path, O_RDWR);
    return fd_bmp;
}

/**
 * information[18-21] ��λ����λ��ˮƽ
 * 			  [22-25] ��λ����λ����ֱ
 * ��ͼƬ�ĳ�����Ϣ��ŵ�Info_bmp�ṹ����
*/
int read_bmpinfo(int fd_bmp, Info_bmp* info_bmp){
    char information[54];
	read(fd_bmp,information,sizeof(information));
	info_bmp->length = information[18] | information[19] << 8 | information[20] << 16 | information[21] << 24;
	info_bmp->high = information[22] | information[23] << 8 | information[24] << 16 | information[25] << 24;
	//printf("length:%d\thigh:%d\t\n",info_bmp->length,info_bmp->high);
}

/**
 * ��������bmp�ļ������������ݴ洢��һ��buff�С�
 * Ӧ��Ҫ�ж�̬�洢�ռ�ġ����������ڻ����Ǻ������
 * ���ⲿ�������buff[length][width].��ô�洢�ռ��������ģ�
 * fd_bmp:Ҫ��ȡ��bmp�ļ�����fd
 * buff��������Ϣ��ŵĵط�,��ά���鲻����...
 * length��high�����Ϳ�
*/
int read_bmp(int fd_bmp, int *buff,int length, int high){
    int m = (length*3) % 4;//�鿴�Ƿ��ܱ�4����
    //printf("yushu: %d\n",m);
    if(m == 0){//�ܱ�����������ֱ��ת����int
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
        
    }else{//���ܱ���������˵��ÿһ�ж��в���
        int buqi = 4 - m;//ÿһ�ж���buqi���ֽ�����
        //������ֱ�Ӷ���һ������ȫ�̽���
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
 * �ر�bmpͼƬ
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
    //���ϰ�bmpͼƬ��Ϣ����buff��

    //�������ǽ�buffд��lcd��Ļ��
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


//��ʾjpegͼƬ�Ĵ���
//��ʾ����ʼλ�õ�X������
//��ʾ����ʼλ�õ�Y������
//��ʾ��ͼƬ��·�������4��5������������
//��ɫ�������ĵ�ַ
//��ɫ���ݵĴ�С
//�Ƿ�������ʾ��ֻ������1�루��Ⱥ͸߶ȶ���Сһ�룩��ֻ��ѡ����٣�����ѡ�����ű���
int lcd_draw_jpg(unsigned int x,unsigned int y,const char *pjpg_path,char *pjpg_buf,unsigned int jpg_buf_size,unsigned int jpg_half)  
{
	/*���������󣬴��������*/
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
		/* ����jpg��Դ��Ȩ�޿ɶ���д */	
		jpg_fd=open(pjpg_path,O_RDWR);
		
		if(jpg_fd == -1)
		{
		   printf("open %s error\n",pjpg_path);
		   
		   return -1;	
		}	
		
		/* ��ȡjpg�ļ��Ĵ�С */
		jpg_size=file_size_get(pjpg_path);	

		/* Ϊjpg�ļ������ڴ�ռ� */	
		pjpg = malloc(jpg_size);

		/* ��ȡjpg�ļ��������ݵ��ڴ� */		
		read(jpg_fd,pjpg,jpg_size);
	}
	else
	{
		jpg_size = jpg_buf_size;
		
		pjpg = pjpg_buf;
	}

	/*ע�������*/
	cinfo.err = jpeg_std_error(&jerr);

	/*��������*/
	jpeg_create_decompress(&cinfo);

	/*ֱ�ӽ����ڴ�����*/		
	jpeg_mem_src(&cinfo,pjpg,jpg_size);
	
	/*���ļ�ͷ*/
	jpeg_read_header(&cinfo, TRUE);

	/*��ʼ����*/
	jpeg_start_decompress(&cinfo);	
	
	
	if(jpg_half)
	{
		x_e	= x_s+(cinfo.output_width/2);
		y_e	= y  +(cinfo.output_height/2);		
		
		/*����������*/
		while(cinfo.output_scanline < cinfo.output_height)
		{		
			pcolor_buf = g_color_buf;
			
			/* ��ȡjpgһ�е�rgbֵ */
			jpeg_read_scanlines(&cinfo,(JSAMPARRAY)&pcolor_buf,1);			
			
			/* �ٶ�ȡjpgһ�е�rgbֵ */
			jpeg_read_scanlines(&cinfo,(JSAMPARRAY)&pcolor_buf,1);

			for(i=0; i<(cinfo.output_width/2); i++)
			{
				/* ��ȡrgbֵ */
				color = 		*(pcolor_buf+2);
				color = color | *(pcolor_buf+1)<<8;
				color = color | *(pcolor_buf)<<16;
				
				/* ��ʾ���ص� */
				lcd_draw_point(x,y,color);
				
				pcolor_buf +=6;
				
				x++;
			}
			
			/* ���� */
			y++;					
			
			
			x = x_s;	

			
		}
	}
	else
	{
		x_e	= x_s+cinfo.output_width;
		y_e	= y  +cinfo.output_height;	

		/*����������*/
		while(cinfo.output_scanline < cinfo.output_height )
		{		
			pcolor_buf = g_color_buf;
			
			/* ��ȡjpgһ�е�rgbֵ */
			jpeg_read_scanlines(&cinfo,(JSAMPARRAY)&pcolor_buf,1);
			
			for(i=0; i<cinfo.output_width; i++)
			{
				/* ��ȡrgbֵ */
				color = 		*(pcolor_buf+2);
				color = color | *(pcolor_buf+1)<<8;
				color = color | *(pcolor_buf)<<16;
				
				/* ��ʾ���ص� */
				lcd_draw_point(x,y,color);
				
				pcolor_buf +=3;
				
				x++;
			}
			
			/* ���� */
			y++;			
			
			x = x_s;
			
		}		
	}
				
	/*�������*/
	jpeg_finish_decompress(&cinfo);
	jpeg_destroy_decompress(&cinfo);

	if(pjpg_path!=NULL)
	{
		/* �ر�jpg�ļ� */
		close(jpg_fd);	
		
		/* �ͷ�jpg�ļ��ڴ�ռ� */
		free(pjpg);		
	}
	
	return 0;
}