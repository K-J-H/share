#include <stdio.h>
#include <strings.h>	//bzero

int main()
{
	int n=100;
	char buf[100];
	
	bzero(buf,sizeof(buf));
	sprintf(buf,"/photo/test%d.bmp",n);
	
	printf("buf=%s\n",buf);
}