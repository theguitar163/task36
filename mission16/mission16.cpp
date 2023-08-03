// 打字母游戏。
// 1.进入游戏后，屏幕顶端随机位置产生一个随机的大写字母下落，按对应按键后字母消失。
// 2.每按对 10 个按键，字母下降速度加快一点，以增加游戏难度。
// 3.字母落到底部后游戏结束。
// 4.游戏窗口右侧显示提示信息：游戏时间、正确数量、错误数量、正确率、打字速度等。
// 5.随机落下的字母由1个变为 3个，按对任何1个按键，对应的字母消失，并重新产生一个新的字母落下。
//   3个字母的逻辑互不影响。
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <easyx.h>
#include <time.h>

#define BOX_LEFT   10
#define BOX_TOP    10
#define BOX_WIDTH  800
#define BOX_HEIGHT 780
#define MSG_LEFT   820
#define MSG_TOP    10
#define MSG_WIDTH  360
#define MSG_HEIGHT 780

#define MAX_LETTER 10

#define FPS 50

// 字符对象 -----------------------------------------------
typedef struct {
	char ch;
	POINT pos;
	int speed;
} TLetter;

TLetter* letterInit(int speed)
{
	TLetter* pletter = (TLetter*)malloc(sizeof(TLetter));
	char asc = (rand() % 2) ? 'A' : 'a';
	pletter->ch = (char)(asc + rand() % 26);
	pletter->pos.x = BOX_LEFT + rand() % BOX_WIDTH;
	pletter->pos.y = BOX_TOP;
	pletter->speed = speed;
	return pletter;
}

// 打字对象 -------------------------------------------------
typedef struct tagTyping {
	TLetter* letters[MAX_LETTER] = { 0 };
	int lettercount = 1;
	int speed = 2;
} TTyping;

void incCount(TTyping* typing)
{
	if (typing->lettercount<MAX_LETTER)
		typing->lettercount++;
}

void AddLetter(TTyping* typing)
{
	int n = 0;
	for (int i = 0; i < MAX_LETTER; i++)
		if (typing->letters[i])
			n++;

	if (n >= typing->lettercount)
		return;

	for (int i = 0; i < MAX_LETTER; i++) {
		if (!typing->letters[i]) {
			if (n < typing->lettercount) {
				typing->letters[i] = letterInit(typing->speed);
				n++;
			}
			else
				break;
		}
	}
}

int moveLetters(TTyping* typing)
{
	TLetter* pletter;
	int outcount = 0;
	for (int i=0; i < MAX_LETTER; i++) {
		pletter = typing->letters[i];
		if (!pletter) continue;
		if (pletter->pos.y + pletter->speed < BOX_TOP + BOX_HEIGHT) {
			pletter->pos.y += pletter->speed;
		}
		else {
			free(pletter);
			typing->letters[i] = NULL;
			outcount++;
		}
	}
	return outcount;
}

int hitLetters(TTyping* typing, char ch)
{
	TLetter* pletter;

	for (int i = 0; i < MAX_LETTER; i++) {
		pletter = typing->letters[i];
		if (pletter && pletter->ch==ch) {
			free(pletter);
			typing->letters[i] = NULL;
			return 1;
		}
	}
	return 0;
}

void render(TTyping *typing)
{
	for (int i=0; i < MAX_LETTER; i++) {
		if (typing->letters[i]) {
			TLetter* pletter = typing->letters[i];
			outtextxy(pletter->pos.x, pletter->pos.y, pletter->ch);
		}
	}
}

int main()
{
	initgraph(1200, 800);
	srand(time(NULL));

	TTyping typing;

	typing.lettercount = 3;
	AddLetter(&typing);

	settextstyle(48, 0, _T("Consolas"));
	BeginBatchDraw();
	ExMessage m;
	while (true) {
		cleardevice();
		render(&typing);

		if (_kbhit()) {
			char ch = _getch();
			if (ch == 27)
				break;
			if (hitLetters(&typing, ch)) {
				AddLetter(&typing);
			}
		}
		int outcount = moveLetters(&typing);
		FlushBatchDraw();

		Sleep(10);
	}
	EndBatchDraw();
	closegraph();
}
