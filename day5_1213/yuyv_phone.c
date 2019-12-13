#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <getopt.h>          
#include <fcntl.h>            
#include <unistd.h>
#include <errno.h>
#include <malloc.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/time.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <asm/types.h>        
#include <linux/videodev2.h>
#include "libjpeg/jpeglib.h"
#include <strings.h>


#include "../../common/Function.h"


#define OUTPUT_BUF_SIZE  4096
#define CLEAR(x) memset (&(x), 0, sizeof (x))
#define WIDTH 640//320
#define HEIGHT 480//240

int flag;
extern int flag_cam;
extern flag_save_photo;//拍摄标志位

struct buffer {
	void   *start;
	size_t length;
};


typedef struct {
	struct jpeg_destination_mgr pub;
	JOCTET * buffer; 
	unsigned char *outbuffer;
	int outbuffer_size;
	unsigned char *outbuffer_cursor;
	int *written;
}mjpg_destination_mgr;

 
typedef mjpg_destination_mgr *mjpg_dest_ptr;
static char *           dev_name        = "/dev/video7";
static int              fd              = -1;
struct buffer *         buffers         = NULL;
static unsigned int     n_buffers       = 0;

static unsigned long file_length;
static unsigned char *file_name;

extern int lcd_draw_jpg(unsigned int x,unsigned int y,const char *pjpg_path,char *pjpg_buf,unsigned int jpg_buf_size,unsigned int jpg_half);

METHODDEF(void) init_destination(j_compress_ptr cinfo)
{
	mjpg_dest_ptr dest = (mjpg_dest_ptr) cinfo->dest;
	dest->buffer = (JOCTET *)(*cinfo->mem->alloc_small) ((j_common_ptr) cinfo, JPOOL_IMAGE, OUTPUT_BUF_SIZE * sizeof(JOCTET));
	*(dest->written) = 0;
	dest->pub.next_output_byte = dest->buffer;
	dest->pub.free_in_buffer = OUTPUT_BUF_SIZE;

}

METHODDEF(boolean) empty_output_buffer(j_compress_ptr cinfo)
{

	mjpg_dest_ptr dest = (mjpg_dest_ptr) cinfo->dest;
	memcpy(dest->outbuffer_cursor, dest->buffer, OUTPUT_BUF_SIZE);
	dest->outbuffer_cursor += OUTPUT_BUF_SIZE;
	*(dest->written) += OUTPUT_BUF_SIZE;
	dest->pub.next_output_byte = dest->buffer;
	dest->pub.free_in_buffer = OUTPUT_BUF_SIZE;

	return TRUE;

} 

METHODDEF(void) term_destination(j_compress_ptr cinfo)
{
	mjpg_dest_ptr dest = (mjpg_dest_ptr) cinfo->dest;
	size_t datacount = OUTPUT_BUF_SIZE - dest->pub.free_in_buffer;
	/* Write any data remaining in the buffer */
	memcpy(dest->outbuffer_cursor, dest->buffer, datacount);
	dest->outbuffer_cursor += datacount;
	*(dest->written) += datacount;

}

 
void dest_buffer(j_compress_ptr cinfo, unsigned char *buffer, int size, int *written)
{
	mjpg_dest_ptr dest;
	if (cinfo->dest == NULL) {
	cinfo->dest = (struct jpeg_destination_mgr *)(*cinfo->mem->alloc_small) ((j_common_ptr) cinfo, JPOOL_PERMANENT, sizeof(mjpg_destination_mgr));
	}

	dest = (mjpg_dest_ptr)cinfo->dest;
	dest->pub.init_destination = init_destination;
	dest->pub.empty_output_buffer = empty_output_buffer;
	dest->pub.term_destination = term_destination;
	dest->outbuffer = buffer;
	dest->outbuffer_size = size;
	dest->outbuffer_cursor = buffer;
	dest->written = written;

}


