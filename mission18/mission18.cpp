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

#include <stdio.h>
#include <conio.h>
#include <easyx.h>
#include <graphics.h>
#include "painter.h"
#include "event.h"

TButton buttons1[] = {
    {bsRDRECT, btDEFAULT, L"打开", LIGHTGRAY, 40, 20, &onLoadImage},
    {bsRECT,   btCOLOR, L"线",  BLACK, 20, 20, &onChoosePenColor, bgPENCOLOR},
    {bsCIRCLE, btDEFAULT, NULL, RED, 20, 20, &onSetPenColor, bgPENCOLOR},
    {bsCIRCLE, btDEFAULT, NULL, GREEN, 20, 20, &onSetPenColor, bgPENCOLOR},
    {bsCIRCLE, btDEFAULT, NULL, BLUE, 20, 20, &onSetPenColor, bgPENCOLOR},
    {bsCIRCLE, btDEFAULT, NULL, YELLOW, 20, 20, &onSetPenColor, bgPENCOLOR},
    {bsCIRCLE, btDEFAULT, NULL, CYAN, 20, 20, &onSetPenColor, bgPENCOLOR},
    {bsCIRCLE, btDEFAULT, NULL, BROWN, 20, 20, &onSetPenColor, bgPENCOLOR},
    {bsRDRECT, btNUM,   L"线宽", LIGHTGRAY, 60, 20, &onChoosePenThickness, bgNONE, 1},

    {bsRDRECT, btDEFAULT, L"画线", LIGHTGRAY, 40, 20, &onSetPenLine, bgPENTYPE},
    {bsRECT, btDEFAULT, L"矩形", LIGHTGRAY, 40, 20, &onSetPenRect, bgPENTYPE},
    {bsCIRCLE, btDEFAULT, L"椭圆", LIGHTGRAY, 60, 20, &onSetPenEllipse, bgPENTYPE},
    {bsRDRECT, btDEFAULT, L"马赛克", LIGHTGRAY, 60, 20, &onSetPenMosaic, bgPENTYPE},
    {bsRDRECT, btDEFAULT, L"橡皮", LIGHTGRAY, 40, 20, &onSetPenEraser, bgPENTYPE},
    {bsRDRECT, btDEFAULT, L"选择", LIGHTGRAY, 40, 20, &onSetPenSelect, bgPENTYPE},
    {bsRDRECT, btDEFAULT, L"撤回", LIGHTGRAY, 40, 20, &onUndoAction},
};
TButton buttons2[] = {
    {bsRDRECT, btDEFAULT, L"保存", LIGHTGRAY, 40, 20, &onSaveImage},
    {bsRECT,   btCOLOR,   L"填", WHITE, 20, 20, &onChooseFillColor, bgFILLCOLOR},
    {bsCIRCLE, btDEFAULT, NULL, RED, 20, 20, &onSetFillColor, bgFILLCOLOR},
    {bsCIRCLE, btDEFAULT, NULL, GREEN, 20, 20, &onSetFillColor, bgFILLCOLOR},
    {bsCIRCLE, btDEFAULT, NULL, BLUE, 20, 20, &onSetFillColor, bgFILLCOLOR},
    {bsCIRCLE, btDEFAULT, NULL, YELLOW, 20, 20, &onSetFillColor, bgFILLCOLOR},
    {bsCIRCLE, btDEFAULT, NULL, CYAN, 20, 20, &onSetFillColor, bgFILLCOLOR},
    {bsCIRCLE, btDEFAULT, NULL, BROWN, 20, 20, &onSetFillColor, bgFILLCOLOR},
    {bsRDRECT, btBOOL,  L"填充", LIGHTGRAY, 60, 20, &onSetFill, bgNONE, 0},
    {bsRDRECT, btDEFAULT, L"截图", LIGHTGRAY, 40, 20, &onSaveClip},
    {bsRDRECT, btDEFAULT, L"水平镜像", LIGHTGRAY, 60, 20, &onHorizontalMirrorImage},
    {bsRDRECT, btDEFAULT, L"垂直镜像", LIGHTGRAY, 60, 20, &onVerticalMirrorImage},
    {bsRDRECT, btDEFAULT, L"模糊", LIGHTGRAY, 40, 20, &onGaussImage},
    {bsRDRECT, btDEFAULT, L"黑白", LIGHTGRAY, 40, 20, &onBlackWhiteImage},
    {bsRDRECT, btDEFAULT, L"灰度", LIGHTGRAY, 40, 20, &onGrayImage},
    {bsRDRECT, btDEFAULT, L"清屏", LIGHTGRAY, 40, 20, &onClearCanvas},
};

int main()
{
    HWND hwnd;
    hwnd = initgraph(800, 600);
    setbkcolor(WHITE);
    setbkmode(TRANSPARENT);
    cleardevice();

    TPanel panel;
    TPainter painter;
    initPainter(&painter, hwnd, &panel, 90, alBOTTOM);

    setButtonPos(&buttons1[0], 40, 15);
    addButton(&panel, &buttons1[0]);
    for (int i = 1; i < sizeof(buttons1)/sizeof(buttons1[0]); i++) {
        addButton(&panel, &buttons1[i], 10, adRIGHT);
    }

    setButtonPos(&buttons2[0], 40, 50);
    addButton(&panel, &buttons2[0]);
    for (int i = 1; i < sizeof(buttons2)/sizeof(buttons2[0]); i++) {
        addButton(&panel, &buttons2[i], 10, adRIGHT);
    }
    initButtonGroup(&panel);

    Run(&painter);

    _getch();
    return 0;
}

