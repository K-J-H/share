       #include <sys/types.h>
       #include <sys/stat.h>
       #include <fcntl.h>
        #include<stdio.h>
/**
 * p:指向一行像素
 * value:要初始化的值
 * begin：初始化开始
 * count：初始化多少个
 * 表示从begin开始，初始化count个值为value的数组
*/
int initialize(int *p , int value, int begin, int count){
    int i = 0;
    while(i <count){
        p[begin+i] = value;
        i++;
    }
    return i;
} 

/**
 * *p：对应一个像素
 * n:有多少列
 * value:每列对应的ARGB
 * dis:每列的像素个数
 * 给定指针，分成n列，每一列的像素由dis，
*/
int inin_column(int *p, int n, int* value, int dis){
    int i = 0;
    for(i; i < n; i++){
        initialize(p,value[i],i*dis,dis);
    }
    return 1;
}

int main(){
    int fd = open("/dev/fb0",O_RDWR);
    if(fd == -1){
        printf("open fail!\n");
    }
    int  pixol[800];
    int value[3]={0x00ff0000,0x0000ff00,0x000000ff};
    inin_column(pixol,3,value,200);//800/3 ok; 
    printf("%02x\n%02x\n%02x\n",pixol[0],pixol[268],pixol[799]);
    int i = 480;
    printf("%d\n", write(fd,pixol,sizeof(pixol)));
    while(i > 0){
        write(fd,pixol,sizeof(pixol));
        i--;
    }
}