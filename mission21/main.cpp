// 任务：可视化排序。
// 1.生成 20 个高度随机、颜色随机的立柱，将其横向一排显示出来。
// 2.将其按照从小到大的顺序后，显示出来。
// 3.再然后，实现可视化的排序过程，排序的每一个步骤，谁和谁比较，谁和谁交换，
//   用动画显示出来，展示完整的排序过程。
#include <stdio.h>
#include <easyx.h>
#include <conio.h>
#include <time.h>
#include "sort.h"

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
