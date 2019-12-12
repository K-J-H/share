#include <stdlib.h>
#include <stdio.h>

int main()
{
	system("madplay 1.mp3 &");	//播放音乐	&表示把命令扔到后台执行
	
	sleep(10);

	system("killall -STOP madplay");
	
	sleep(10);

	system("killall -CONT madplay");
	
	sleep(10);
	
	system("killall -9 madplay");	//关闭进程
}