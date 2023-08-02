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

int ballCount = 10;
TBall* balls;


int randRange(int from, int to)
{
	return rand() % (to - from + 1) + from;
}

void initBall(TBall* pb)
{
	pb->x = randRange(-400 + pb->r, 400 - pb->r);
	pb->y = randRange(-300 + pb->r, 300 - pb->r);
	pb->r = 10;
	pb->color = HSVtoRGB((float)(rand() % 360), 0.8f, 0.9f);
	int v = randRange(3, 8);
	double theta = rand() % 360;
	pb->vx = v * cos(theta * PI / 180);
	pb->vy = v * sin(theta * PI / 180);
}

void initAllBalls(int count)
{
	ballCount = 0;
	balls = (TBall*)malloc(sizeof(TBall) * count);
	if (balls != NULL) {
		ballCount = count;
		for (int i = 0; i < count; i++) {
			initBall(&(balls[i]));
		}
	}
}

void addBall()
{
	TBall* pb = (TBall*)realloc(balls, sizeof(TBall) * (ballCount + 1));
	if (pb != NULL) {
		balls = pb;
		ballCount++;
		initBall(&(balls[ballCount - 1]));
	}
}

void removeBall()
{
	if (ballCount < 1)
		return;
	else if (ballCount==1) {
		TBall* pb = (TBall*)realloc(balls, sizeof(TBall) * (ballCount - 1));
		ballCount--;
	}
	else {
		TBall* pb = (TBall*)realloc(balls, sizeof(TBall) * (ballCount - 1));
		if (pb != NULL) {
			ballCount--;
			balls = pb;
		}
	}
}

void freeBalls()
{
	free(balls);
	ballCount = 0;
}

void drawBalls()
{
	for (int i = 0; i < ballCount; i++) {
		setfillcolor(balls[i].color);
		fillcircle(balls[i].x, balls[i].y, balls[i].r);
	}
}

void moveBalls()
{
	for (int i = 0; i < ballCount; i++) {
		if (balls[i].x >= 400 - balls[i].r || balls[i].x <= -400 + balls[i].r) {
			balls[i].vx = -balls[i].vx;
		}
		if (balls[i].y >= 300 - balls[i].r || balls[i].y <= -300 + balls[i].r) {
			balls[i].vy = -balls[i].vy;
		}
		balls[i].x += balls[i].vx;
		balls[i].y += balls[i].vy;
	}
}
int main()
{
	initgraph(800, 600);
	setorigin(400, 300);
	setaspectratio(1, -1);

	initAllBalls(ballCount);
	BeginBatchDraw();
	ExMessage m;
	while (1) {
		cleardevice();
		drawBalls();
		FlushBatchDraw();
		Sleep(40);
		moveBalls();
		peekmessage(&m);
		if (m.ch == '+')
			addBall();
		else if (m.ch == '-')
			removeBall();
	}
	freeBalls();
	EndBatchDraw();
	closegraph();
	return 0;
}