#include "typing.h"

// 字符对象 -----------------------------------------------

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

void incCount(TTyping* typing)
{
	if (typing->lettercount < MAX_LETTER)
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
	for (int i = 0; i < MAX_LETTER; i++) {
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
		if (pletter && pletter->ch == ch) {
			free(pletter);
			typing->letters[i] = NULL;
			return 1;
		}
	}
	return 0;
}

void render(TTyping* typing)
{
	for (int i = 0; i < MAX_LETTER; i++) {
		if (typing->letters[i]) {
			TLetter* pletter = typing->letters[i];
			outtextxy(pletter->pos.x, pletter->pos.y, pletter->ch);
		}
	}
}
