// 任务：读写 BMP 文件。
// 读取.bmp 图片文件，一个字节一个字节的读取其内容，并将图片逐像素的画出来。
// 关于 BMP 图片的文件格式，请自行搜索。
// 为了简化程序，可以只考虑 24 位色、未压缩的 BMP 图片。
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <easyx.h>
#include <conio.h>

// 文件信息头结构体
typedef struct tagBMPFileHeader {
    //unsigned short bfType;        // 19778，必须是BM字符串，对应的十六进制为0x4d42,十进制为19778，否则不是bmp格式文件
    unsigned int   bfSize;        // 文件大小 以字节为单位(2-5字节)
    unsigned short bfReserved1;   // 保留，必须设置为0 (6-7字节)
    unsigned short bfReserved2;   // 保留，必须设置为0 (8-9字节)
    unsigned int   bfOffBits;     // 从文件头到像素数据的偏移  (10-13字节)
} TBMPFileHeader;

//图像信息头结构体
typedef struct tagBMPInfoHeader {
    unsigned int    biSize;          // 此结构体的大小 (14-17字节)
    long            biWidth;         // 图像的宽  (18-21字节)
    long            biHeight;        // 图像的高  (22-25字节)
    unsigned short  biPlanes;        // 表示bmp图片的平面属，显然显示器只有一个平面，所以恒等于1 (26-27字节)
    unsigned short  biBitCount;      // 一像素所占的位数，一般为24   (28-29字节)
    unsigned int    biCompression;   // 说明图象数据压缩的类型，0为不压缩。(30-33字节)
    unsigned int    biSizeImage;     // 像素数据所占大小, 这个值应该等于上面文件头结构中bfSize-bfOffBits (34-37字节)
    long            biXPelsPerMeter; // 说明水平分辨率，用象素/米表示。一般为0 (38-41字节)
    long            biYPelsPerMeter; // 说明垂直分辨率，用象素/米表示。一般为0 (42-45字节)
    unsigned int    biClrUsed;       // 说明位图实际使用的彩色表中的颜色索引数（设为0的话，则说明使用所有调色板项）。(46-49字节)
    unsigned int    biClrImportant;  // 说明对图象显示有重要影响的颜色索引的数目，如果是0，表示都重要。(50-53字节)
} TBMPInfoHeader;

//24位图像素信息结构体,即调色板
typedef struct tagPixel {
    unsigned char rgbBlue;   //该颜色的蓝色分量  (值范围为0-255)
    unsigned char rgbGreen;  //该颜色的绿色分量  (值范围为0-255)
    unsigned char rgbRed;    //该颜色的红色分量  (值范围为0-255)
    unsigned char rgbReserved;// 保留，必须为0
} TPixel;

//定义显示信息的函数，传入文件头结构体
void showBmpHead(TBMPFileHeader* pf)
{
    printf("BMP文件大小：%dkb\n", pf->bfSize / 1024);
}

//定义显示信息的函数，传入的是信息头结构体
void showBmpInfoHead(TBMPInfoHeader* pi)
{
    printf("位图信息头----------\n");
    printf("位图宽度:%d\n", pi->biWidth);
    printf("位图高度:%d\n", pi->biHeight);
    printf("每个像素的位数:%d\n", pi->biBitCount);
    printf("压缩方式:%d\n", pi->biCompression);
    printf("图像的大小:%d\n", pi->biSizeImage);
}

int main()
{
    TBMPFileHeader fileheader;
    TBMPInfoHeader info;

    FILE* fp;
    if (fopen_s(&fp, "D:\\C语言编程\\mission\\\mission24\\bird.bmp", "rb")!=0) {
        printf("打开文件失败！\n");
        return -1;
    }
    //如果不先读取bifType，根据C语言结构体Sizeof运算规则——整体大于部分之和，从而导致读文件错位
    unsigned short  fileType;
    fread(&fileType, 1, sizeof(unsigned short), fp);
    if (fileType != 0x4d42) {
        printf("非BMP文件！\n");
        return -1;
    }
    // 读取文件头
    fread(&fileheader, 1, sizeof(TBMPFileHeader), fp);
    showBmpHead(&fileheader);
    // 读取信息头
    fread(&info, 1, sizeof(TBMPInfoHeader), fp);
    showBmpInfoHead(&info);

    if (info.biCompression != 0) {
        printf("无法读取压缩BMP文件！\n");
        return -1;
    }

    if (info.biBitCount != 24) {
        printf("只能读取24位BMP文件！\n");
        return -1;
    }
    
    // 由于Windows在进行行扫描的时候最小的单位为4个字节，
    // 所以当图片宽 X 每个像素的字节数 ！ = 4的整数倍时要在每行的后面补上缺少的字节，
    // 以0填充（一般来说当图像宽度为2的幂时不需要对齐）。
    // 位图文件里的数据在写入的时候已经进行了行对齐，也就是说加载的时候不需要再做行对齐。
    // 但是这样一来图片数据的长度就不是：宽 X 高 X 每个像素的字节数了，我们需要计算正确的数据长度：
    int lineByteCnt = (((info.biWidth * info.biBitCount / 8) + 3) >> 2) << 2;

    unsigned char* buff = (unsigned char*)malloc(info.biSizeImage);
    fseek(fp, fileheader.bfOffBits, SEEK_SET);
    fread(buff, 1, info.biSizeImage, fp);
    fclose(fp);
    _getch();

    initgraph(info.biWidth, info.biHeight);

    for (int i = 0; i < info.biHeight; i++) {
        for (int j = 0; j < info.biWidth; j++) {
            unsigned char* p = &buff[i * lineByteCnt + j * info.biBitCount / 8];
            // 注意BMP图片是上下颠倒的，所以需要从下往上输出
            // 每像素连续三个字节为BGR，所以需要调整为RGB
            putpixel(j, info.biHeight - i -1, RGB(*(p + 2), *(p + 1), *p));
        }
    }
    _getch();
    free(buff);
    closegraph();
}