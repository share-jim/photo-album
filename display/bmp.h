
#ifndef _BMP_H_
#define _BMP_H_

// BMP 文件头
typedef struct  
{  
//    unsigned short    bfType;  			// 2  因为字节对齐问题，屏蔽
    unsigned long    	bfSize; 			// 4 
    unsigned short    	bfReserved1;  
    unsigned short    	bfReserved2;  
    unsigned long    	bfOffBits;  
} ClBitMapFileHeader;

// BMP 信息头
typedef struct  
{  
    unsigned long  		biSize;   
    long   				biWidth;   
    long   				biHeight;   
    unsigned short   	biPlanes;   
    unsigned short   	biBitCount;  
    unsigned long 	 	biCompression;   
    unsigned long  		biSizeImage;   
    long   				biXPelsPerMeter;   
    long   				biYPelsPerMeter;   
    unsigned long  	 	biClrUsed;   
    unsigned long   	biClrImportant;   
} ClBitMapInfoHeader;  


#endif


