#ifndef _MANAGE_H_
#define _MANAGE_H_

#define pathname_len 255
#define image_cnt_max 100


typedef enum image_type
{
	bmp_type,
	jpg_type,
	png_type,
}image_type;



typedef struct image_info
{
	char pathname[pathname_len];  //图片文件pathname
	image_type type;			  //图片文件类型
}image_info;














#endif

