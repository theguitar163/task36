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
        // �ھ��εײ���������
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

// ð������
void bubbleSort()
{
    cleardevice();
    drawData();
    for (int i = 0; i < DATA_COUNT - 1; i++) {
        // ÿ��ѭ��ȷ����������ͨ������ð�������        
        for (int j = 0; j < DATA_COUNT - 1 - i; j++) {
            if (data[j].num > data[j + 1].num) {        // �������������Ա�
                swapData(&data[j], &data[j + 1]);       // ��������

                cleardevice();
                drawData();
                FlushBatchDraw();
                Sleep(FRAME_RATE);
            }
        }
    }
}

// ѡ������
void selectSort()
{
    cleardevice();
    drawData();
    int minIdx;
    for (int i = 0; i < DATA_COUNT - 1; i++) {
        minIdx = i;
        // Ѱ��ʣ������([i+1]�����)����С�����ݣ���¼���±�
        for (int j = i + 1; j < DATA_COUNT; j++) {
            if (data[minIdx].num > data[j].num) {
                minIdx = j;
            }
        }
        // �ҵ��󣬽���С��[minIdx]�뵱ǰ������ĵ�һ������[i]���н���
        swapData(&data[i], &data[minIdx]);

        cleardevice();
        drawData();
        FlushBatchDraw();
        Sleep(FRAME_RATE);
    }
}
