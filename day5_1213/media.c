#include <dirent.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>		//printf
#include <sys/types.h>	//文件IO
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <strings.h>	//bzero
#include <sys/mman.h>	//mmap
#include <linux/input.h>	//输入子系统
#include <pthread.h>
#include <math.h>


#define CAMERA_LOW_X 80
#define CAMERA_HIGH_X 320
#define CAMERA_LOW_Y 50
#define CAMERA_HIGH_Y 230
 
#define MUSIC_LOW_X 460
#define MUSIC_HIGH_X 700
#define MUSIC_LOW_Y 50
#define MUSIC_HIGH_Y 230

#define VEDIO_LOW_X 80
#define VEDIO_HIGH_X 320
#define VEDIO_LOW_Y 260
#define VEDIO_HIGH_Y 460

#define PHOTO_LOW_X 460
#define PHOTO_HIGH_X 700
#define PHOTO_LOW_Y 260
#define PHOTO_HIGH_Y 460

#define RETURN_LOW_X 730
#define RETURN_HIGH_X 800
#define RETURN_LOW_Y 0
#define RETURN_HIGH_Y 60


#define NUM_1_LOW_X 0
#define NUM_1_LOW_Y 250
#define NUM_1_HIGH_X 217
#define NUM_1_HIGH_Y 330

#define NUM_2_LOW_X 217
#define NUM_2_LOW_Y 250
#define NUM_2_HIGH_X 434
#define NUM_2_HIGH_Y 330

#define NUM_3_LOW_X 434
#define NUM_3_LOW_Y 250
#define NUM_3_HIGH_X 651
#define NUM_3_HIGH_Y 330

#define NUM_4_LOW_X 0
#define NUM_4_LOW_Y 330
#define NUM_4_HIGH_X 217
#define NUM_4_HIGH_Y 410

#define NUM_5_LOW_X 217
#define NUM_5_LOW_Y 330
#define NUM_5_HIGH_X 434
#define NUM_5_HIGH_Y 410

#define NUM_6_LOW_X 434
#define NUM_6_LOW_Y 330
#define NUM_6_HIGH_X 651
#define NUM_6_HIGH_Y 410

#define NUM_7_LOW_X 0
#define NUM_7_LOW_Y 410
#define NUM_7_HIGH_X 217
#define NUM_7_HIGH_Y 480

#define NUM_8_LOW_X 217
#define NUM_8_LOW_Y 410
#define NUM_8_HIGH_X 434
#define NUM_8_HIGH_Y 480

#define NUM_9_LOW_X 434
#define NUM_9_LOW_Y 410
#define NUM_9_HIGH_X 651
#define NUM_9_HIGH_Y 480

#define NUM_0_LOW_X 651
#define NUM_0_LOW_Y 330
#define NUM_0_HIGH_X 800
#define NUM_0_HIGH_Y 410

#define BACKSPACE_LOW_X 651
#define BACKSPACE_LOW_Y 250
#define BACKSPACE_HIGH_X 800
#define BACKSPACE_HIGH_Y 330

#define ENTER_LOW_X 651
#define ENTER_LOW_Y 410
#define ENTER_HIGH_X 800
#define ENTER_HIGH_Y 480

#define SET_PWD_LOW_X 620
#define SET_PWD_LOW_Y 0
#define SET_PWD_HIGH_X 800
#define SET_PWD_HIGH_Y 80

#define EXIT_PWD_LOW_X 0
#define EXIT_PWD_LOW_Y 0
#define EXIT_PWD_HIGH_X 65
#define EXIT_PWD_HIGH_Y 65


// extern int camera();
// extern int music();
// extern int photo();
// extern int vedio();


int fd_ts;

int ts_init()
{
	fd_ts = open("/dev/input/event0",O_RDWR);
	if(-1 == fd_ts)
	{
		printf("open ts failed !\n");
		return -1;
	}
}

int ts_uinit()
{
	close(fd_ts);
}

int get_x_y(int *x,int *y)
{
	struct input_event ts;
	
	while(1)
	{
		bzero(&ts,sizeof(ts));
		read(fd_ts,&ts,sizeof(ts));
		//printf("type=%d,code=%d,value=%d\n",ts.type,ts.code,ts.value);
	
		if(ts.type==EV_ABS&&ts.code==ABS_X)
		{
			*x=ts.value;
		}
		
		if(ts.type==EV_ABS&&ts.code==ABS_Y)
		{
			*y=ts.value;
		}
		
		if(ts.type==EV_ABS&&ts.code==ABS_PRESSURE&&ts.value==0)
		{
			break;
		}
		else if(ts.type==EV_KEY&&ts.code==BTN_TOUCH&&ts.value==0)
		{
			break;
		}
	}
}


