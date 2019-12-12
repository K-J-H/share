#include "font.h"

int main()
{
	Init_Font();
	
	Clean_Area(0,0,800,480,0x00FFFFFF);

	Display_characterX(100,100,"天王盖地虎",0x00000000,2);
	
	UnInit_Font();
	
	return 0;
}
//编译命令：arm-linux-gcc test.c libfont.a -o arm_font -lfont