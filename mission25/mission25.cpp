﻿// 任务：求解八皇后问题。
// 八皇后问题（Eight queens），由国际西洋棋棋手马克斯·贝瑟尔于1848年提出：
// 在8×8格的国际象棋上摆放 8 个皇后，使其不能互相攻击，即任意两个皇后都不
// 能处于同一行、同一列或同一斜线上，问有多少种摆法。
// 请写一个程序，动态演示八皇后问题的求解过程。

/*
回溯算法：
将列A的皇后放在第一行以后，列 B的皇后放在第一行已经发生冲突。这时候不必继续
放列 C的皇后，而是调整列B的皇后到第二行，继续冲突放第三行，不冲突了才开始进
入列C。如此可依次放下列A至E的皇后，如图 2所示。将每个皇后往右边横向、斜向攻
击的点位用叉标记，发现列F的皇后无处安身。这时回溯到列 E的皇后，将其位置由第
4行调整为第8行，进入列F，发现皇后依然无处安身，再次回溯列E。此时列E已经枚举
完所有情况，回溯至列 D，将其由第2行移至第7行，再进入列E继续。按此算法流程最
终找到如图 3所示的解，成功在棋盘里放下了8个“和平共处”的皇后。继续找完全部
的解共92个。
回溯算法求解八皇后问题的原则是：有冲突解决冲突，没有冲突往前走，无路可走往回
退，走到最后是答案。为了加快有无冲突的判断速度，可以给每行和两个方向的每条对
角线是否有皇后占据建立标志数组。放下一个新皇后做标志，回溯时挪动一个旧皇后清
除标志。
*/
#include <stdio.h>
#include <stdlib.h>
#include <easyx.h>
#include <graphics.h>
#include <conio.h>
#include <iostream>

#define CELL_WIDTH 64
#define BORDER_WIDTH 32
#define MAX_CELL 8

// 皇冠图形结构 -------------------------------------------
typedef struct tagCrown {
    IMAGE image;
    IMAGE mask;
} TCrown;

void initCrown(TCrown* pcrown, const TCHAR* fimage, const TCHAR* fmask)
{
    loadimage(&pcrown->image, fimage); // 源图像
    loadimage(&pcrown->mask, fmask);   // 掩码图像
}

void drawCrown(TCrown* pcrown, int cx, int cy)
{
    int x = BORDER_WIDTH + cx * CELL_WIDTH;
    int y = BORDER_WIDTH + cy * CELL_WIDTH;
    putimage(x, y, &pcrown->mask, SRCAND);       // 掩码图与背景与
    putimage(x, y, &pcrown->image, SRCPAINT);    // 源图像与背景或
}

// 棋盘格 ---------------------------------------------
typedef struct tagGrid {
    TCrown* pcrown;           // 皇冠图形指针
    int rows[MAX_CELL];       // 使用一维数组保存皇后位置，数组值为列cx，数组下标为行cy
} TGrid;

// 初始化单元格
void initGrid(TGrid* pgrid, TCrown* pcrown = NULL)
{
    // 首次初始化时，需传入皇冠图形对象指针
    if (pcrown != NULL)
        pgrid->pcrown = pcrown;
    for (int i = 0; i < MAX_CELL; i++)
            pgrid->rows[i] = -1;   // -1表示该行未放置皇后
}

// 依据每行皇后位置，绘制皇冠
void drawCells(TGrid* pgrid)
{
    for (int i = 0; i < MAX_CELL; i++) {
        if (pgrid->rows[i] >= 0)
            drawCrown(pgrid->pcrown, pgrid->rows[i], i);  // 数组值为列cx，数组下标为行cy
    }
}

