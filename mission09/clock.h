#pragma once

#include <easyx.h>

#define ULEN 80
#define PI 3.1415926
#define CLOCK_R 220
#define SHAND_R 200
#define MHAND_R 180
#define HHAND_R 150

// ���Ʊ���
void drawClockFace(POINT op);
// ��������
void drawSecHand(POINT op, double theta);
// ���Ʒ���
void drawMinHand(POINT op, double theta);
// ����ʱ��
void drawHurHand(POINT op, double theta);