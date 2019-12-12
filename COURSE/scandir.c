#include <stdio.h>
#include <dirent.h>
#include <string.h>


//自定义"*.bmp"文件筛选回调函数
int myfilter_jpg(const struct dirent *filename) 
{
    size_t len;
    len = strlen(filename->d_name);  //strlen（）获取字符串长度
    if (len >= 4
        && filename->d_name[len - 4] == '.'
        && filename->d_name[len - 3] == 'b'
		&& filename->d_name[len - 2] == 'm'
        && filename->d_name[len - 1] == 'p')
        return 1;
 
    return 0;
}

int main(int argc,char **argv)
{
	struct dirent **namelist;	//文件名列表
	
	int z;
	//n  代表的是myfilter函数的检测的jpg个数
	int n=scandir("./",&namelist,myfilter_jpg,alphasort);
	printf("n=%d\n",n);
	z=n;
	
	char buf[50];
	while(z--)
	{
		printf("namelist[%d] %s\n",z,namelist[z]->d_name);
		
		bzero(buf,sizeof(buf)):
		sprintf(buf,"/%s",namelist[z]->d_name);	
	}

	return 0;
}