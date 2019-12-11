#include "../common/lcd.h"

#include<stdio.h>


int main(){
    int zhen = 50;
    init();
    if(fd_lcd == -1){
        printf("open file fail.\n");
        return -1;
    }
    int diff[PIXELS_COLUMN][PIXELS_ROW];

    
    int i, j;
    for(i = 0; i < PIXELS_COLUMN; i++){
        for(j = 0; j < PIXELS_ROW; j ++){
            diff[i][j] = color_at_point(i,j)
        }
    }
}