#pragma once
#include <easyx.h>

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

TLetter* letterInit(int speed);

// 打字对象 -------------------------------------------------
typedef struct tagTyping {
	TLetter* letters[MAX_LETTER] = { 0 };
	int lettercount = 1;
	int speed = 2;
} TTyping;

void incCount(TTyping* typing);
void AddLetter(TTyping* typing);
int moveLetters(TTyping* typing);
int hitLetters(TTyping* typing, char ch);
void render(TTyping* typing);
