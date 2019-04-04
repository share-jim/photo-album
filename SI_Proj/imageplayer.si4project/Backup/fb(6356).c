
/*
 *	framebuffer基本操作代码,fb的打开，ioctl获取信息，fb测试显示
 *
 * *********************************************************************/

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <linux/fb.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <unistd.h>

#include <fb.h>
//#include "1024600.h"
//#include "500300.h"


 // 全局变量
 unsigned int *pfb = NULL;

 int fbfd = -1;
 
 int fb_open(void)
 {
	 int ret = -1;
	 
	 
	 struct fb_fix_screeninfo finfo ;
	 struct fb_var_screeninfo vinfo ;
	 
	 // 第1步：打开设备
	 fbfd = open(FBDEVICE, O_RDWR);
	 if (fbfd < 0)
	 {
		 perror("open");
		 return -1;
	 }
	 printf("open %s success.\n", FBDEVICE);
	 
	 // 第2步：获取设备的硬件信息
	 ret = ioctl(fbfd, FBIOGET_FSCREENINFO, &finfo);
	 if (ret < 0)
	 {
		 perror("ioctl");
		 return -1;
	 }
	 printf("smem_start = 0x%lx, smem_len = %u.\n", finfo.smem_start, finfo.smem_len);
	 
	 ret = ioctl(fbfd, FBIOGET_VSCREENINFO, &vinfo);
	 if (ret < 0)
	 {
		 perror("ioctl");
		 return -1;
	 }
	 printf("xres = %u, yres = %u.\n", vinfo.xres, vinfo.yres);
	 printf("xres_virtual = %u, yres_virtual = %u.\n", vinfo.xres_virtual, vinfo.yres_virtual);
	 printf("bpp = %u.\n", vinfo.bits_per_pixel);
 
	 
	 // 修改驱动中屏幕的分辨率
	 vinfo.xres = 1024;
	 vinfo.yres = 600;
	 vinfo.xres_virtual = 1024;
	 vinfo.yres_virtual = 1200;
	 ret = ioctl(fbfd, FBIOPUT_VSCREENINFO, &vinfo);
	 if (ret < 0)
	 {
		 perror("ioctl");
		 return -1;
	 }
	 
	 // 再次读出来检验一下
	 ret = ioctl(fbfd, FBIOGET_VSCREENINFO, &vinfo);
	 if (ret < 0)
	 {
		 perror("ioctl");
		 return -1;
	 }
	 printf("修改过之后的参数：\n");
	 printf("xres = %u, yres = %u.\n", vinfo.xres, vinfo.yres);
	 printf("xres_virtual = %u, yres_virtual = %u.\n", vinfo.xres_virtual, vinfo.yres_virtual);
	 printf("bpp = %u.\n", vinfo.bits_per_pixel);
	 
	 
	 // 第3步：进行mmap
	 unsigned long len = vinfo.xres_virtual * vinfo.yres_virtual * vinfo.bits_per_pixel / 8;
	 printf("len = %ld\n", len);
	 pfb = mmap(NULL, len, PROT_READ | PROT_WRITE, MAP_SHARED, fbfd, 0);
	 if (NULL == pfb)
	 {
		 perror("mmap");
		 return -1;
	 }
	 printf("pfb = %p.\n", pfb);
	 
	 fb_draw_back(WIDTH, HEIGHT, WHITE);
	 fb_draw_line(RED);
	 
 

	 
	 return 0;
 }

 void fb_close(void)
 {
 	 close(fbfd);
 }
 
 
 
 
 void fb_draw_back(unsigned int width, unsigned int height, unsigned int color)
 {
	 unsigned int x, y;
	 
	 for (y=0; y<height; y++)
	 {
		 for (x=0; x<width; x++)
		 {
			 *(pfb + y * WIDTH + x) = color;
		 }
	 }
 }
 
 void fb_draw_line(unsigned int color)
 {
	 unsigned int x;
	 
	 for (x=50; x<600; x++)
	 {
		 *(pfb + 200 * WIDTH + x) = color;
	 }
 }
 
#if 0
void fb_draw_picture1(void)
{
	const unsigned char *pdate = gImage_1024600;
	unsigned int x, y;
	unsigned int cnt = 0;
	
	for(y=0; y<HEIGHT; y++)
		for(x=0; x<WIDTH; x++)
		{
			cnt = y * WIDTH + x;   //当前像素点编号，下面左值和右值计算方式一样
			*(pfb+cnt) = ((pdate[3*cnt+0]<<0 ) | (pdate[3*cnt+1]<<8) | (pdate[3*cnt+2]<<16));   //当前像素点填充的RGB数据
		}

}