/**                                     camera                        **/
// int camera(){
//     printf("camera\n");
// }
extern int lcd_open(const char *str);
extern void close_lcd_(void);
extern int lcd_draw_jpg(unsigned int x,unsigned int y,const char *pjpg_path,char *pjpg_buf,unsigned int jpg_buf_size,unsigned int jpg_half);
extern int lcd_draw_bmp(char*, int, int);
extern int get_picture();

int flag_cam = 1;

void *func(void *arg)
{
    flag_cam = 1;
	get_picture();
}

int flag_save_photo = 0;

int cam_filter(const struct dirent *file_info){
    int length = 0;
    while(file_info->d_name[length]){
        length ++;
    }
    char* last_four_character = file_info->d_name + length - 4;
    if(strcmp(last_four_character,".jpg") == 0){
        return 1;
    }
    return 0;
}
int camera_photos(){
	struct dirent **namelist;
    int n;
    n = scandir("/kjh/CAMERA/PICTURES/", &namelist, cam_filter, alphasort);
    if (n < 0)
        perror("scandir");
    // else {
    //     while (n--) {
        
    //         printf("%d:%s\n", n,namelist[n]->d_name);
    //         free(namelist[n]);
    //     }
    //     free(namelist);
    // }
    int count = 0;
    char path_picture[256] ;
    int pointer_picture;
    int x, y;
    // char* path_return = "/kjh/PHOTO/PICTURES_SYSTEM/return.bmp";

    do{
        pointer_picture = (int)fabs(count) % n;
        sprintf(path_picture,"/kjh/CAMERA/PICTURES/%s",namelist[pointer_picture]->d_name);
		lcd_draw_jpg(0,0,path_picture,NULL,0,0);
    // lcd_draw_bmp(path_return,0,0);
        get_x_y(&x,&y);
        if(x<320){
            count--;
        }else if(x >= 320 && x<640 ){
            count++;
        }else{
            return 0;
    }
    } while(1) ;
}

int camera()
{
	int getpoint_x, getpoint_y;

	
	lcd_draw_bmp("/kjh/CAMERA/RESOURCES/camera_toolbar.bmp",640,0);


	pthread_t pid;
	//开启摄像头
	pthread_create(&pid,NULL,func,NULL);
	while(1){
		get_x_y(&getpoint_x,&getpoint_y);
		if(getpoint_x > 640){
			if(getpoint_y < 160){
				// printf("main:return\n");
				flag_cam = 0;
                lcd_draw_bmp("/kjh/RESOURCES/home.bmp",0,0); 
				return 0;
			}else if(getpoint_y >= 320){
				flag_cam = 0;
				camera_photos();
                lcd_draw_bmp("/kjh/RESOURCES/home.bmp",0,0); 
				return 0;
			}else{
				flag_save_photo = 1;
			}
		}
	}

	//关闭摄像头
	//获取触摸屏的坐标点，直接设置flag_cam = 0;
	// scanf("%d",&flag_cam);
	// sleep(2);
	


	// return 0;
}
/**                 camera            **/

/****************************************password*********************/
void getInputPwd(int *pwdLength, char *pwd, char num);
int verifyPwd(int pwdLength, char *pwd);
void setPassword(int pwdLength, char *pwd);

