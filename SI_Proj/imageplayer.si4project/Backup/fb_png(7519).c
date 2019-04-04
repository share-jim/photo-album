/*
 *		png图片解析
 *
 *
 *
 ***************************************************************/

#include <stdio.h>
#include <config.h>

#include <fb.h>

#include <png.h>
//#include <zlib.h>
#include <pngstruct.h>   
#include <pnginfo.h>  
	//注意头文件包含数据，png_structp，png_infop和这两个结构体有关



#define PNG_BYTES_TO_CHECK 4    //4或8都可以

/*
 *功能    :判断jpg图片
 *参数    :图片的文件名
 *返回值：是返回0，不是返回1，错误返回-1
 */
static int is_png(const struct pic_info *pPic)
{
	 char buf[PNG_BYTES_TO_CHECK];
	 FILE *file=NULL;
	 

   //打开文件
   if ((file = fopen(pPic->pathname, "rb")) == NULL)
      return -1;

   //读取文件头信息
   if (fread(buf, 1, PNG_BYTES_TO_CHECK, file) != PNG_BYTES_TO_CHECK)
      return -1;
   
   /*//调试，查看文件头标识
    int i=0;
    for(i=0;PNG_BYTES_TO_CHECK>i;i++)
   	{
   		printf("%x ",buf[i]);
   	}
	*/
	//对比头信息与png头标识，判断是否位png图，内部使用memcmp
   //return(!png_sig_cmp(buf, (png_size_t)0, PNG_BYTES_TO_CHECK));
   return(png_sig_cmp(buf, (png_size_t)0, PNG_BYTES_TO_CHECK));
}


/*
 *功能    :解析jpg图片
 *参数    :图片的结构体(包含宽高,bpp,图片数据)
 *返回值：成功返回0，失败返回-1
 */
static int fb_png_Resolve2(struct pic_info *pPic)
{
	png_structp png_ptr;
    png_infop info_ptr;
	FILE *file=NULL;
	int color_type;
	png_bytep* row_pointers;
	int i=0,j=0;
	unsigned long len=0;
	int pos=0;

	//打开文件
	 if ((file = fopen(pPic->pathname, "rb")) == NULL)
      return -1;

	//1.相关数据结构体的初始化
	 //png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING,
     // png_voidp user_error_ptr, user_error_fn, user_warning_fn); //?
     png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, 0, 0, 0);

  	if (png_ptr == NULL)
  	{
  	  fclose(file);
  	  return -1;
  	}

	info_ptr = png_create_info_struct(png_ptr);
   if (info_ptr == NULL)
   {
      fclose(file);
      png_destroy_read_struct(&png_ptr, NULL, NULL);
      return -1;
   }

   //2.设置错误处理程序
   if (setjmp(png_jmpbuf(png_ptr)))
   {
      png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
      fclose(file);
      return -1;
   }

   //3.绑定文件指针与解码器
    png_init_io(png_ptr, file);

	//4.读取图片数据
	//png_read_png(png_ptr, info_ptr, png_transforms, NULL); //?
	png_read_png(png_ptr, info_ptr, PNG_TRANSFORM_EXPAND | PNG_TRANSFORM_STRIP_ALPHA, 0);

	//5.相关图片信息测试
	color_type=info_ptr->color_type;
	debug("color_type:%d\n",color_type);

	pPic->width =info_ptr->width;
	pPic->height=info_ptr->height;
	pPic->bpp   =info_ptr->pixel_depth;
	len=info_ptr->width*info_ptr->height*info_ptr->pixel_depth/8;
	debug("wdith=%d,height=%d,bpp=%d\n",info_ptr->width,info_ptr->height,info_ptr->pixel_depth);
	//6.获取真正的图像信息
	row_pointers = png_get_rows(png_ptr,info_ptr);         //?
	// 只处理RGB24位真彩色图片，其他格式的图片不管，实际项目要考虑其他情况
	//7.图像数据移动到我们自己的buf中
	if(color_type == PNG_COLOR_TYPE_RGB)
  	{
   		//memcpy(pPic->pData, row_pointers, len);  //图像扭曲
		for(i=0; i<pPic->height; i++)
		{
			for(j=0; j<3*pPic->width; j+=3)
			{
				pPic->pdate[pos++] = row_pointers[i][j+0];		
				pPic->pdate[pos++] = row_pointers[i][j+1];		
				pPic->pdate[pos++] = row_pointers[i][j+2];		
			}
		}
  	}

	//8.清理工作
	png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
	close(file);
	
	return 0;
}


int display_png(const char *filename)
{
	struct pic_info picture;
	int ret=-1;
	picture.pathname=filename;
	picture.pdate=pic_buf;
	//1.判断png
	ret = is_png(&picture);
	if(ret!=0)
	{	
		debug("%s is not png picture\n",filename);
		return -1;
	}
	debug("%s is png picture\n",filename);
	//2.解析png
	fb_png_Resolve2(&picture);
	//3.显示png
	fb_draw_picture_png(&picture);
	return 0;

}











