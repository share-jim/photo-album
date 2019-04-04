#include <stdio.h>
#include <unistd.h>

#include "fb.h"
#include <config.h>

char pic_buf[picture_buff_size];

int main(void)
{
	int ret = 0;
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
/*
	ret=display_jpg("456.jpg");
	if(ret==-1)
	{
		fprintf(stderr,"dispaly jpg error\n");
	}

	sleep(3);
	
	ret=display_bmp("456.bmp");
	if(ret==-1)
	{
		fprintf(stderr,"dispaly jpg error\n");
	}
*/	
	ret=display_png("456.png");
	if(ret==-1)
	{
		fprintf(stderr,"dispaly png error\n");
	}
	debug("version_15\n");
	fb_close();
	
	return 0;
}