/**
 * 1.进入密码界面
 * 2.获取输入
 * 3.回车后，校验密码
 * 4.输入密码后，可以设置密码将密码保存到密码文件
 * 
 * 返回值
 * 0：退出
 * 1：密码正确进入程序
*/
int password()
{
    //显示密码界面
    lcd_draw_bmp("/kjh/RESOURCES/password.bmp", 0, 0);
    lcd_draw_bmp("/kjh/RESOURCES/lock.bmp", 345, 40);
    lcd_draw_bmp("/kjh/RESOURCES/exit.bmp", 0, 0);
    char pwd[8];
    int pwdLength = 0;
    int x, y;              //获取点击坐标
    int verifyReturn = -1; //密码验证结果,1：密码正确；0：密码错误；-1：读取密码文件失败

    while (1)
    {
        get_x_y(&x, &y);
        if (x > NUM_1_LOW_X && x < NUM_1_HIGH_X)
        {
            if (y > NUM_1_LOW_Y && y < NUM_1_HIGH_Y)
            {
                printf("1\n");
                getInputPwd(&pwdLength, pwd, '1');
            }
            if (y > NUM_4_LOW_Y && y < NUM_4_HIGH_Y)
            {
                printf("4\n");
                getInputPwd(&pwdLength, pwd, '4');
            }
            if (y > NUM_7_LOW_Y && y < NUM_7_HIGH_Y)
            {
                printf("7\n");
                getInputPwd(&pwdLength, pwd, '7');
            }
        }
        if (x > NUM_2_LOW_X && x < NUM_2_HIGH_X)
        {
            if (y > NUM_2_LOW_Y && y < NUM_2_HIGH_Y)
            {
                printf("2\n");
                getInputPwd(&pwdLength, pwd, '2');
            }
            if (y > NUM_5_LOW_Y && y < NUM_5_HIGH_Y)
            {
                printf("5\n");
                getInputPwd(&pwdLength, pwd, '5');
            }
            if (y > NUM_8_LOW_Y && y < NUM_8_HIGH_Y)
            {
                printf("8\n");
                getInputPwd(&pwdLength, pwd, '8');
            }
        }
        if (x > NUM_3_LOW_X && x < NUM_3_HIGH_X)
        {
            if (y > NUM_3_LOW_Y && y < NUM_3_HIGH_Y)
            {
                printf("3\n");
                getInputPwd(&pwdLength, pwd, '3');
            }
            if (y > NUM_6_LOW_Y && y < NUM_6_HIGH_Y)
            {
                printf("6\n");
                getInputPwd(&pwdLength, pwd, '6');
            }
            if (y > NUM_9_LOW_Y && y < NUM_9_HIGH_Y)
            {
                printf("9\n");
                getInputPwd(&pwdLength, pwd, '9');
            }
        }
        if (x > NUM_0_LOW_X && x < NUM_0_HIGH_X)
        {
            if (y > NUM_0_LOW_Y && y < NUM_0_HIGH_Y)
            {
                printf("0\n");
                getInputPwd(&pwdLength, pwd, '0');
            }
            if (y > BACKSPACE_LOW_Y && y < BACKSPACE_HIGH_Y)
            {
                printf("backspace\n");
                if (pwdLength != 0)
                {
                    lcd_draw_bmp("/kjh/RESOURCES/white.bmp", 20 + 65 * (pwdLength - 1), 185);
                    pwdLength--;
                }
            }
            if (y > ENTER_LOW_Y && y < ENTER_HIGH_Y)
            {
                printf("enter\n");
                verifyReturn = verifyPwd(pwdLength, pwd);
                printf("verify result:");
                printf("%d\n", verifyReturn);
                if (verifyReturn == 0)
                {
                    lcd_draw_bmp("/kjh/RESOURCES/pwd_wrong.bmp", 20, 130);
                }
                if (verifyReturn == 1)
                {
                    lcd_draw_bmp("/kjh/RESOURCES/unlock.bmp", 342, 22);
                    lcd_draw_bmp("/kjh/RESOURCES/white.bmp", 0, 100);
                    lcd_draw_bmp("/kjh/RESOURCES/white.bmp", 50, 100);
                    sleep(1);
                    return 1;
                }
                //pwdLength归零
                if (pwdLength != 0)
                {
                    while (1)
                    {
                        lcd_draw_bmp("/kjh/RESOURCES/white.bmp", 20 + 65 * (pwdLength - 1), 185);
                        if (--pwdLength == 0)
                        {
                            break;
                        }
                    }
                }
                printf("pwdLength:");
                printf("%d\n", pwdLength);
            }
        }
        if (x > SET_PWD_LOW_X && x < SET_PWD_HIGH_X && y > SET_PWD_LOW_Y && y < SET_PWD_HIGH_Y)
        {
            printf("set password");
            setPassword(pwdLength, pwd);
            if (pwdLength != 0)
            {
                while (1)
                {
                    lcd_draw_bmp("/kjh/RESOURCES/white.bmp", 20 + 65 * (pwdLength - 1), 185);
                    if (--pwdLength == 0)
                    {
                        break;
                    }
                }
            }
        }
        if (x > EXIT_PWD_LOW_X && x < EXIT_PWD_HIGH_X && y > EXIT_PWD_LOW_Y && y < EXIT_PWD_HIGH_Y)
        {
            return 0;
        }
    }
    return 0;
}