//摄像头采集的YUYV格式转换为JPEG格式
int compress_yuyv_to_jpeg(unsigned char *buf, unsigned char *buffer, int size, int quality) 
{

	struct jpeg_compress_struct cinfo;
	struct jpeg_error_mgr jerr;
	JSAMPROW row_pointer[1];
	unsigned char *line_buffer, *yuyv;
	int z;
	static int written;
	//int count = 0;
	//printf("%s\n", buf);

	line_buffer = calloc (WIDTH * 3, 1);
	yuyv = buf;//将YUYV格式的图片数据赋给YUYV指针
	//printf("compress start...\n");

	cinfo.err = jpeg_std_error (&jerr);
	jpeg_create_compress (&cinfo);

	/* jpeg_stdio_dest (&cinfo, file); */

	dest_buffer(&cinfo, buffer, size, &written);

	cinfo.image_width = WIDTH;
	cinfo.image_height = HEIGHT;
	cinfo.input_components = 3;
	cinfo.in_color_space = JCS_RGB;


	jpeg_set_defaults (&cinfo);
	jpeg_set_quality (&cinfo, quality, TRUE);
	jpeg_start_compress (&cinfo, TRUE);

	z = 0;

	while (cinfo.next_scanline < HEIGHT) 
	{
		int x;
		unsigned char *ptr = line_buffer;
		for (x = 0; x < WIDTH; x++) 
		{
			int r, g, b;
			int y, u, v;
			if (!z)
				y = yuyv[0] << 8;
			else
				y = yuyv[2] << 8;

			u = yuyv[1] - 128;
			v = yuyv[3] - 128;

			r = (y + (359 * v)) >> 8;
			g = (y - (88 * u) - (183 * v)) >> 8;
			b = (y + (454 * u)) >> 8;
			//r = y + v + (v * 103) >> 8;
			//g = y - (u * 88) >> 8 - (v * 183) >> 8;
			//b = y + u + (u * 198) >> 8;

			*(ptr++) = (r > 255) ? 255 : ((r < 0) ? 0 : r);
			*(ptr++) = (g > 255) ? 255 : ((g < 0) ? 0 : g);
			*(ptr++) = (b > 255) ? 255 : ((b < 0) ? 0 : b);

			if (z++) 
			{
				z = 0;
				yuyv += 4;
			}

		}
		row_pointer[0] = line_buffer;
		jpeg_write_scanlines (&cinfo, row_pointer, 1);

	}

	jpeg_finish_compress (&cinfo);
	jpeg_destroy_compress (&cinfo);
	free (line_buffer);
	
	return (written);

}

 
//读取一帧的内容
static int read_frame ()
{
	struct v4l2_buffer buf;
	int ret;
	unsigned int i;
	CLEAR (buf);
	
	buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	buf.memory = V4L2_MEMORY_MMAP;

	int ff = ioctl (fd, VIDIOC_DQBUF, &buf); //出列采集的帧缓冲
	if(ff<0)
		printf("failture\n");
	
	unsigned char src[buf.length+1];
	unsigned char dest[buf.length+1];

	assert (buf.index < n_buffers);
	//printf ("buf.index dq is %d,\n",buf.index);

	memcpy(src, buffers[buf.index].start, buf.length);
	ret = compress_yuyv_to_jpeg(src, dest,(WIDTH * HEIGHT), 80);//YUV数据转换成jpeg格式

	
	
	//============需要修改的地方==============
	//显示jpeg图片
	lcd_draw_jpg(0,0,NULL,dest,ret,0);
	//0,0 分别表示图片在lcd屏幕的起始x坐标和起始y坐标
	//NULL表示使用test-mmap.jpeg文件
	//dest==jpeg图片数据，ret==图片字节数
	//0表示是否需要缩放
	if(flag_save_photo){
		printf("read_frame: save photo...\n");
		char buff_jpegname[64] = {0};
		getTimeNow(buff_jpegname);
		strcat(buff_jpegname,".jpg");
		char path_jpeg[128] = {0};
		sprintf(path_jpeg,"/kjh/CAMERA/PICTURES/%s",buff_jpegname);
		printf("%s\n",path_jpeg);
		int fd_save_jpeg = open(path_jpeg,O_RDWR|O_CREAT,S_IRWXU);
		if(fd_save_jpeg == -1){
			printf("open fail.\n");
			flag_save_photo = 0;
			return -1;
		}
		write(fd_save_jpeg,dest,ret);
		printf("save %s in %s\n",buff_jpegname,path_jpeg);
		flag_save_photo = 0;
	}
	//打开一张"xxx.jpg"的图片，
	//如果不存在，则创建一张名叫"xxx.jpg"的图片，
	
	//将拍摄的照片信息进行保存，保存到xxx.jpg
	
	//关闭图片
	
	//=========================================

	ff=ioctl (fd, VIDIOC_QBUF, &buf); //重新入列
	if(ff<0)
		printf("failture VIDIOC_QBUF\n");

	return 1;

}

 
int get_picture()
{
	//定义变量
	struct v4l2_capability cap;
	struct v4l2_format fmt;
	unsigned int i;
	enum v4l2_buf_type type;

	//打开设备/dev/video7
	fd = open (dev_name, O_RDWR | O_NONBLOCK, 0);
	
	//获取摄像头参数
	int ff=ioctl(fd, VIDIOC_QUERYCAP, &cap);

	//检查摄像头是否连接上了开发板
	if(ff<0)
		printf("failture VIDIOC_QUERYCAP\n");

	struct v4l2_fmtdesc fmt1;
	int ret;

	//清除摄像头缓冲区
	memset(&fmt1, 0, sizeof(fmt1));
	fmt1.index = 0;

	fmt1.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;

	//查看并配置摄像头格式
	while ((ret = ioctl(fd, VIDIOC_ENUM_FMT, &fmt1)) == 0)
	{
		fmt1.index++;
		printf("{ pixelformat = '%c%c%c%c', description = '%s' }\n",
		fmt1.pixelformat & 0xFF, (fmt1.pixelformat >> 8) & 0xFF,
		(fmt1.pixelformat >> 16) & 0xFF, (fmt1.pixelformat >> 24) & 0xFF,fmt1.description);
	}

	//配置摄像头格式
	CLEAR (fmt);
	fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	fmt.fmt.pix.width       = 640;//320;
	fmt.fmt.pix.height      = 480;//240;
	fmt.fmt.pix.pixelformat = V4L2_PIX_FMT_YUYV;

	fmt.fmt.pix.field       = V4L2_FIELD_INTERLACED;

	ff = ioctl (fd, VIDIOC_S_FMT, &fmt); //设置图像格式
	if(ff<0)
	printf("failture VIDIOC_S_FMT\n");


	file_length = fmt.fmt.pix.bytesperline * fmt.fmt.pix.height; //计算图片大小
	struct v4l2_requestbuffers req;
	CLEAR (req);
	req.count               = 4;
	req.type                = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	req.memory              = V4L2_MEMORY_MMAP;

	//申请缓冲，count是申请的数量
	ioctl(fd, VIDIOC_REQBUFS, &req);  
	if(ff<0)
	printf("failture VIDIOC_REQBUFS\n");

	if (req.count < 1)
	printf("Insufficient buffer memory\n");

	buffers = calloc (req.count, sizeof (*buffers));//内存中建立对应空间


	//将4个缓冲区都指向lcd屏幕的映射内存
	for (n_buffers = 0; n_buffers < req.count; ++n_buffers)
	{
		struct v4l2_buffer buf; 
		CLEAR (buf);
		buf.type        = V4L2_BUF_TYPE_VIDEO_CAPTURE;
		buf.memory      = V4L2_MEMORY_MMAP;
		buf.index       = n_buffers;
		if (-1 == ioctl (fd, VIDIOC_QUERYBUF, &buf)) //映射用户空间
			printf ("VIDIOC_QUERYBUF error\n");

		buffers[n_buffers].length = buf.length;
		buffers[n_buffers].start=mmap(NULL,buf.length,PROT_READ|PROT_WRITE,MAP_SHARED,fd,buf.m.offset); //通过mmap建立映射关系
		
		if (MAP_FAILED == buffers[n_buffers].start)
		printf ("mmap failed\n");
	}


	for (i = 0; i < n_buffers; ++i)
	{
		struct v4l2_buffer buf;
		CLEAR (buf);
		buf.type        = V4L2_BUF_TYPE_VIDEO_CAPTURE;
		buf.memory      = V4L2_MEMORY_MMAP;
		buf.index       = i;

		if (-1 == ioctl (fd, VIDIOC_QBUF, &buf))//申请到的缓冲进入列队
		printf ("VIDIOC_QBUF failed\n");
	}

	//开启摄像头
	type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	if (-1 == ioctl (fd, VIDIOC_STREAMON, &type)) //开始捕捉图像数据
	printf ("VIDIOC_STREAMON failed\n");

	sleep(1);
	int time_flag=0;
	for (;;) //这一段涉及到异步IO ==>while(1)
	{
		fd_set fds;
		struct timeval tv;
		int r;

		FD_ZERO (&fds);//将指定的文件描述符集清空
		FD_SET (fd, &fds);//在文件描述符集合中增加一个新的文件描述符
		/* Timeout. */
		
		tv.tv_sec = 2;
		tv.tv_usec = 0;

		r = select (fd + 1, &fds, NULL, NULL, &tv);//判断是否可读（即摄像头是否准备好），tv是定时
		if (-1 == r)
		{
			if (EINTR == errno)
			//continue;
			printf ("select err\n");
		}

		if (0 == r)
		{
			fprintf (stderr, "select timeout\n");
			exit (EXIT_FAILURE);
		}

		if (read_frame ())//如果可读，执行read_frame函数，把采集到的每一帧图像显示到开发板上
		{
			//printf("OK\n");
		}
		/*
		//表示摄像采集数据的次数
		time_flag++;
		if(time_flag==100)
			break;//跳出循环
		*/
		
		if(flag_cam == 0)
			break;
	}
unmap:
	for (i = 0; i < n_buffers; ++i)
	if (-1 == munmap (buffers[i].start, buffers[i].length))
		 printf ("munmap error");
		 type = V4L2_BUF_TYPE_VIDEO_CAPTURE;  

	if (-1 == ioctl(fd, VIDIOC_STREAMOFF, &type))   
		printf("VIDIOC_STREAMOFF"); 
	close (fd);
	//exit (EXIT_SUCCESS);
	return 0;
}


