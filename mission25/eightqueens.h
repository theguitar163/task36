#pragma once

#include <easyx.h>

#define CELL_WIDTH 64
#define BORDER_WIDTH 32
#define MAX_CELL 8

// 皇冠图像结构 -------------------------------------------
typedef struct tagCrown {
    IMAGE image;
    IMAGE mask;
} TCrownImage;

// 加载皇冠图像和掩码(文件)
void initCrownFile(TCrownImage* pcrown, const TCHAR* fimage, const TCHAR* fmask);
// 加载皇冠图像和掩码(资源文件)
void initCrownRes(TCrownImage* pcrown, const TCHAR* rtype, const TCHAR* rimage, const TCHAR* rmask);
// 绘制皇冠透明图像
void drawCrown(TCrownImage* pcrown, int cx, int cy);

// 棋盘格 ---------------------------------------------
typedef struct tagBoard {
    TCrownImage* pcrown;      // 皇冠图像指针
    int rows[MAX_CELL];       // 使用一维数组保存皇后位置，数组值为列cx，数组下标为行cy
    int solvecount;           // 
} TBoard;

// 初始化单元格
void initBoard(TBoard* pboard, TCrownImage* pcrown = NULL);
// 绘制棋盘以及棋盘上的皇后
void drawBoard(TBoard* pboard);
// 在cy行添加皇后。（递归回溯算法）
void addQueen(TBoard* pboard, int cy);