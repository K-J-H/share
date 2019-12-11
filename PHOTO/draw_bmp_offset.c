#include "../common/draw_bmp.h"
#include "../common/Function.h"

int main(int argc, char* argv[]){
    int offset_x;
    int offset_y;
    if(argc == 2){ //user tpye: command bmpPATH, use default offset
        offset_x = 0;
        offset_y = 0;
    }else if(argc == 4){
        offset_x = charToint(argv[2]);
        offset_y = charToint(argv[3]);
    }else{
        printf("unknow command. type command path [off_x off_y]");
        return -1;
    }
    
    draw_bmp_offset(argv[1],offset_x,offset_y);
    return 0;
}