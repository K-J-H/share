#include "../common/draw_bmp.h"

int main(int argc, char* argv[]){
    int i;
    for(i = 0; i < 90; i++){
        draw_bmp_rotation(argv[1],i);
        usleep(atoi(argv[2]));
    }
}