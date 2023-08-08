#pragma once

#include <easyx.h>
#include "controls.h"

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
    void Gauss();
    void getGaussianArray();
    void message_proce(IMAGE read_img);
    void drawFocus();

    int Height; // ͼƬ(����)�߶�
    int Width; // ͼƬ(����)���
    int i; // ����ѡ��İ�ť
    int bluex, bluey; // ��¼��ɫѡ��״̬��λ��
    int size; // ��¼�����ͺ�
    double** Gaussian_Ker; // ��˹�˺�
    IMAGE* pimg;
    COLORREF bluecolor;
    TButton button[30]; // Ԥ����ʮ����ť
};