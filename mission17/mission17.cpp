// 双人击球游戏。
//
// 请顺序完成以下步骤：
// 1. 在屏幕上画一个较大的长方形作为游戏区域，里面有一个小球做 45 度反弹。
// 2. 将游戏区域长方形的左、右边界擦掉，改为在左右各画一个小矩形表示挡板，通过按键可以移动这个挡板。
//    左右挡板只能在己方的区域（以不同颜色区分）内移动。小球碰到挡板就反弹；小球越过左、右边界，游戏结束。
// 3. 左侧游戏者通过ASDW、右侧游戏者通过方向键在各自的区域内上下左右四个方向移动挡板，击打小球。
// 4. 挡板改为圆球，小球碰撞到球形挡板后，根据两球碰撞的接触位置，计算出小球的反弹角度。
// 5. 在顶部增加游戏信息，先累计赢 3 次者，胜利。

#include <stdio.h>
#include <easyx.h>
#include <conio.h>
#include <math.h>
#include <time.h>

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

void initTable(TTable* ptable)
{
    ptable->left = MARGIN;
    ptable->right = getwidth() - MARGIN;
    ptable->top = MARGIN;
    ptable->bottom = getheight() - MARGIN;
    ptable->batRange = (ptable->right - ptable->left) / 2;
}

void drawTable(TTable* ptable)
{
    setbkcolor(WHITE);
    cleardevice();
    setfillcolor(0xf1ebd1);
    solidrectangle(ptable->left, ptable->top, ptable->left + ptable->batRange, ptable->bottom);
    setfillcolor(0xe3effd);
    solidrectangle(ptable->right - ptable->batRange, ptable->top, ptable->right, ptable->bottom);
    setlinecolor(BLACK);
    setlinestyle(PS_SOLID | PS_ENDCAP_SQUARE, 2);
    line(ptable->left, ptable->top, ptable->right, ptable->top);
    line(ptable->left, ptable->bottom, ptable->right, ptable->bottom);
}

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

void initBat(TBat* pbat, TTable* ptable, int side, int type=BAT_TYPE_BAR) 
{
    pbat->ptable = ptable;
    pbat->side = side;
    pbat->type = type;
    if (pbat->type == BAT_TYPE_BAR) {
        pbat->w = 10;
        pbat->h = 80;
    }
    else {
        pbat->r = 40;
    }
    pbat->v = 5;
    pbat->x = (pbat->side==SIDE_LEFT) ? ptable->left : ptable->right;
    pbat->y = ptable->top + (ptable->bottom - ptable->top) / 2;
 }

void moveBat(TBat* pbat, unsigned char vkcode)
{
    // 根据按键移动挡板
    if (vkcode == VK_UP)
        pbat->y -= pbat->v;
    if (vkcode == VK_DOWN)
        pbat->y += pbat->v;
    if (vkcode == VK_LEFT)
        pbat->x -= pbat->v;
    if (vkcode == VK_RIGHT)
        pbat->x += pbat->v;

    // 边界检测
    TTable* ptable = pbat->ptable;
    // 上下边界检测
    if (pbat->type == BAT_TYPE_BALL) {
        if (pbat->y < ptable->top + pbat->r)
            pbat->y = ptable->top + pbat->r;
        if (pbat->y > ptable->bottom - pbat->r)
            pbat->y = ptable->bottom - pbat->r;
    }
    else {
        if (pbat->y < ptable->top + pbat->h / 2)
            pbat->y = ptable->top + pbat->h / 2;
        if (pbat->y > ptable->bottom - pbat->h / 2)
            pbat->y = ptable->bottom - pbat->h / 2;
    }

    // 左右边界检测，需要分左右两边分别进行检测
    if (pbat->side==SIDE_LEFT) {
        if (pbat->x < pbat->ptable->left)
            pbat->x = ptable->left;
        if (pbat->x > ptable->left + ptable->batRange)
            pbat->x = ptable->left + ptable->batRange;
    }
    else {
        if (pbat->x < ptable->left + ptable->batRange)
            pbat->x = ptable->left + ptable->batRange;
        if (pbat->x > ptable->right)
            pbat->x = ptable->right;
    }
}

void drawBat(TBat* pbat)
{
    COLORREF c = (pbat->side==SIDE_LEFT) ? BLUE : BROWN;
    setfillcolor(c);
    if (pbat->type == BAT_TYPE_BAR) {
        int left = pbat->x - pbat->w / 2;
        int right = pbat->x + pbat->w / 2;
        int top = pbat->y - pbat->h / 2;
        int bottom = pbat->y + pbat->h / 2;
        solidrectangle(left, top, right, bottom);
    }
    else
        solidcircle(pbat->x, pbat->y, pbat->r);
}

typedef struct tagPlayer {
    TCHAR name[20];
    int hitCount;
    int winCount;
    int side;
    int textx;
    int texty;
    TBat* pbat;
} TPlayer;

void initPlayer(TPlayer* pplayer, const TCHAR* name, int side, TTable* ptable)
{
    lstrcpy(pplayer->name, name);
    pplayer->hitCount = 0;
    pplayer->winCount = 0;
    pplayer->side = side;
    if (side == SIDE_LEFT)
        pplayer->textx = ptable->left + 10;
    else
        pplayer->textx = ptable->left + (ptable->right - ptable->left) / 2 + 10;

    pplayer->texty = ptable->top - 20;
}

void drawPlayer(TPlayer* pplayer)
{
    TCHAR str[200];
    swprintf(str, 100, L"%s hit:%d win:%d", pplayer->name, pplayer->hitCount, pplayer->winCount);
    settextcolor(BLACK);
    outtextxy(pplayer->textx, pplayer->texty, str);
}

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

