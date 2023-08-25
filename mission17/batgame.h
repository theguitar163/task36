#pragma once

#include <easyx.h>

#define PI 3.1415926
#define MARGIN  50

#define SIDE_LEFT  1
#define SIDE_RIGHT 0

#define BAT_TYPE_BAR  0
#define BAT_TYPE_BALL 1

#define KEY_DOWN(vk_code) (GetAsyncKeyState(vk_code) & 0x8000) ? 1 : 0

typedef struct tagTable {
    int left;
    int top;
    int right;
    int bottom;
    int batRange;
} TTable;

// 球桌相关函数

void initTable(TTable* ptable);
void drawTable(TTable* ptable);

typedef struct tagBat {
    TTable* ptable;
    int v;
    int side;
    int type;
    int x;
    int y;
    int w;
    int h;
    int r;
} TBat;

// 球拍相关函数

void initBat(TBat* pbat, TTable* ptable, int side, int type = BAT_TYPE_BAR);
void moveBat(TBat* pbat, unsigned char vkcode);
void drawBat(TBat* pbat);

typedef struct tagPlayer {
    TCHAR name[20];
    int hitCount;
    int winCount;
    int side;
    int textx;
    int texty;
    TBat* pbat;
} TPlayer;

// 玩家相关函数

void initPlayer(TPlayer* pplayer, const TCHAR* name, int side, TTable* ptable);
void drawPlayer(TPlayer* pplayer);

typedef struct tagBall {
    TTable* ptable;
    TBat* pbatA;
    TBat* pbatB;
    TPlayer* pplayerA;
    TPlayer* pplayerB;
    int x;
    int y;
    int r;
    double v;
    double theta;
} TBall;

// 球体相关函数

void initBall(TBall* pball, TTable* ptable, TBat* pbatA, TBat* pbatB, TPlayer* pplayerA, TPlayer* pplayerB);
void drawBall(TBall* pball);
void nextBallPos(TBall* pball, int* px, int* py);
int collisionBat(TBall* pball, int x, int y, TBat* pbat);
int moveBall(TBall* pball);
void initAll(int all = 1);
void render();