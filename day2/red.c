       #include <sys/types.h>
       #include <sys/stat.h>
       #include <fcntl.h>

int initialize(int *p,int value, int unit){
    while(unit){
        p[--unit]=value;
    }
    return unit;
}

int main(){
    int fd = open("/dev/fb0",O_RDWR);
    int  pixol[800];
    initialize(pixol,0x00ff0000,800);
    int i = 480;
    while(i > 0){
        write(fd,pixol,800*4);
        i--;
    }
}