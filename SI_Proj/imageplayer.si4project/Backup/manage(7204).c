/*
 *
 *   扫描和管理图片文件
 *
 *******************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>

#include <sys/types.h>
 #include <sys/stat.h>

#include "manage.h"
#include <config.h>
#include <fb.h>

#include <fcntl.h>
#include <linux/input.h>

image_info images[image_cnt_max];			//图片集
int pic_index;								//图片集的编号

/*功能：在本函数中递归检索pathname文件夹，将其中所有图片填充到images数组中去
 *参数：指定的目录
 */
void san_picture(const char *pathname)
{	
	
	DIR *dir;
    struct dirent *ptr;
    char base[pathname_len];
	struct stat sta;
	
	//打开目录
    if ((dir=opendir(pathname)) == NULL)
    {
        perror("Open dir error...");
        exit(1);
    }
	//检测所有文件，需要的类型
    while ((ptr=readdir(dir)) != NULL)
    {
        if(strcmp(ptr->d_name,".")==0 || strcmp(ptr->d_name,"..")==0)    ///current dir OR parrent dir
            continue;
		
            memset(base,'\0',sizeof(base));
            strcpy(base,pathname);
            strcat(base,"/");
            strcat(base,ptr->d_name);
			lstat(base,&sta);             

		if(S_ISREG(sta.st_mode))
		{
			//检测图片类型,然后填充到图片的数据结构数组中
			if(!is_bmp(base))
			{	
				strcpy(images[pic_index].pathname,base);
				images[pic_index].type=bmp_type;
			}
			if(!is_jpg(base))
			{	
				strcpy(images[pic_index].pathname,base);
				images[pic_index].type=jpg_type;
			}
			if(!is_png(base))
			{	
				strcpy(images[pic_index].pathname,base);
				images[pic_index].type=png_type;
			}
			pic_index++;
		}
		if(S_ISDIR(sta.st_mode))
		{
			san_picture(base);
		}
            
    }	
}

static void show_images_info(void)
{
	int i=0;
	for(i=0;i<pic_index;i++)
	{

		printf("images[%d].pathname=%s,type=%d\n",i,images[i].pathname,images[i].type); 
	}
}

void show_images(void)
{
	int i=0;
	//show_images_info();
	for(i=0;i<pic_index;i++)
	{
		switch(images[i].type)
		{
		case bmp_type:
			display_bmp(images[i].pathname); break;
		case jpg_type:
			display_jpg(images[i].pathname); break;
		case png_type:
			display_png(images[i].pathname); break;
		default:					
			break;
		}
		sleep(2);
	}
	
}

static void touch_show_images(int i)
{

	switch(images[i].type)
	{
	case bmp_type:
		display_bmp(images[i].pathname); break;
	case jpg_type:
		display_jpg(images[i].pathname); break;
	case png_type:
		display_png(images[i].pathname); break;
	default:					
		break;
	}

}


//实现触摸屏的翻页              //from:input/appinput.c
int touch_PgTurn_images(void)
{
	int fd = -1, ret = -1;
	struct input_event ev;
	int i=0;
	
	// 第1步：打开设备文件
	fd = open(DEVICE_TOUCHSCREEN, O_RDONLY);
	if (fd < 0)
	{
		perror("open");
		return -1;
	}
	
	while (1)
	{
		// 第2步：读取一个event事件包
		memset(&ev, 0, sizeof(struct input_event));
		ret = read(fd, &ev, sizeof(struct input_event));
		if (ret != sizeof(struct input_event))
		{
			perror("read");
			close(fd);
			return -1;
		}

		//判断坐标轴,x轴
		if((ev.type==EV_ABS)&&(ev.code==ABS_X))        //查看input.h相关宏
		{
			//上翻
			if((ev.value<=TOUCH_WIDTH)&&(ev.value>=0)) 
			{
				if(i--<=1)
				{
					i=pic_index;
				}
			}
			//下翻
			if((ev.value>=WIDTH-TOUCH_WIDTH)&&(ev.value<=WIDTH)) 
			{
				if(i++>=pic_index)
				{
					i=0;
				}
			}
			touch_show_images(i-1);
		}
	/*	
		// 第3步：解析event包，才知道发生了什么样的输入事件
		printf("-------------------------\n");
		printf("type: %hd\n", ev.type);
		printf("code: %hd\n", ev.code);
		printf("value: %d\n", ev.value);
		printf("\n");
	*/
	}
	
	
	// 第4步：关闭设备
	close(fd);
	
	return 0;
}






