#include "../common/draw_bmp.h"
/**
 * 
*/
int main(int argc, char* argv[]){
    // Info_bmp info_bmp;
    // int fd_bmp = open_bitmap(argv[1]);
    // read_bmpinfo(fd_bmp,&info_bmp);
    // int length = info_bmp.length;
    // int high = info_bmp.high;
    // int buff[length*high];
    // // int *p = &buff[0][0];
    // read_bmp(fd_bmp, buff,length,high);
    // open_lcd();
    // int i, j;
    // for(i = 0; i < length; i ++){
    //     for(j = 0; j < high; j ++){
    //         draw_point(i,j,buff[j*length+i]);
    //     }
    // }
    // close_lcd();
    // close_bitmap(fd_bmp);
    draw_bmp_offset(argv[1],0,0);
}