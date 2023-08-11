#pragma once
#include <easyx.h>

struct tagPainter;

// �����ť�¼���Ӧ����ָ������
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

#define bgNONE      0
#define bgPENCOLOR  1
#define bgFILLCOLOR 2
#define bgPENTYPE   3

// ��ť�ṹ
typedef struct tagButton {
    int shape;                  // ��ť��״bsCIRCLE/bsRDRECT/bsRECT
    int type;                   // ��ť����btDEFAULT/btCOLOR/btNUM/btBOOL
    const TCHAR* text;          // ��ť�ϵ�����
    COLORREF color;             // ��ť����ɫ
    int w, h;                   // ��ť���
    TFunction* pOnClick = NULL; // �����ť�¼���������ָ��
    int groupid = bgNONE;       // Ⱥ��id
    LONG tag = 0;               // ��������ֵ�ɼ�����ɫ������������������type�����京��
    int x, y;                   // ��ť������
    int selected = 0;           // ѡ��״̬
    struct tagPanel* container; // ���ư�����
} TButton;

// ���ð�ťλ��
void setButtonPos(TButton* pbtn, int x, int y);
// ��ȡ��ť��ɫ
COLORREF getButtonColor(TButton* pbtn);
void setButtonColor(TButton* pbtn, COLORREF c);
// ���ð�ťTag��ֵ
void setButtonTag(TButton* pbtn, LONG tag);
LONG getButtonTag(TButton* pbtn);
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
    int btnCount = 0;       // ��ť����
    int btnClicked = -1;    // ����������ť���±�
    struct tagPainter* ppainter;
} TPanel;

// �ٿ��ư�����Ӱ�ť�ķ���
#define adRIGHT   0    // �Ҳ����
#define adBOTTOM  1    // �·����
#define adNEWLINE 2    // �������

void initPanel(TPanel* ppanel, int size, int align);
void addButton(TPanel* ppanel, TButton* pbutton);
void addButton(TPanel* ppanel, TButton* pbutton, int spacing, int dir = adRIGHT);
void drawPanel(TPanel* ppanel);
void updateButtonGroup(TPanel* ppanel, int btnIdx);
void initButtonGroup(TPanel* ppanel);
void buttonClick(TPanel* ppanel, int x, int y);