void fb_draw_picture2(void)
{
	const unsigned char *pdate = gImage_1024600;
	unsigned int x, y;
	unsigned int cnt = 0;
	unsigned int a = 0;
	
	for(y=0; y<HEIGHT; y++)
		for(x=0; x<WIDTH; x++)
		{
			cnt = y * WIDTH + x;   //当前像素点编号,fb的偏移量,下面左值和右值计算方式不一样
			
			*(pfb+cnt) = ((pdate[a+0]<<0 ) | (pdate[a+1]<<8) | (pdate[a+2]<<16));   //当前像素点填充的RGB数据
			a += 3;             //像素点在数组的移动标号
		}

}


//左上角显示小图片

void fb_draw_picture_little1(void)
{
	const unsigned char *pdate = gImage_500300;
	unsigned int x, y;
	unsigned int cnt1 = 0,cnt2 = 0;
	
	for(y=0; y<300; y++)
		for(x=0; x<500; x++)
		{
			cnt1 = y * WIDTH + x;   //当前像素点编号，标识fb
			cnt2 = y * 500 + x;		//当前像素点编号，标识图像数组
			*(pfb+cnt1) = ((pdate[3*cnt2+0]<<0 ) | (pdate[3*cnt2+1]<<8) | (pdate[3*cnt2+2]<<16));   //当前像素点填充的RGB数据
		}

}

void fb_draw_picture_little2(void)
{
	const unsigned char *pdate = gImage_500300;
	unsigned int x, y;
	unsigned int cnt = 0;
	unsigned int a = 0;
	
	for(y=0; y<300; y++)
		for(x=0; x<500; x++)
		{
			cnt = y * WIDTH + x;   //当前像素点编号,fb的偏移量,下面左值和右值计算方式不一样
			
			*(pfb+cnt) = ((pdate[a+0]<<0 ) | (pdate[a+1]<<8) | (pdate[a+2]<<16));   //当前像素点填充的RGB数据
			a += 3;             //像素点在数组的移动标号
		}

}



//任意位置显示小图片,不考虑边界
void fb_draw_picture_litAny1(int x0,int y0)
{
	const unsigned char *pdate = gImage_500300;
	unsigned int x, y;
	unsigned int cnt1 = 0,cnt2 = 0;
	
	for(y=y0; y < y0+300; y++)
		for(x=x0; x < x0+500; x++)
		{
			cnt1 = y * WIDTH + x;   //当前像素点编号，在fb中
			cnt2 = ( y - y0 ) * 500 + ( x - x0 );		//当前像素点编号，在图像数组中
			*(pfb+cnt1) = ((pdate[3*cnt2+0]<<0 ) | (pdate[3*cnt2+1]<<8) | (pdate[3*cnt2+2]<<16));   //当前像素点填充的RGB数据
		}

}

void fb_draw_picture_litAny2(int x0,int y0)
{
	const unsigned char *pdate = gImage_500300;
	unsigned int x, y;
	unsigned int cnt = 0;
	unsigned int a = 0;
	
	for(y=y0; y<300+y0; y++)
		for(x=x0; x<500+x0; x++)
		{
			cnt = y * WIDTH + x;   //当前像素点编号,fb的偏移量,下面左值和右值计算方式不一样
			
			*(pfb+cnt) = ((pdate[a+0]<<0 ) | (pdate[a+1]<<8) | (pdate[a+2]<<16));   //当前像素点填充的RGB数据
			a += 3;             //像素点在数组的移动标号
		}

}



//超限显示小图片
void fb_draw_picture_OverRange1(int x0,int y0)
{
	const unsigned char *pdate = gImage_500300;
	unsigned int x, y;
	unsigned int cnt1 = 0,cnt2 = 0;
	
	for(y=y0; y < y0+300; y++)
	{
		if(y>=HEIGHT)
		{
			break;
		}
		for(x=x0; x < x0+500; x++)
		{
			if(x>=WIDTH)
			{
				continue;
			}
			cnt1 = y * WIDTH + x;   //当前像素点编号，在fb中
			cnt2 = ( y - y0 ) * 500 + ( x - x0 );		//当前像素点编号，在图像数组中
			*(pfb+cnt1) = ((pdate[3*cnt2+0]<<0 ) | (pdate[3*cnt2+1]<<8) | (pdate[3*cnt2+2]<<16));   //当前像素点填充的RGB数据
		}
	}

}

