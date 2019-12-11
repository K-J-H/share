#include "../common/draw_bmp.h"
#include "../common/Function.h"
int main(int argc, char* argv[]){
    int color;
    if(argc == 1){
        color = 0x00ffffff;
    }else if(argc == 2){
        color = charToint(argv[1]);
        if(color == -1){
            return -1;
        }
    }else{
        printf("unknown command. try:\ncomman color\n");
    }
    draw_color(color);
    return 0;
}