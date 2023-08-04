// 任务：可视化排序。
// 1.生成 20 个高度随机、颜色随机的立柱，将其横向一排显示出来。
// 2.将其按照从小到大的顺序后，显示出来。
// 3.再然后，实现可视化的排序过程，排序的每一个步骤，谁和谁比较，谁和谁交换，
//   用动画显示出来，展示完整的排序过程。
#include <stdio.h>
#include <easyx.h>
#include <conio.h>
#include <time.h>

#define MAX_NUM 100
#define MIN_NUM 10
#define DATA_COUNT 20
#define FRAME_RATE (1000/10)

typedef struct tagData {
    int num;
    COLORREF color;
} TData;

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
        solidrectangle(x, bottom, x+barwidth-2, y);
        swprintf_s(s, L"%d", data[i].num);
        outtextxy(x + (barwidth - textwidth(s)) / 2, bottom - 20, s);
    }
}

void swapData(TData *pd1, TData* pd2)
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

int main()
{
    initgraph(800, 600);
    srand(time(NULL));
    BeginBatchDraw();
    // 冒泡排序
    initData();
    bubbleSort();
    // 选择排序
    initData();
    selectSort();

    EndBatchDraw();
    _getch();
    closegraph();
    return 0;
}