// 绘制棋盘
void drawBoard()
{

    setbkcolor(WHITE);
    setbkmode(TRANSPARENT);  // 输出字符时保持透明背景
    cleardevice();

    // 绘制棋盘边框（实际是全图填充）
    setfillcolor(0x212476);
    solidrectangle(0, 0, 8 * CELL_WIDTH + BORDER_WIDTH * 2, 8 * CELL_WIDTH + BORDER_WIDTH * 2);

    // 绘制棋盘格子
    for (int i = 0; i < MAX_CELL; i++) {
        for (int j = 0; j < MAX_CELL; j++) {
            // 交错设置深浅颜色
            if ((i + j) % 2 == 0)
                setfillcolor(0xafdaf5);
            else
                setfillcolor(0x366ecc);
            // 填充单元格
            solidrectangle(BORDER_WIDTH + i * CELL_WIDTH,
                BORDER_WIDTH + j * CELL_WIDTH,
                BORDER_WIDTH + (i + 1) * CELL_WIDTH,
                BORDER_WIDTH + (j + 1) * CELL_WIDTH);
        }
    }

    RECT r;
    TCHAR str[10];
    settextcolor(WHITE);
    for (int i = 0; i < MAX_CELL; i++) {
        // 绘制纵向坐标值1-8
        swprintf_s(str, L"%d", i + 1);
        r = { 0, BORDER_WIDTH + i * CELL_WIDTH, BORDER_WIDTH, BORDER_WIDTH + (i + 1) * CELL_WIDTH };
        drawtext(str, &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
        // 绘制横向坐标值A-H
        swprintf_s(str, L"%c", 'A' + i);
        r = { BORDER_WIDTH + i * CELL_WIDTH, BORDER_WIDTH + MAX_CELL * CELL_WIDTH,
            BORDER_WIDTH + (i + 1) * CELL_WIDTH, BORDER_WIDTH * 2 + MAX_CELL * CELL_WIDTH };
        drawtext(str, &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
    }
}

// 检查该位置能否放入皇后，此处仅检查从 [0 .. cy-1] 行
int checkQueenPos(TGrid* pgrid, int cx, int cy)
{
    for (int i = 0; i < cy; i++) {
        if (pgrid->rows[i] == cx ||     // 同一列
            (pgrid->rows[i] - i) == (cx - cy) ||   // 左侧斜线，x-y相等
            (pgrid->rows[i] + i) == (cx + cy)) {   // 右侧斜线，x+y相等
            return 0;
        }
    }
    return 1;
}

// 绘制当前第 n 种解法
void drawSolveCount(int solvecount)
{
    RECT r = r = { 0, 0, BORDER_WIDTH * 2 + MAX_CELL * CELL_WIDTH, BORDER_WIDTH };
    TCHAR str[100];
    settextcolor(WHITE);
    swprintf_s(str, L"八皇后问题：第 %d 种解法", solvecount);
    drawtext(str, &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
}

// 添加皇后
void addQueen(TGrid* pgrid, int cy, int* psolvecount)
{
    if (cy > MAX_CELL - 1) {
        drawBoard();
        drawCells(pgrid);
        (*psolvecount)++;
        drawSolveCount(*psolvecount);
        FlushBatchDraw();
        Sleep(500);
        return;
    }

    for (int cx = 0; cx < MAX_CELL; cx++) {
        if (checkQueenPos(pgrid, cx, cy)) {
            pgrid->rows[cy] = cx;
            addQueen(pgrid, cy + 1, psolvecount);
        }
    }
}

int main()
{
    initgraph(BORDER_WIDTH * 2 + CELL_WIDTH * MAX_CELL, BORDER_WIDTH * 2 + CELL_WIDTH * MAX_CELL);
    srand(GetTickCount());

    TCrown crown;
    initCrown(&crown,
        L"\\C语言编程\\mission\\mission25\\crown.png",
        L"\\C语言编程\\mission\\mission25\\crownmask.png");

    TGrid grid;
    initGrid(&grid, &crown);

    BeginBatchDraw();

    int solvecount = 0;
    addQueen(&grid, 0, &solvecount);

    EndBatchDraw();
    _getch();
    closegraph();
    return 0;
}