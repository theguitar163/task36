#pragma once

#include <easyx.h>
#include "controls.h"

// 定义一个处理图像的类
class Algorithm
{
public:
    Algorithm(IMAGE* img, int width, int heigth);
    void per_img(IMAGE read_img);
    void ver_img(IMAGE read_img);
    void draw_sence();
    void Gray();
    void Black_White();
    void Gauss();
    void getGaussianArray();
    void message_proce(IMAGE read_img);
    void drawFocus();

    int Height; // 图片(画板)高度
    int Width; // 图片(画板)宽度
    int i; // 控制选择的按钮
    int bluex, bluey; // 记录蓝色选中状态的位置
    int size; // 记录画笔型号
    double** Gaussian_Ker; // 高斯滤盒
    IMAGE* pimg;
    COLORREF bluecolor;
    TButton button[30]; // 预置三十个按钮
};