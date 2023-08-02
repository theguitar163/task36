// 任务：可视化排序。
// 生成 20 个高度随机、颜色随机的立柱，将其横向一排显示出来。
// 然后，将其按照从小到大的顺序后，显示出来。
// 再然后，实现可视化的排序过程，就是说，排序的每一个步骤，谁和谁比较，谁和谁交换，用动画显示出来，展示完整的排序过程。
#include <stdio.h>
#include <easyx.h>
#include <conio.h>
#include <time.h>

#define MAX_NUM 100
#define MIN_NUM 10
#define DATA_COUNT 50

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

void drawData(int left, int top, int width, int height)
{
    int bottom = top + height;
    int right = left + width;
    int x, y;
    for (int i = 0; i < DATA_COUNT; i++) {
        setlinecolor(data[i].color);
        x = left + i * width / DATA_COUNT;
        y = bottom - data[i].num * height / MAX_NUM;
        line(x, bottom, x, y);
    }
}

void swapData(TData *pd1, TData* pd2)
{
    TData d;
    d = *pd1;
    *pd1 = *pd2;
    *pd2 = d;
}

void sortBubble() 
{
    int done = 0;
    while (!done) {
        done = 1;
        for (int i = 0; i < DATA_COUNT - 1; i++) {
            if (data[i].num > data[i + 1].num) {
                swapData(&data[i], &data[i + 1]);
                cleardevice();
                drawData(50, 50, 240, 200);
                FlushBatchDraw();
                Sleep(5);
                done = 0;
            }
        }
    }
}

int main()
{
    initgraph(800, 600);
    srand(time(NULL));
    initData();
    drawData(50, 50, 200, 200);
    BeginBatchDraw();
    sortBubble();
    EndBatchDraw();
    _getch();
    closegraph();
    return 0;
}
