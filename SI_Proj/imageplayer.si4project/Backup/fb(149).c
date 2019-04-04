
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

#include "fb.h"

 // 全局变量
 unsigned int *pfb = NULL;

 int fbfd = -1;
 
 int fb_open(void)
 {
	 int ret = -1;
	 
	 
	 struct fb_fix_screeninfo finfo = {0};
	 struct fb_var_screeninfo vinfo = {0};
	 
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
	 unsigned int x, y;
	 
	 for (x=50; x<600; x++)
	 {
		 *(pfb + 200 * WIDTH + x) = color;
	 }
 }
 



























