#pragma once

#include <easyx.h>

#define MAX_NUM 100
#define MIN_NUM 10
#define DATA_COUNT 20
#define FRAME_RATE (1000/10)

typedef struct tagData {
    int num;
    COLORREF color;
} TData;

void initData();
void drawData();
void swapData(TData* pd1, TData* pd2);

// ð������
void bubbleSort();
// ѡ������
void selectSort();