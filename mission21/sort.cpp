#include <easyx.h>
#include "sort.h"

TData data[DATA_COUNT];

void initData()
{
    for (int i = 0; i < DATA_COUNT; i++) {
        data[i].num = MIN_NUM + rand() % (MAX_NUM - MIN_NUM + 1);
        data[i].color = RGB(rand() % 256, 128 + rand() % 128, rand() % 256);
    }
}

void drawData()
{
    int left = 50;
    int top = 50;
    int width = getwidth() - 100;
    int height = getheight() - 100;
    int bottom = top + height;
    int right = left + width;
    int x, y, barwidth;
    TCHAR s[20];
    barwidth = width / DATA_COUNT;
    setbkmode(TRANSPARENT);
    settextcolor(BLACK);
    for (int i = 0; i < DATA_COUNT; i++) {
        setfillcolor(data[i].color);
        x = left + i * width / DATA_COUNT;
        y = bottom - data[i].num * height / MAX_NUM;
        solidrectangle(x, bottom, x + barwidth - 2, y);
        // 在矩形底部绘制数字
        swprintf_s(s, L"%d", data[i].num);
        outtextxy(x + (barwidth - textwidth(s)) / 2, bottom - 20, s);
    }
}

void swapData(TData* pd1, TData* pd2)
{
    TData d = *pd1;
    *pd1 = *pd2;
    *pd2 = d;
}

// 冒泡排序
void bubbleSort()
{
    cleardevice();
    drawData();
    for (int i = 0; i < DATA_COUNT - 1; i++) {
        // 每轮循环确保最大的数，通过交换冒泡至最后        
        for (int j = 0; j < DATA_COUNT - 1 - i; j++) {
            if (data[j].num > data[j + 1].num) {        // 相邻两数两两对比
                swapData(&data[j], &data[j + 1]);       // 交换两数

                cleardevice();
                drawData();
                FlushBatchDraw();
                Sleep(FRAME_RATE);
            }
        }
    }
}

// 选择排序
void selectSort()
{
    cleardevice();
    drawData();
    int minIdx;
    for (int i = 0; i < DATA_COUNT - 1; i++) {
        minIdx = i;
        // 寻找剩余数组([i+1]至最后)中最小的数据，记录其下标
        for (int j = i + 1; j < DATA_COUNT; j++) {
            if (data[minIdx].num > data[j].num) {
                minIdx = j;
            }
        }
        // 找到后，将最小数[minIdx]与当前待排序的第一个数据[i]进行交换
        swapData(&data[i], &data[minIdx]);

        cleardevice();
        drawData();
        FlushBatchDraw();
        Sleep(FRAME_RATE);
    }
}