//超限显示小图片，算法不同
void fb_draw_picture_OverRange2(int x0,int y0)
{
	const unsigned char *pdate = gImage_500300;
	unsigned int x, y;
	unsigned int cnt = 0;
	unsigned int a = 0;
	
	for(y=y0; y<300+y0; y++)
	{
		if(y>=HEIGHT)
		{
			break;
		}
		for(x=x0; x<500+x0; x++)
		{
			if(x>=WIDTH)
			{
				a+=3;		  
				continue;
			}
			cnt = y * WIDTH + x;   //当前像素点编号,fb的偏移量,下面左值和右值计算方式不一样
			
			*(pfb+cnt) = ((pdate[a+0]<<0 ) | (pdate[a+1]<<8) | (pdate[a+2]<<16));	//当前像素点填充的RGB数据
			a += 3; 			//像素点在数组的移动标号
		}
	}

}

//未处理前的图像，镜像倒立
void fb_draw_picture_bmp(const char *pbmp)
{
	const unsigned char *pdate = pbmp;
	unsigned int x, y;
	unsigned int cnt1 = 0,cnt2 = 0;
	
	for(y=0; y<300; y++)
		for(x=0; x<500; x++)
		{

			cnt1 = y * WIDTH + x;   //当前像素点编号，标识fb
			cnt2 = y * 500 + x;		//当前像素点编号，标识图像数组
			*(pfb+cnt1) = ((pdate[3*cnt2+0]<<0 ) | (pdate[3*cnt2+1]<<8) | (pdate[3*cnt2+2]<<16));   //当前像素点填充的RGB数据
		}

}

//扫描方式至下而上，至右向左(相对Img2lcd);
//功能：图像数据翻转180，镜像图
void fb_draw_picture_bmp2(const char *pbmp)
{
	const unsigned char *pdate = pbmp;
	unsigned int x, y;
	unsigned int cnt = 0;
	unsigned int a = (((500*300)-1)*24/8);
	
	for(y=0; y<300; y++)
		for(x=0; x<500; x++)
		{
			cnt = y * WIDTH + x;   //当前像素点编号,fb的偏移量,下面左值和右值计算方式不一样
			
			*(pfb+cnt) = ((pdate[a+0]<<0 ) | (pdate[a+1]<<8) | (pdate[a+2]<<16));   //当前像素点填充的RGB数据
			a -= 3;             //像素点在数组的移动标号
		}

}

#endif

//扫描方式至下而上，至左向右(相对Img2lcd);
//功能：图像数据翻转180，改变扫描方向，BMP正常图
void fb_draw_picture_bmp3(const struct pic_info *pPic)
{		
		const unsigned char *pdate =(const unsigned char *)pPic->pdate;
		unsigned int x, y;
		unsigned int cnt = 0;
		unsigned int ay,a=0;

		if((24!=pPic->bpp)&&(32!=pPic->bpp))
		{
			debug("picture bpp is error\n");
			return;
		}

		//for(y=0,ay=pPic->height; y<pPic->height,ay>0; y++,ay--) // 左侧表达式的循环条件没有影响
		for(y=0,ay=pPic->height;ay>0; y++,ay--)
		{
			for(x=0; x<pPic->width; x++)
			{
				cnt = y * WIDTH + x;   //当前像素点编号,fb的偏移量,下面左值和右值计算方式不一样
				a= 3*(ay * pPic->width + x);
					
				*(pfb+cnt) = ((pdate[a+0]<<0 ) | (pdate[a+1]<<8) | (pdate[a+2]<<16));	//当前像素点填充的RGB数据

			}
		}
		
	
}



//改变fb_draw_picture_bmp3的扫描方向，(图像翻转180)，rgb的顺序，jpg正常图
void fb_draw_picture_jpg(const struct pic_info *pPic)
{		
		const unsigned char *pdate =(const unsigned char *)pPic->pdate;
		unsigned int x, y;
		unsigned int cnt = 0;
		//unsigned int a = (((pPic->height*pPic->width)-1)*pPic->bpp/8);  //翻转180,改动之1
		unsigned int a = 0;

		if((24!=pPic->bpp)&&(32!=pPic->bpp))
		{
			debug("picture bpp is error\n");
			return;
		}

		//for(y=0,ay=pPic->height; y<pPic->height,ay>0; y++,ay--) // 左侧表达式的循环条件没有影响
		for(y=0;y<pPic->height; y++)
		{
			for(x=0; x<pPic->width; x++)
			{
				cnt = y * WIDTH + x;   //当前像素点编号,fb的偏移量,下面左值和右值计算方式不一样	
				*(pfb+cnt) = ((pdate[a+2]<<0 ) | (pdate[a+1]<<8) | (pdate[a+0]<<16));	//当前像素点填充的RGB数据
				//a-=3;													   //翻转180,改动之2
				a+=3;	
			}
		}
		
	
}










