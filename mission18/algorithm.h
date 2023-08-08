#pragma once

#include <easyx.h>
#include "painter.h"

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
    void getGaussianArray();
    void message_proce(IMAGE read_img);
    void drawFocus();

    int m_height; // 图片(画板)高度
    int m_width; // 图片(画板)宽度
    int m_btnIdx; // 控制选择的按钮
    int m_focusx, m_focusy; // 记录蓝色选中状态的位置
    int m_size; // 记录画笔型号
    IMAGE* m_pimg;
    COLORREF m_focuscolor;
    TButton button[30]; // 预置三十个按钮
};