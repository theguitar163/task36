#include <math.h>
#include "batgame.h"

// ������غ���

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

// ������غ���

void initBat(TBat* pbat, TTable* ptable, int side, int type)
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
    pbat->x = (pbat->side == SIDE_LEFT) ? ptable->left : ptable->right;
    pbat->y = ptable->top + (ptable->bottom - ptable->top) / 2;
}

void moveBat(TBat* pbat, unsigned char vkcode)
{
    // ���ݰ����ƶ�����
    if (vkcode == VK_UP)
        pbat->y -= pbat->v;
    if (vkcode == VK_DOWN)
        pbat->y += pbat->v;
    if (vkcode == VK_LEFT)
        pbat->x -= pbat->v;
    if (vkcode == VK_RIGHT)
        pbat->x += pbat->v;

    // �߽���
    TTable* ptable = pbat->ptable;
    // ���±߽���
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

    // ���ұ߽��⣬��Ҫ���������߷ֱ���м��
    if (pbat->side == SIDE_LEFT) {
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
    COLORREF c = (pbat->side == SIDE_LEFT) ? BLUE : BROWN;
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

// �����غ���

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

// ������غ���

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
    // ��Ϊ��״���壬������ײ���
    if (pbat->type == BAT_TYPE_BAR) {
        double dist = pbat->x - x;
        // ������״���壬�ı�С���˶��Ƕ�
        if (fabs(dist) <= pbat->w / 2 + pball->r + pball->v    // x�᷽������Ѿ�������ײ
            && pball->y > pbat->y - pbat->h / 2              // y�᷽������״���巶Χ֮��
            && pball->y < pbat->y + pbat->h / 2) {
            // ���ǵ�С���뵲������п��ܹ�������Ҫ����С��λ������ײ��
            if (dist < 0)
                pball->x = pbat->x + (pbat->w / 2 + pball->r + pball->v);
            else
                pball->x = pbat->x - (pbat->w / 2 + pball->r + pball->v);
            pball->theta = PI - pball->theta;
            return 1;
        }
    }
    // ��Ϊ���ε���
    else if (pbat->type == BAT_TYPE_BALL) {
        // ʹ��С��Ԥ�Ƶ�λ�ã���������֮��ľ���
        double dist = sqrt(pow(pbat->y - y, 2) + pow(pbat->x - x, 2));
        if (dist <= pbat->r + pball->r + pball->v) {
            double delta = atan2(pbat->y - pball->y, pbat->x - pball->x);
            // ���ǵ�С���뵲������п��ܹ�������Ҫ����С��λ������ײ��
            pball->x = pbat->x + cos(PI + delta) * (pbat->r + pball->r + pball->v);
            pball->y = pbat->y + sin(PI + delta) * (pbat->r + pball->r + pball->v);
            // �����Ƕ�
            pball->theta = PI + 2 * delta - pball->theta;
            return 1;
        }
    }
    return 0;
}

int moveBall(TBall* pball)
{
    int x, y;
    // Ԥ�ȼ���С����һ��λ��
    nextBallPos(pball, &x, &y);

    // ����Ƿ����ҳ���
    if (x + pball->r > pball->ptable->right) {
        pball->pplayerA->winCount++;
        return 0;
    }
    if (x - pball->r < pball->ptable->left) {
        pball->pplayerB->winCount++;
        return 0;
    }

    // ���±߽���ײ����
    if (y + pball->r > pball->ptable->bottom || y - pball->r < pball->ptable->top)
        pball->theta = -pball->theta;

    // ʹ��Ԥ�ȼ����С����һλ�ý��е�����ײ����
    if (collisionBat(pball, x, y, pball->pbatA))
        pball->pplayerA->hitCount++;
    if (collisionBat(pball, x, y, pball->pbatB))
        pball->pplayerB->hitCount++;
    nextBallPos(pball, &pball->x, &pball->y);
    return 1;
}

