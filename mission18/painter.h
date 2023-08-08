#pragma once

#include <easyx.h>

#define btCIRCLE  0
#define btRECT    1
#define btRDRECT  2

#define MAX_BUTTON 100

#define alTOP    0
#define alBOTTOM 1
#define alLEFT   2
#define alRIGHT  3

struct tagPainter;

// ����ָ������
typedef void (TFunction)(struct tagPainter* ppainter);

// ��ť�ṹ
typedef struct tagButton {
    int type;                 // ��ť����btCIRCLE/btRDRECT/btRECT
    const TCHAR* text;        // ��ť�ϵ�����
    COLORREF color;           // ��ť����ɫ
    int w, h;
    TFunction* pfun = NULL;
    int x, y, x2, y2;         // ��ť������
    int radius;               // Բ�ΰ�ť�İ뾶    
    struct tagPanel* container;
} TButton;

// ���׿������
// ����������Ӱ�ť����Ϊʹ�ô�C����̫����ʵ�ֳ������̬��������ؼ���
typedef struct tagPanel {
    int x;
    int y;
    int w;
    int h;
    TButton* pbuttons[MAX_BUTTON];
    int btnCount = 0;
    struct tagPainter* ppainter;
} TPanel;

#define dtFreehand 0
#define dtLine     1
#define dtRect     2
#define dtCircle   3
#define dtellipse  4

typedef struct tagPainter {
    int x;
    int y;
    int w;
    int h;
    int drawType = 0;
    COLORREF drawColor = BLACK;
    TPanel* ppanel;
} TPainter;

void initPainter(TPainter* ppainter, TPanel*ppanel, int panelsize, int panelalign);
void clearPainter(TPainter* ppainter);
void drawPainter(TPainter* ppainter);

void initButton(TButton* pbtn, int x, int y, int x2, int y2, COLORREF color, TCHAR* text, int mod);
void initButton(TButton* pbtn, int x1, int y1, int radius, int mod);
void initButton(TButton* pbtn, int x, int y, int w, int h, int mod);
void initButton(TButton* pbtn, int x, int y);
// ���ư�ť
void drawButton(TButton* pbtn);
// �жϵ�pt�Ƿ��ڰ�ť��
int ptInButton(POINT p, TButton* pbtn);
void initPanel(TPanel* ppanel, int x, int y, int w, int h);
void initPanel(TPanel* ppanel, int size, int align);
void addButton(TPanel* ppanel, TButton* pbutton);
void drawPanel(TPanel* ppanel);

void buttonClick(TPanel* ppanel, int x, int y);