void setPassword(int pwdLength, char *pwd)
{
    if (pwdLength == 0)
    {
        return;
    }
    int pfile = open("/kjh/RESOURCES/password.txt", O_RDWR, O_CREAT);
    lseek(pfile, 0, SEEK_END);
    write(pfile, pwd, pwdLength);
    lseek(pfile, 0, SEEK_END);
    char *gan = "|";
    write(pfile, gan, 1);
    close(pfile);
}

void getInputPwd(int *pwdLength, char *pwd, char num)
{
    if (*pwdLength == 8)
    {
        return;
    }
    pwd[*pwdLength] = num;
    lcd_draw_bmp("/kjh/RESOURCES/xing.bmp", 20 + 65 * (*pwdLength), 185);
    (*pwdLength)++;
}

int verifyPwd(int pwdLength, char *pwd)
{
    if (pwdLength == 0)
    {
        return 0;
    }
    int result = -1;           //result=1:密码正确； result=0:密码错误；result=-1:读取密码文件失败
    int i = 0;                 //用作数组下标
    char pwdFromFile[9];       //密码默认最多8位，第九位是换行符
    int pwdFromFileLength = 0; //读取出来的密码位数,包括换行符
    int readLength = 0;
    //读取密码文件
    int pwdFile = open("/kjh/RESOURCES/password.txt", O_RDONLY);
    if (pwdFile == -1)
    {
        close(pwdFile);
        return result; //读取密码文件失败
    }
    while (1)
    {
        //读取密码文件中的密码。换行符分隔密码。
        do
        {
            readLength = read(pwdFile, pwdFromFile + pwdFromFileLength, 1);
            if (readLength == 0 || readLength == -1) //0，到文件末尾
            {
                result = 0;
                close(pwdFile);
                return result;
            }
            pwdFromFileLength++;
        } while (pwdFromFile[pwdFromFileLength - 1] != '|');

        //先比较密码位数
        if (pwdFromFileLength - 1 != pwdLength) //-1是除去换行符
        {
            pwdFromFileLength = 0;
            continue;
        }
        else
        {
            //循环比较两个数组中的字符
            while (i < pwdLength)
            {
                if (pwd[i] != pwdFromFile[i])
                {
                    break;
                }
                i++;
            }
            if (i == pwdFromFileLength - 1)
            {
                result = 1; //密码正确
                close(pwdFile);
                return result;
            }
            else
            {
                i = 0;
                pwdFromFileLength = 0;
                continue;
            }
        }
    }
}

/****************************************password*********************/

/***************************photo***********************/
int filter(const struct dirent *file_info){
    int length = 0;
    while(file_info->d_name[length]){
        length ++;
    }
    char* last_four_character = file_info->d_name + length - 4;
    if(strcmp(last_four_character,".bmp") == 0){
        return 1;
    }
    return 0;
}
int photo(){
    struct dirent **namelist;
    int n;
    n = scandir("/kjh/PHOTO/PICTURES/", &namelist, filter, alphasort);
    if (n < 0)
        perror("scandir");
    // else {
    //     while (n--) {
        
    //         printf("%d:%s\n", n,namelist[n]->d_name);
    //         free(namelist[n]);
    //     }
    //     free(namelist);
    // }
    int count = 0;
    char path_picture[256] ;
    int pointer_picture;
    int x, y;
    char* path_return = "/kjh/PHOTO/PICTURES_SYSTEM/return.bmp";

    do{
        pointer_picture = (int)fabs(count) % n;
        sprintf(path_picture,"/kjh/PHOTO/PICTURES/%s",namelist[pointer_picture]->d_name);
        lcd_draw_bmp(path_picture,0,0);
        lcd_draw_bmp(path_return,0,0);
        get_x_y(&x,&y);
        if(x>400){
            count++;
        }else if(x>60 && x < 400 || x<60 && y > 45){
            count--;
        }else{
            return 0;
    }
    } while(1) ;
}
/**
 * 以上是粗略的相册模块，有很多不足：
 * 第一，draw_bmp函数封装了打开和关闭的过程，每画一张照片都要打开和关闭资源。
 *      改进方法：打开和关闭资源不要封装在一个函数，封装应该是：打开，处理，关闭
 * 第二，没有实现转场效果，所以函数要再加个功能draw_bmp(path, 转场)，当然，这个也只是
 *      如何显示一张图片。还可以连续画多帧实现动态效果。
 *      此外，还应该有颜色变化。初步想法是初始态到终止态步步增长（多帧显示)
 * 第三，如果可以，还可以加上音效
*/
/***************************photo***********************/


