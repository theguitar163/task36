#pragma once

#include <easyx.h>

#define CELL_SIZE 10//С����߳�

#define COLOR_ROAD WHITE       // ������ɫ
#define COLOR_WALL DARKGRAY    // ǽ����ɫ
#define COLOR_WAIT LIGHTRED   // ѡ����ɫ

/*��������*/
void clearscreen(COLORREF color);               // ��յ�ͼΪRGB��ɫ(��������)
void drawCell(int x, int y, COLORREF color);    // ��x��yλ�û���ɫΪRGB�ķ���
void drawMaze();
void displayMaze();