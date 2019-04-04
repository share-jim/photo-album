/*
 *		jpg图片解析
 *
 *
 *
 ***************************************************************/


#include <stdio.h>
#include <config.h>
#include "jpeglib.h"
#include <string.h>
#include <fb.h>
#include <stdlib.h>


struct my_error_mgr {
  struct jpeg_error_mgr pub;	/* "public" fields */

  //jmp_buf setjmp_buffer;	/* for return to caller */
};
typedef struct my_error_mgr * my_error_ptr;



//错误处理函数
METHODDEF(void)																	//自定义的错误处理函数
my_error_exit (j_common_ptr cinfo)
{
  //my_error_ptr myerr = (my_error_ptr) cinfo->err;

  //(*cinfo->err->output_message) (cinfo);							

  //longjmp(myerr->setjmp_buffer, 1);
  
  fprintf(stderr,"my_error_exit\n");
}

/*
 *功能    :判断jpg图片
 *参数    :图片的文件名
 *返回值：是返回0，不是返回1，错误返回-1
 */
int is_jpg(const char *pathname)
{
	int ret=-1;
	FILE *file=NULL;
	char buffer[2]={0};
	//打开文件
	file = fopen(pathname,"rb");
	if(NULL==ret)
	{
		fprintf(stderr,"open file error\n");
		return -1;
	}
	
	//读文件头信息	
	fread(buffer,sizeof(char),sizeof(buffer),file);
	debug("buffer_0=%x,buffer_1=%x\n",buffer[0],buffer[1]);
	//判断前两个字节
	if(!((buffer[0]==0xff)&&(buffer[1]==0xd8)))
	{
		fclose(file);
		return 1;
	}
	
    fseek(file,-2,SEEK_END);
	
	fread(buffer,sizeof(char),sizeof(buffer),file);	
	debug("buffer_end-2=%x,buffer_end-1=%x\n",buffer[0],buffer[1]);
    //判断末尾两个字节
	if(!((buffer[0]==0xff)&&(buffer[1]==0xd9)))
	{
		fclose(file);
		return 1;
	}
	
	fclose(file);
	return 0;
}


/*
 *功能    :解析jpg图片
 *参数    :图片的结构体(包含宽高,bpp,图片数据)
 *返回值：成功返回0，失败返回-1
 */
static int fb_jpg_Resolve2(struct pic_info *pPic)
{
	 
  struct jpeg_decompress_struct cinfo;
  struct my_error_mgr jerr;

  FILE * infile;		    //源文件指针
 //JSAMPARRAY buffer=NULL;	//解码出图像数据的行缓冲
  char *buffer=NULL;			                  //解决图像部分黑屏关键之1
  int row_stride;		    //图像一行的字节数

  //打开文件
  if ((infile = fopen(pPic->pathname, "rb")) == NULL)       //rb 读二进制文件
  {								
    fprintf(stderr, "can't open %s\n", pPic->pathname);
    return -1;
  }

  //第1步，绑定错误处理程序
  cinfo.err = jpeg_std_error(&jerr.pub);
  jerr.pub.error_exit = my_error_exit;						//实例化错误处理函数
  
  //第2步，给解码器申请内存和数据结构的初始化
  jpeg_create_decompress(&cinfo);												

  //第3步，将解码器结构体与源图片相关联
  jpeg_stdio_src(&cinfo, infile);												

  //第4步，读头信息处理
  jpeg_read_header(&cinfo, TRUE);

  //第5步,开始解码
  jpeg_start_decompress(&cinfo);
  //一行的字节数，cinfo.output_components=bpp/8 
  row_stride = cinfo.output_width * cinfo.output_components;

  debug("jpg picture size:%dx%d,bp:%d\n",cinfo.output_width,cinfo.output_height,cinfo.output_components*8);
  //第6步,申请内存，用于存放一行的解码后的图片数据
  //buffer = (*cinfo.mem->alloc_sarray)((j_common_ptr) &cinfo, JPOOL_IMAGE, row_stride, 1);
  buffer=(char *)malloc(row_stride);		     //解决图像部分黑屏关键之2
  if(NULL==buffer)
  {
  	fprintf(stderr,"men buffer error\n");
	return -1;
  }
 	
  //第7步,循环读取一行的图片数据到buffer中
  while (cinfo.output_scanline < cinfo.output_height)
  {
    //jpeg_read_scanlines(&cinfo, buffer, 1);								   
	jpeg_read_scanlines(&cinfo, &buffer, 1);	//解决图像部分黑屏关键之3

	//将解码完的一行放到图片的数据结构中，
	//pPic->pdate起始地址，cinfo.output_scanline当前的行数*row_stride每行的字节数=偏移量
	//使用前注意给结构体实例化一段内存给pdate
	memcpy(pPic->pdate+(cinfo.output_scanline-1)*row_stride,buffer,row_stride);
  }

  //第8步，清理工作，结束解码，释放内存，关闭文件
  jpeg_finish_decompress(&cinfo);
  jpeg_destroy_decompress(&cinfo);
  fclose(infile);

  //图片显示
  pPic->width=cinfo.output_width;
  pPic->height=cinfo.output_height;
  pPic->bpp=cinfo.output_components*8;
  fb_draw_picture_jpg(pPic);

  return 0;
}

/*
 *功能：对外使用的解码和显示jpg图片的接口，复杂的的数据结构的部分实例化
 *参数：jpg图片pathname
 *返回值：正确返回1，错误返回0
 */
int display_jpg(const char *filename)
{
	struct pic_info picture;
	int ret=-1;
	picture.pathname=filename;
	picture.pdate=pic_buf;
	ret = is_jpg(filename);
	if(ret!=0)
	{	
		return -1;
	}
	fb_jpg_Resolve2(&picture);
	return 0;

}






























