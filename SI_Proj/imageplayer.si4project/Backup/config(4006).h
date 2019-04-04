#ifndef _CONFIG_H_
#define _CONFIG_H_

//#define DEBUG             //DEBUG宏的开关之方式1,方式2在makefile中
/**********************宏定义*******************/
#ifdef DEBUG
#define debug(...)                                                      \
        {                                                               \
            fprintf(stderr, "[debug][%s:%s:%d] ",                     \
                    __FILE__, __FUNCTION__, __LINE__);                  \
            fprintf(stderr, __VA_ARGS__);                               \
        }
#else	
#define debug(...) 
#endif

#define picture_max_resolution (1024*600)
#define picture_buff_size (picture_max_resolution*3)


/********************结构体*******************************/
typedef struct pic_info 
{
	char *pathname;       //图片的路径+文件名
	unsigned int size;	  //图片的尺寸
	unsigned int offsize; //图片的宽
	unsigned int width;	  //图片的高
	unsigned int height;  //图片的有效数据
	unsigned int bpp;
	char *pdate;

}pic_info;


/********************函数原型********************************/
int fb_jpg_Resolve2(struct pic_info *pPic);





#endif







