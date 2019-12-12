#include <stdio.h>
#include <dirent.h>
#include <string.h>


//�Զ���"*.bmp"�ļ�ɸѡ�ص�����
int myfilter_jpg(const struct dirent *filename) 
{
    size_t len;
    len = strlen(filename->d_name);  //strlen������ȡ�ַ�������
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
	struct dirent **namelist;	//�ļ����б�
	
	int z;
	//n  �������myfilter�����ļ���jpg����
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