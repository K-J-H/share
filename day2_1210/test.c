#include<stdio.h>
#include <limits.h>
int main(){
    unsigned long a = 1;
    unsigned long b = 1;
    unsigned long x , y;
    int c;
    for(;a < 1000;a++){
        for(b = 1; b < 1000; b++){
            x = a*a + b*b;
            y = a*b + 1;
            c = x%y;
            if(c == 0){
                printf("x=%d^2+%d^2=%d\t",a,b,x);
                printf("y=%d*%d+1=%d\t",a,b,y);
                printf("%d/%d=%d\t\n",x,y,x/y);
            }
        }
    }
    // printf("a:%d\tb:%d\t",i,j);
}