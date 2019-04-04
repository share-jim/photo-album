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
	fb_close();
	
	return 0;
}































