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

image_info images[image_cnt_max];			
int pic_index;

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
	show_images_info();
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






