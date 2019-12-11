#include<stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


int main(){
    int file1 = open("/mnt/hgfs/share/day1/1.txt",O_RDWR);
    int file2 = creat("/mnt/hgfs/share/day1/2.txt",0777);
    printf("1.txt fd:%d\t2.txt fd:%d\n",file1,file2);
    int bytes = lseek(file1,0,SEEK_END); 
    printf("bytes in 1.txt:\t%d\n",bytes);
    lseek(file1,0,SEEK_SET);
    char buff[bytes];//define a buffer to store 1.txt
    if(bytes != read(file1,buff,bytes)){
	printf("read fail!\n");
	return ;
    }
    write(file2,buff,bytes);
}
