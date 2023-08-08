#pragma once

#include <easyx.h>
#include "painter.h"

// ����һ������ͼ�����
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

    int m_height; // ͼƬ(����)�߶�
    int m_width; // ͼƬ(����)���
    int m_btnIdx; // ����ѡ��İ�ť
    int m_focusx, m_focusy; // ��¼��ɫѡ��״̬��λ��
    int m_size; // ��¼�����ͺ�
    IMAGE* m_pimg;
    COLORREF m_focuscolor;
    TButton button[30]; // Ԥ����ʮ����ť
};