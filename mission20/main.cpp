// 任务：一堆反弹的小球（数量可增减）。
// 屏幕上随机出现 10 个颜色各异的小球，沿任意角度运动，碰到边界反弹。
// 按键盘的 + 号，屏幕上增加一个随机运动且颜色随机的小球。按 - 号，屏幕上减少一个小球。
// 每增加一个小球，就为这个小球分配所需的存储空间；每减少一个小球，就释放对应的存储空间。
// 小球数量的下限为 0，上限不限（数量太多时运行会变慢）。

#include<stdio.h>
#include<easyx.h>
#include<math.h>
#include<conio.h>
#define PI 3.1415926

typedef struct tagBall {
	int x;
	int y;
	int r;
	int vx;
	int vy;
	COLORREF color;
} TBall;

typedef struct tagBounce {
	int ballCount = 10;
	TBall* balls;
} TBounce;


int randRange(int from, int to)
{
	return rand() % (to - from + 1) + from;
}

void ballInit(TBall* pb)
{
	pb->r = randRange(10, 30);
	pb->x = randRange(-400 + pb->r, 400 - pb->r);
	pb->y = randRange(-300 + pb->r, 300 - pb->r);

	pb->color = HSVtoRGB((float)(rand() % 360), 0.8f, 0.9f);
	int v = randRange(3, 8);
	double theta = rand() % 360;
	pb->vx = v * cos(theta * PI / 180);
	pb->vy = v * sin(theta * PI / 180);
}

void bounceInitBalls(TBounce* pbc, int count)
{
	pbc->ballCount = 0;
	pbc->balls = (TBall*)malloc(sizeof(TBall) * count);
	if (pbc->balls != NULL) {
		pbc->ballCount = count;
		for (int i = 0; i < count; i++) {
			ballInit(&(pbc->balls[i]));
		}
	}
}

void bounceAddBall(TBounce* pbc)
{
	TBall* pb = (TBall*)realloc(pbc->balls, sizeof(TBall) * (pbc->ballCount + 1));
	if (pb != NULL) {
		pbc->balls = pb;
		pbc->ballCount++;
		ballInit(&(pbc->balls[pbc->ballCount - 1]));
	}
}

void bounceDelBall(TBounce* pbc)
{
	if (pbc->ballCount < 1)
		return;
	else if (pbc->ballCount==1) {
		TBall* pb = (TBall*)realloc(pbc->balls, sizeof(TBall) * (pbc->ballCount - 1));
		pbc->ballCount--;
		pbc->balls = pb;
	}
	else {
		TBall* pb = (TBall*)realloc(pbc->balls, sizeof(TBall) * (pbc->ballCount - 1));
		if (pb != NULL) {
			pbc->ballCount--;
			pbc->balls = pb;
		}
	}
}

void bounceFreeBalls(TBounce* pbc)
{
	free(pbc->balls);
	pbc->ballCount = 0;
}

void bounceDrawBalls(TBounce* pbc)
{
	for (int i = 0; i < pbc->ballCount; i++) {
		setfillcolor(pbc->balls[i].color);
		fillcircle(pbc->balls[i].x, pbc->balls[i].y, pbc->balls[i].r);
	}
}

void bounceMoveBalls(TBounce* pbc)
{
	TBall* pb;
	for (int i = 0; i < pbc->ballCount; i++) {
		pb = &(pbc->balls[i]);
		if (pb->x >= 400 - pb->r || pb->x <= -400 + pb->r) {
			pb->vx = - pb->vx;
		}
		if (pb->y >= 300 - pb->r || pb->y <= -300 + pb->r) {
			pb->vy = - pb->vy;
		}
		pb->x += pb->vx;
		pb->y += pb->vy;
	}
}

int main()
{
	initgraph(800, 600);
	setorigin(400, 300);
	setaspectratio(1, -1);

	TBounce bc;
	bounceInitBalls(&bc, 10);

	BeginBatchDraw();
	ExMessage m;
	while (1) {
		cleardevice();
		bounceDrawBalls(&bc);
		FlushBatchDraw();
		Sleep(40);

		bounceMoveBalls(&bc);

		peekmessage(&m, EX_KEY|EX_CHAR);
		if (m.vkcode == VK_ESCAPE)
			break;
		if (m.ch == '+')
			bounceAddBall(&bc);
		else if (m.ch == '-')
			bounceDelBall(&bc);
	}
	bounceFreeBalls(&bc);
	EndBatchDraw();
	closegraph();
	return 0;
}