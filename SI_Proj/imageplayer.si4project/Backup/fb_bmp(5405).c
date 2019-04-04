
/*
 *		bmp图片解析
 *
 *
 *
 ***************************************************************/


#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


#include <unistd.h>

#include <stdio.h>

#include "bmp.h"


#define picture_max_resolution (1024*600)
#define fd_buff_size (picture_max_resolution*3)

unsigned char fd_buf[fd_buff_size];


/*参数：图片文件path_name
  返回值: 返回0成功，-1失败
*/

#if 0
int fb_bmp_Resolve(const char *path)
{
	int fd = -1; 
	ssize_t ret = -1;

	unsigned char fd_read_buf[54]={0};
	//1.打开文件并读文件
	fd = open(path,O_RDONLY);
	if(fd<0)
	{
		fprintf(stderr,"open %s is error.\n",path);
		return -1;
	}

	ret = read(fd,fd_read_buf,sizeof(fd_read_buf));
	if(ret!=54)
	{
		fprintf(stderr,"read %s is error.\n",path);   //标准错误输出
		return -1;
	}	

	//2.判断是否是bmp格式
	if((fd_read_buf[0]!='B') | (fd_read_buf[1]!='M'))
	{
		printf("%s is not bmp type picture.\n",path);
		return -1;
	}
	else
	{
		printf("%s is bmp type picture.\n",path);
	}

	//3.解析
	printf("width is %u\n",*((unsigned int *)(fd_read_buf+0x12)));  //%u表示无符号数
	printf("height is %u\n",*((unsigned int *)(fd_read_buf+0x16)));

	//4.处理
	return 0;
}

#endif

//判断是否是bmp图片
int is_bmp(const char *pPATH)
{
	
	int fd = -1; 
	ssize_t ret = -1;

	unsigned char fd_read_buf[2];
	//1.打开文件
	fd = open(pPATH,O_RDONLY);
	if(fd<0)
	{
		fprintf(stderr,"open %s is error.\n",pPATH);
		return -1;
	}
	//2.读取文件头信息
	ret = read(fd,fd_read_buf,sizeof(fd_read_buf));
	if(ret!=54)
	{
		fprintf(stderr,"read %s is error.\n",pPATH);   //标准错误输出
		ret = -1;
		goto close;
	}	

	//3.判断是否是bmp格式
	if((fd_read_buf[0]!='B') | (fd_read_buf[1]!='M'))
	{
		printf("%s is not bmp type picture.\n",pPATH);
		ret = -1;
		goto close;
	}
	else
	{
		printf("%s is bmp type picture.\n",pPATH);
		ret = 0;
	}
	close:
		close(fd);
	return ret;
}

//获取bmp图片的有效信息

int fb_bmp_Resolve2(const char *path)
{
	int fd = -1; 
	ssize_t ret = -1;
	unsigned long len =0;

	ClBitMapFileHeader fheader={0};
	ClBitMapInfoHeader iheader={0};
	unsigned char fd_read_buf[2];

	//1.打开文件
	fd = open(path,O_RDONLY);
	if(fd<0)
	{
		fprintf(stderr,"open %s is error.\n",path);
		return -1;
	}

	//2.读/解析文件头
	read(fd,fd_read_buf,sizeof(fd_read_buf));  //避免字节对齐引发错误
	read(fd,&fheader,sizeof(fheader));	

	printf("picture size:%d\n",fheader.bfSize);
	printf("picture val_info offset:%d\n",fheader.bfOffBits);

	//3.读/解析信息头
	read(fd,&iheader,sizeof(iheader));
	
	printf("picture resolution: %d x %d \n",iheader.biWidth,iheader.biHeight);
	printf("picture bpp:%d\n",iheader.biBitCount);

	//4.读/解析有效信息
	//指针移动到图片的有效信息偏移量处fheader.bfOffBits
	lseek(fd,fheader.bfOffBits,SEEK_SET);  
	//读出的有效信息字节数len
	len = iheader.biWidth*iheader.biHeight*iheader.biBitCount/3; 
	
	read(fd,&fd_buf,len);
	
	//fb_draw_picture_bmp(fd_buf);   //显示有效数据,倒立镜像
	//fb_draw_picture_bmp2(fd_buf);   //显示有效数据，镜像
	fb_draw_picture_bmp3(fd_buf);  	////显示有效数据，原始图像
	
	//5.处理
	close(fd);
	return 0;
}





