/********************music********************/
int myfilter_mp3(const struct dirent *filename) 
{
    size_t len;
    len = strlen(filename->d_name);  //strlen（）获取字符串长度
    if (len >= 4
        && filename->d_name[len - 4] == '.'
        && filename->d_name[len - 3] == 'm'
		&& filename->d_name[len - 2] == 'p'
        && filename->d_name[len - 1] == '3')
        return 1;
 
    return 0;
}
int flag = -1;
      int i =0;
      void *func_mp3(void* arg)
      {
		   int pid = *(int*)arg;
		  if(pid = 1)
		  {
			  if(flag=-1)
			  {
				  system("mplayer /zjf/music/deskmate.mp3");
				  lcd_draw_bmp("/zjf/music/pause.bmp",300, 280);
			  }
			  else
			  {
				  system("pause");
				  lcd_draw_bmp("/zjf/music/start.bmp",300, 280);
			  }
			  flag = -1 * flag;
			  
		  }
		  if(pid = 2)
		  {
			  i++;
			  if(i>2)
			  {
				  i = 0;
			  }
			  system("killall -9 mplayer");
			  system("mplayer /zjf/music/alive.mp3");
			  
			  
		  }
		  if(pid = 3)
		  {
			  i--;
			  if(i<0)
			  {
				  i = 2;
			  }
			  system("killall -9 mplayer");
			  system("mplayer /zjf/music/seven.mp3");
			  			  
		  }

	
      }
int music(){
    lcd_draw_bmp("/zjf/music.bmp",0, 0);
	int x,y;
    do{
		get_x_y(&x,&y);
		if(x>0 && x<80&& y>0 && y<60)
		{
			printf("return");
			lcd_draw_bmp("/kjh/PHOTO/PICTURES_SYSTEM/home.bmp",0, 0);
			return 0;
		}
		
		if(x>300 && x<500&& y>280 && y<480)
		{
			pthread_t pid = 1;
			int a = 1;
            pthread_create(&pid,NULL,func_mp3,&a);
		}
		
		if(x>500 && x<700&& y>280 && y<480)
		{
			pthread_t pid = 2;
			int a = 2;
            pthread_create(&pid,NULL,func_mp3,&a);
		}
		
		if(x>100 && x<300&& y>280 && y<480)
		{
			pthread_t pid = 3;
			int a = 3;
            pthread_create(&pid,NULL,func_mp3,&a);
		}
		
	}while(1);  		

}
/********************music********************/

int vedio(){
    printf("vedio\n");
}
int main(){
    ts_init();
    lcd_open("/dev/fb0");
    // draw_bmp(path) 背景
    int x, y;//获取点击坐标


    int correct = password();
    if(correct == 0){
        return -1;
    }

    while(1){
    lcd_draw_bmp("/kjh/RESOURCES/home.bmp",0,0);    
    get_x_y(&x, &y);
    if(x>CAMERA_LOW_X && x<CAMERA_HIGH_X && y>CAMERA_LOW_Y && y<CAMERA_HIGH_Y ){
        camera();
    }
    else if(x>MUSIC_LOW_X && x<MUSIC_HIGH_X && y>MUSIC_LOW_Y  && y<MUSIC_HIGH_Y ){
        music();
    }
    else if(x>PHOTO_LOW_X && x<PHOTO_HIGH_X && y>PHOTO_LOW_Y && y<PHOTO_HIGH_Y ){
        photo();
    }
    else if(x>VEDIO_LOW_X && x<VEDIO_HIGH_X && y>VEDIO_LOW_Y && y<VEDIO_HIGH_Y ){
        vedio();
    }
    else if(x>RETURN_LOW_X && x < RETURN_HIGH_X && y>RETURN_LOW_Y&& y<RETURN_HIGH_Y){
        break;
    }

    }

    ts_uinit();
    close_lcd();
}
