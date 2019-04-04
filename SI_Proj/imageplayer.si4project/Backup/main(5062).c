#include <stdio.h>

#include "fb.h"

int main(void)
{
	int ret = 0;
	printf("hello imageplayer!\n");
    ret = fb_open();
	if(ret<0)
	{
		printf("fb open error.\n");
		return -1;
	}
	fb_draw_back(1024,600 , BLUE);
	//fb_draw_picture();
	//fb_draw_picture_little1();
	//fb_draw_picture_little2();
	//fb_draw_picture_litAny1(262,150);  //中间测试
	//fb_draw_picture_litAny2(262,180);
	//fb_draw_picture_litAny2(800,180);   //测试超边界
	//fb_draw_picture_OverRange(800,450);
	//fb_draw_picture_OverRange2(600,400);
	
	//fb_bmp_Resolve("456.bmp");      //注意文件路径
	fb_bmp_Resolve2("456.bmp");
	fb_close();
	
	return 0;
}































