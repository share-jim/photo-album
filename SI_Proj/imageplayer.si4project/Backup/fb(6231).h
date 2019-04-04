#include <config.h>

 
 // 宏定义
#define FBDEVICE	"/dev/fb0"
 
 // 旧开发板
 //#define WIDTH	 800 
 //#define HEIGHT		 480
 // 新开发板
#define WIDTH		1024	
#define HEIGHT		600
 
 
#define WHITE		0xffffffff			// test ok
#define BLACK		0x00000000
#define RED			0xffff0000
#define GREEN		0xff00ff00			// test ok
#define BLUE		0xff0000ff			
 
 
#define GREENP		0x0000ff00			// 一样，说明前2个ff透明位不起作用
 
 
 // 函数声明
 
 int fb_open(void);
 void fb_close(void);

 void fb_draw_back(unsigned int width, unsigned int height, unsigned int color);
 void fb_draw_line(unsigned int color);
 
 #if 0
 void fb_draw_picture1(void);
 void fb_draw_picture2(void);
 void fb_draw_picture_little1(void);
 void fb_draw_picture_little2(void);
 void fb_draw_picture_litAny1(int x0,int y0);
 void fb_draw_picture_litAny2(int x0,int y0);
 void fb_draw_picture_OverRange1(int x0,int y0);
 void fb_draw_picture_OverRange2(int x0,int y0);
 void fb_draw_picture_bmp(const char *pbmp);
 void fb_draw_picture_bmp2(const char *pbmp);
 #endif
 
 void fb_draw_picture_bmp3(const struct pic_info *pPic);
 void fb_draw_picture_jpg(const struct pic_info *pPic);








