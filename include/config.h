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

// 我们规定最大支持1024*600这么大的图片，BPP最多24
#define picture_max_resolution (1024*600)
#define picture_buff_size (picture_max_resolution*3)

													
#define DEVICE_TOUCHSCREEN		"/dev/input/event2" // 定义系统中的触摸屏设备的设备名
#define TOUCH_WIDTH             200                 //触摸屏接触限制宽度

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
/**********************全局变量声明***************************/
extern char pic_buf[picture_buff_size];



/********************函数原型********************************/

int display_bmp(const char *filename);
int display_jpg(const char *filename);
int display_png(const char *filename);

int is_bmp(const char *pathname);
int is_jpg(const char *pathname);
int is_png(const char *pathname);

void san_picture(const char *pathname);
void show_images(void);

int touch_PgTurn_images(void);



#endif







