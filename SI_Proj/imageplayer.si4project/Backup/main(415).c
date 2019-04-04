#include <stdio.h>

#include "fb.h"
#include <config.h>

int main(void)
{
	int ret = 0;
	struct pic_info picture;
	
	printf("hello imageplayer!\n");
    ret = fb_open();
	if(ret<0)
	{
		debug("fb open error.\n");
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
	
	//fb_bmp_Resolve();      //注意文件路径

	picture.pathname="456.bmp";
	if(-1!=is_bmp(&picture))
	{
		fb_bmp_Resolve2(&picture);		
	}
	debug("version_11\n");
	fb_close();
	
	return 0;
}































