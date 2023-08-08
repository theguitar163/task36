// 任务：基于点阵图像的绘图板。
// 点阵图又称位图或栅格图，整个图像由 M 行、N 列的像素点构成。
// 在点阵图里画一条线，会引起若干像素颜色变化。
// 点阵图只存储每个像素的颜色，不记录引起像素颜色改变的是直线还是圆。
// 1.实现一个基于点阵图象的绘图板。需要实现的功能包括：
//   选择颜色、画直线、画圆、画矩形、保存图片、加载图片。
//   保存和加载图片不考虑多种尺寸的情况，只处理一种固定的尺寸即可。
// 2.在该任务的基础上，增加功能：
//   鼠标在图像上选取一个矩形区域，然后可以对该区域应用图像滤镜，包括：
//   马赛克效果、高斯模糊效果、灰度效果、黑白二值图效果等。

#include <graphics.h>
#include <stdio.h>
#include <conio.h>
#include <cmath>
#include <cstdlib>
#include <commdlg.h>
#include "controls.h"
#include "algorithm.h"

COLORREF chooseColor(HWND hwnd)
{
    CHOOSECOLOR cc;                 // common dialog box structure 
    static COLORREF acrCustClr[16]; // array of custom colors 
//    HWND hwnd;                      // owner window
    HBRUSH hbrush;                  // brush handle
    static DWORD rgbCurrent;        // initial color selection

    // Initialize CHOOSECOLOR 
    ZeroMemory(&cc, sizeof(cc));
    cc.lStructSize = sizeof(cc);
    cc.hwndOwner = hwnd;
    cc.lpCustColors = (LPDWORD)acrCustClr;
    cc.rgbResult = rgbCurrent;
    cc.Flags = CC_FULLOPEN | CC_RGBINIT;

    if (ChooseColor(&cc) == TRUE)
    {
        hbrush = CreateSolidBrush(cc.rgbResult);
        rgbCurrent = cc.rgbResult;
    }
    return rgbCurrent;
}

int main()
{
    int Height, Width;
    IMAGE image;
    IMAGE read_img;
    Height = 480;
    Width = 640;
    
    HWND hout = initgraph(800, 600);
    setbkcolor(WHITE);
    setbkmode(TRANSPARENT);
    cleardevice();

    TPanel panel;
    TButton buttons[22] = {
        {btCIRCLE, NULL, RED, 20, 20},
        {btCIRCLE, NULL, GREEN, 20, 20},
        {btCIRCLE, NULL, BLUE, 20, 20},
        {btCIRCLE, NULL, YELLOW, 20, 20},
        {btCIRCLE, NULL, CYAN, 20, 20},
        {btCIRCLE, NULL, BROWN, 20, 20},
        {btRDRECT, L"画线", LIGHTGRAY, 50, 30},
        {btRDRECT, L"颜色", LIGHTGRAY, 50, 30},
        {btRDRECT, L"矩形", LIGHTGRAY, 50, 30},
        {btRDRECT, L"椭圆", LIGHTGRAY, 50, 30},
        {btRDRECT, L"画笔", LIGHTGRAY, 50, 30},
        {btRDRECT, L"橡皮", LIGHTGRAY, 50, 30},
        {btRDRECT, L"保存", LIGHTGRAY, 50, 30},
        {btRDRECT, L"绘画", LIGHTGRAY, 50, 30},

        {btRDRECT, L"打开图片", LIGHTGRAY, 80, 30},
        {btRDRECT, L"水平镜像", LIGHTGRAY, 80, 30},
        {btRDRECT, L"垂直镜像", LIGHTGRAY, 80, 30},
        {btRDRECT, L"截图", LIGHTGRAY, 80, 30},
        {btRDRECT, L"黑白二值", LIGHTGRAY, 80, 30},
        {btRDRECT, L"高斯模糊", LIGHTGRAY, 80, 30},
        {btRDRECT, L"灰度效果", LIGHTGRAY, 80, 30},
        {btRDRECT, L"马赛克", LIGHTGRAY, 80, 30},
    };

    initPanel(&panel, 90, alBOTTOM);
    for (int i = 0; i < 6; i++) {
        initButton(&buttons[i], 20 + i * 30, 15);
        addButton(&panel, &buttons[i]);
    }
    for (int i = 0; i < 8; i++) {
        initButton(&buttons[i+6], 220 + i * 60, 10);
        addButton(&panel, &buttons[6+i]);
    }

    for (int i = 0; i < 7; i++) {
        initButton(&buttons[14 + i], 20 + i * 90, 50);
        addButton(&panel, &buttons[14 + i]);
    }

    drawPanel(&panel);

    _getch();
    return 0;
}

extern int con;
extern int key;

int main1()
{
    int Height, Width;
    IMAGE* m_img;
    IMAGE* read_img;
    TCHAR szFile[MAX_PATH] = { 0 }; // 存储打开图片的路径
    HWND hout;
    while (true)
    {
        Height = 480;
        Width = 640;
        if (con == 0)// 空白绘图板
        {
            m_img = new IMAGE();
            read_img = new IMAGE();
            hout = initgraph(Width, Height + 60);

            setbkcolor(WHITE);
            cleardevice();
            getimage(m_img, 0, 0, 640, 480);
            getimage(read_img, 0, 0, 640, 480);
            Algorithm myimg(m_img, Width, Height);
            key = 0;
            while (key == 0)
            {
                myimg.message_proce(*m_img);
            }
            delete m_img;
            delete read_img;
            m_img = read_img = NULL;
        }
        if (con == 1)// 插图绘图板
        {
            m_img = new IMAGE();
            read_img = new IMAGE();
            loadimage(read_img, szFile);
            loadimage(m_img, szFile);
            Height = (*m_img).getheight();
            Width = (*m_img).getwidth();
            Algorithm mying(m_img, Width, Height);
            if (Width < 640)
            {
                hout = initgraph(640, Height + 60);
                setbkcolor(WHITE);
                cleardevice();
            }
            if (Height >= 480 && Width >= 640)
            {
                hout = initgraph(Width, Height + 60);
                setbkcolor(WHITE);
                cleardevice();
            }
            putimage(0, 0, m_img);
            key = 0;
            while (key == 0)
            {
                mying.message_proce(*read_img);
            }
            delete m_img;
            delete read_img;
            m_img = read_img = NULL;
        }
    }
    _getch();
    return 0;
}