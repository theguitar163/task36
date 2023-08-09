#pragma once

#include <easyx.h>


struct tagPainter;

// ����ָ������
typedef void (TFunction)(struct tagPainter* ppainter);

// ��ť��״
#define bsCIRCLE  0
#define bsRECT    1
#define bsRDRECT  2

// ��ť����
#define btDEFAULT 0
#define btCOLOR   1
#define btNUM     2
#define btBOOL    3

// ��ť�ṹ
typedef struct tagButton {
    int shape;                // ��ť��״bsCIRCLE/bsRDRECT/bsRECT
    int type;                 // ��ť����btDEFAULT/btCOLOR/btNUM/btBOOL
    const TCHAR* text;        // ��ť�ϵ�����
    COLORREF color;           // ��ť����ɫ
    int w, h;                 // ��ť���
    TFunction* pfun = NULL;   // ��������ָ��
    int x, y, x2, y2;         // ��ť������
    LONG value = 0;           // ��������ֵ�ɼ�����ɫ��������������
    struct tagPanel* container; // ���ư�����
} TButton;

// ���ð�ťλ��
void setButtonPos(TButton* pbtn, int x, int y);
// ���ư�ť
void drawButton(TButton* pbtn);
// �жϵ�pt�Ƿ��ڰ�ť��
int ptInButton(POINT p, TButton* pbtn);

// ���ť����
#define MAX_BUTTON 100

// ���������뷽ʽ
#define alTOP    0
#define alBOTTOM 1
#define alLEFT   2
#define alRIGHT  3

// ���׿������
// ����������Ӱ�ť����Ϊʹ�ô�C����̫����ʵ�ֳ������̬��������ؼ���
typedef struct tagPanel {
    int x;
    int y;
    int w;
    int h;
    TButton* pbuttons[MAX_BUTTON];
    int btnCount = 0;
    int btnFocused = -1;
    struct tagPainter* ppainter;
} TPanel;

#define adRIGHT   0
#define adBOTTOM  1
#define adNEWLINE 2

void initPanel(TPanel* ppanel, int size, int align);
void addButton(TPanel* ppanel, TButton* pbutton);
void addButton(TPanel* ppanel, TButton* pbutton, int spacing, int dir = adRIGHT);
void drawPanel(TPanel* ppanel);

// ��������
#define ptFREEHAND 0
#define ptLINE     1
#define ptRECT     2
#define ptCIRCLE   3
#define ptELLIPSE  4
#define ptERASER   5
#define ptMOSAIC   6

// ����ṹ
typedef struct tagPainter {
    HWND hwnd;  // ���ھ��
    int x;      // ������Χ
    int y;
    int w;
    int h;
    int startx, starty;         // ������ʼλ��
    int penType = ptLINE;       // ��������
    int penThickness = 1;       // �����߿�
    COLORREF penColor = BLACK;  // ������ɫ
    int isFill = 0;             // �Ƿ����
    COLORREF fillColor = WHITE; // �����ɫ
    IMAGE imgBackup;    // ����ͼ�񱸷�
    TPanel* ppanel;     // ��ť���ư�
} TPainter;

void initPainter(TPainter* ppainter, HWND hwnd, TPanel*ppanel, int panelsize, int panelalign);
void clearPainter(TPainter* ppainter);
void backupPainter(TPainter* ppainter);
void drawPainter(TPainter* ppainter);
int ptInPainter(POINT p, TPainter* ppainter, int shrinksize=0);

void painterClick(TPainter* ppainter, int startx, int starty);
void buttonClick(TPanel* ppanel, int x, int y);

void PaintLine(TPainter* ppainter, int startx, int starty);
void PaintRect(TPainter* ppainter, int startx, int starty);
void PaintEllipse(TPainter* ppainter, int startx, int starty);
void PaintEraser(TPainter* ppainter, int startx, int starty);
void PaintMosaic(TPainter* ppainter, int startx, int starty);


