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
#include "painter.h"
#include "algorithm.h"
#include "imageproc.h"

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
    HWND hwnd;
    hwnd = initgraph(800, 600);
    setbkcolor(WHITE);
    setbkmode(TRANSPARENT);
    cleardevice();

    TButton buttons[] = {
        {bsCIRCLE, btDEFAULT, NULL, RED, 20, 20, &SetPenColor},
        {bsCIRCLE, btDEFAULT, NULL, GREEN, 20, 20, &SetPenColor},
        {bsCIRCLE, btDEFAULT, NULL, BLUE, 20, 20, &SetPenColor},
        {bsCIRCLE, btDEFAULT, NULL, YELLOW, 20, 20, &SetPenColor},
        {bsCIRCLE, btDEFAULT, NULL, CYAN, 20, 20, &SetPenColor},
        {bsCIRCLE, btDEFAULT, NULL, BROWN, 20, 20, &SetPenColor},
        {bsRECT,   btCOLOR, L"颜", LIGHTGRAY, 30, 30, &ChoosePenColor},
        {bsRDRECT, btNUM,   L"线宽", LIGHTGRAY, 80, 30, &ChoosePenThickness},
        {bsRDRECT, btBOOL,  L"填充", LIGHTGRAY, 80, 30, &SetFill},
        {bsRDRECT, btDEFAULT, L"画线", LIGHTGRAY, 50, 30, &SetPenLine},
        {bsRDRECT, btDEFAULT, L"矩形", LIGHTGRAY, 50, 30, &SetPenRect},
        {bsRDRECT, btDEFAULT, L"椭圆", LIGHTGRAY, 50, 30, &SetPenEllipse},
        {bsRDRECT, btDEFAULT, L"马赛克", LIGHTGRAY, 80, 30, &SetPenMosaic},
        {bsRDRECT, btDEFAULT, L"橡皮", LIGHTGRAY, 50, 30, &SetPenEraser},


        {bsRDRECT, btDEFAULT, L"打开", LIGHTGRAY, 80, 30, &LoadImage},
        {bsRDRECT, btDEFAULT, L"保存", LIGHTGRAY, 50, 30, &SaveImage},
        {bsRDRECT, btDEFAULT, L"截图", LIGHTGRAY, 80, 30, &SaveClip},
        {bsRDRECT, btDEFAULT, L"撤回", LIGHTGRAY, 50, 30, &UndoAction},

        {bsRDRECT, btDEFAULT, L"水平镜像", LIGHTGRAY, 80, 30, &HorizontalMirrorImage},
        {bsRDRECT, btDEFAULT, L"垂直镜像", LIGHTGRAY, 80, 30, &VerticalMirrorImage},
        {bsRDRECT, btDEFAULT, L"黑白二值", LIGHTGRAY, 80, 30, &BlackWhiteImage},
        {bsRDRECT, btDEFAULT, L"高斯模糊", LIGHTGRAY, 80, 30, &GaussImage},
        {bsRDRECT, btDEFAULT, L"灰度效果", LIGHTGRAY, 80, 30, &GrayImage},
    };

    TPanel panel;
    TPainter painter;
    initPainter(&painter, hwnd, &panel, 90, alBOTTOM);

    for (int i = 0; i < 6; i++) {
        addButton(&panel, &buttons[i], 10, adRIGHT);
    }
    for (int i = 0; i < 8; i++) {
        addButton(&panel, &buttons[6+i], 10, adRIGHT);
    }

    for (int i = 0; i < 8; i++) {
        initButton(&buttons[14 + i], 20 + i * 90, 50);
        addButton(&panel, &buttons[14 + i]);
    }

    BeginBatchDraw();
    drawPainter(&painter);
    ExMessage m;
    while (true) {
        if (peekmessage(&m, EM_MOUSE | EM_KEY)) {
            // 左键单击判断
            if(m.message == WM_LBUTTONDOWN) {
                if (ptInPainter({ m.x, m.y }, &painter)) {
                    painterClick(&painter, m.x, m.y);
                }
                else
                    buttonClick(&panel, m.x, m.y);
            }
        }
        FlushBatchDraw();
        Sleep(10);
    }
    EndBatchDraw();
    _getch();
    return 0;
}