void initBall(TBall* pball, TTable* ptable, TBat* pbatA, TBat* pbatB, TPlayer* pplayerA, TPlayer* pplayerB)
{
    pball->x = ptable->left + (ptable->right - ptable->left) / 2;
    pball->y = ptable->top + (ptable->bottom - ptable->top) / 2;
    pball->r = 10;
    pball->v = 5.0;
    pball->theta = 2.0 * PI * (rand() % 360) / 360.0;
    pball->ptable = ptable;
    pball->pbatA = pbatA;
    pball->pbatB = pbatB;
    pball->pplayerA = pplayerA;
    pball->pplayerB = pplayerB;
}

void drawBall(TBall* pball)
{
    setfillcolor(BLACK);
    fillcircle(pball->x, pball->y, pball->r);
}

void nextBallPos(TBall* pball, int* px, int* py)
{
    double vx, vy;
    vx = cos(pball->theta) * pball->v;
    vy = sin(pball->theta) * pball->v;
    *px = pball->x + (int)round(vx);
    *py = pball->y + (int)round(vy);
}

int collisionBat(TBall* pball, int x, int y, TBat* pbat)
{
    // 若为条状挡板，进行碰撞检测
    if (pbat->type == BAT_TYPE_BAR) {
        double dist = pbat->x - x;
        // 碰到条状挡板，改变小球运动角度
        if (fabs(dist) <= pbat->w/2 + pball->r + pball->v    // x轴方向距离已经进入碰撞
            && pball->y > pbat->y - pbat->h / 2              // y轴方向在条状挡板范围之内
            && pball->y < pbat->y + pbat->h / 2) {
            // 考虑到小球与挡板距离有可能过近，需要修正小球位置至碰撞点
            if (dist < 0)
                pball->x = pbat->x + (pbat->w / 2 + pball->r + pball->v);
            else
                pball->x = pbat->x - (pbat->w / 2 + pball->r + pball->v);
            pball->theta = PI - pball->theta;
            return 1;
        }
    }
    // 若为球形挡板
    else if (pbat->type == BAT_TYPE_BALL) {
        // 使用小球预计的位置，计算两者之间的距离
        double dist = sqrt(pow(pbat->y - y, 2) + pow(pbat->x - x, 2));
        if (dist <= pbat->r + pball->r + pball->v) {
            double delta = atan2(pbat->y - pball->y, pbat->x - pball->x);
            // 考虑到小球与挡板距离有可能过近，需要修正小球位置至碰撞点
            pball->x = pbat->x + cos(PI + delta) * (pbat->r + pball->r + pball->v);
            pball->y = pbat->y + sin(PI + delta) * (pbat->r + pball->r + pball->v);
            // 修正角度
            pball->theta = PI + 2 * delta - pball->theta;
            return 1;
        }
    }
    return 0;
}

int moveBall(TBall* pball)
{
    int x, y;
    // 预先计算小球下一个位置
    nextBallPos(pball, &x, &y);

    // 检测是否左右超界
    if (x + pball->r > pball->ptable->right) {
        pball->pplayerA->winCount++;
        return 0;
    }
    if (x - pball->r < pball->ptable->left) {
        pball->pplayerB->winCount++;
        return 0;
    }

    // 上下边界碰撞处理
    if (y+pball->r > pball->ptable->bottom || y-pball->r < pball->ptable->top)
        pball->theta = -pball->theta;

    // 使用预先计算的小球下一位置进行挡板碰撞处理
    if (collisionBat(pball, x, y, pball->pbatA))
        pball->pplayerA->hitCount++;
    if (collisionBat(pball, x, y, pball->pbatB))
        pball->pplayerB->hitCount++;
    nextBallPos(pball, &pball->x, &pball->y);
    return 1;
}

TTable table;
TBat batA, batB;
TPlayer playerA, playerB;
TBall ball;

void initAll(int all=1)
{
    initTable(&table);
    initBat(&batA, &table, SIDE_LEFT, BAT_TYPE_BALL);
    initBat(&batB, &table, SIDE_RIGHT); // , BAT_TYPE_BALL);
    if (all) {
        initPlayer(&playerA, L"喜洋洋", SIDE_LEFT, &table);
        initPlayer(&playerB, L"美羊羊", SIDE_RIGHT, &table);
    }
    initBall(&ball, &table, &batA, &batB, &playerA, &playerB);
}

void render()
{
    drawTable(&table);
    drawBat(&batA);
    drawBat(&batB);
    drawBall(&ball);
    drawPlayer(&playerA);
    drawPlayer(&playerB);
}

int main()
{
    initgraph(1200, 600);
    srand(time(NULL));

    initAll();

    BeginBatchDraw();
    while (true) {
        if (KEY_DOWN(VK_ESCAPE))
            break;

        if (KEY_DOWN('A')) moveBat(&batA, VK_LEFT);
        if (KEY_DOWN(VK_LEFT)) moveBat(&batB, VK_LEFT);
        if (KEY_DOWN('D')) moveBat(&batA, VK_RIGHT);
        if (KEY_DOWN(VK_RIGHT)) moveBat(&batB, VK_RIGHT);
        if (KEY_DOWN('W')) moveBat(&batA, VK_UP);
        if (KEY_DOWN(VK_UP)) moveBat(&batB, VK_UP);
        if (KEY_DOWN('S')) moveBat(&batA, VK_DOWN);
        if (KEY_DOWN(VK_DOWN)) moveBat(&batB, VK_DOWN);

        if (!moveBall(&ball))
            initAll(0);
        render();
        FlushBatchDraw();
        Sleep(10);
    }
    EndBatchDraw();
    closegraph();
}
