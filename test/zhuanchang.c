#include "../common/BitMap.h"
#include "../common/lcd.h"

/**
 * 实现转场效果其实只需要知道：
 *  1.图片信息
 *      颜色信息buff
 *      长，高。
 *  2.显示屏的长和高（其实也需要知道译码方式）
*/
int (*ZHUANG_CHANG)(int *buff, int length_bmp, int high_bmp, int length_lcd, int high_lcd);

//接下来实现百叶窗
int baiyechuang(int *buff, int length_bmp, int high_bmp, int length_lcd, int high_lcd, int yeshu){
    int i, j, k;

    for(i = 0; i < yeshu; i ++){

    }
}

int draw_zhuanchang(char* path, int mode)