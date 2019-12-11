#include "../common/draw_bmp.h"

int main(int argc, char* argv[]){
    int i;
    for(i = 0; i < atoi(argv[4]); i++){
        draw_bmp_rotation(argv[1],atoi(argv[2]),atoi(argv[3]),i);
        //usleep(atoi(argv[4]));
    }
}