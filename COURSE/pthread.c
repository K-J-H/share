#include <stdio.h>
#include <pthread.h>	//线程
#include <unistd.h>

int n = 100;

void *func(void *arg)
{
	printf("INPUT n:");
	scanf("%d",&n);
	printf("INPUT OVER !\n");
}

int main()
{
	pthread_t pid;
	pthread_create(&pid,NULL,func,NULL);
	
	while(n)
	{
		printf("n=%d\n",n);
		n++;
		sleep(1);
	}
}