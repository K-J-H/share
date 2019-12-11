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

    int i=0;//row 1
    initialize(pixol,0x00ff0000,800);
    while(i++ <160)
    {
        write(fd,pixol,800*sizeof(int));
    }

    initialize(pixol,0x0000ff00,800);
    while(i++ <320)
    {
        write(fd,pixol,800*4);
    }
    
    initialize(pixol,0x000000ff,800);
    while(i++ < 480)
        write(fd,pixol,800*4);
   
}