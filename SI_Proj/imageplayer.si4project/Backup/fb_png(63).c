/*
 *		png图片解析
 *
 *
 *
 ***************************************************************/

#include <stdio.h>
#include <config.h>

#include <png.h>
#include <zlib.h>


#define PNG_BYTES_TO_CHECK 4

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
   
   /*//查看文件头标识
    int i=0;
    for(i=0;PNG_BYTES_TO_CHECK>i;i++)
   	{
   		printf("%x ",buf[i]);
   	}
	*/
	//对比头信息与png头标识，判断是否位png图
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
	int bit_depth, color_type, interlace_type;

	//打开文件
	 if ((file = fopen(pPic->pathname, "rb")) == NULL)
      return -1;

	//1.相关数据结构体的初始化
	 //png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING,
     // png_voidp user_error_ptr, user_error_fn, user_warning_fn);
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
	//png_read_png(png_ptr, info_ptr, png_transforms, NULL);
	png_read_png(png_ptr, info_ptr, PNG_TRANSFORM_EXPAND | PNG_TRANSFORM_STRIP_ALPHA, 0);

	//5.相关图片信息测试
	color_type=info_ptr->color_type;
	debug("color_type:%d",color_type);

	pPic->width =info_ptr->width;
	pPic->height=info_ptr->height;
	pPic->bpp   =info_ptr->pixel_depth;

	debug("wdith=%d,height=%d,bpp=%d\n",info_ptr->width,info_ptr->height,info_ptr->pixel_depth);
	
	return 0;
}


int display_png(const char *filename)
{
	struct pic_info picture;
	int ret=-1;
	picture.pathname=filename;
	picture.pdate=pic_buf;
	ret = is_png(&picture);
	if(ret!=0)
	{	
		debug("not png picture\n");
		return -1;
	}
	debug("png picture\n");
	//fb_png_Resolve2(&picture);
	return 0;

}











