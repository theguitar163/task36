#pragma once
#include <easyx.h>

class CPanel;

// �����ť�¼���Ӧ����ָ������
typedef void (TFunction)();

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
#define bgFORMAT    1

// ��ť�ṹ
class CButton
{
public:
    int m_shape;                  // ��ť��״bsCIRCLE/bsRDRECT/bsRECT
    int m_type;                   // ��ť����btDEFAULT/btCOLOR/btNUM/btBOOL
    const TCHAR* m_text;          // ��ť�ϵ�����
    COLORREF m_color;             // ��ť����ɫ
    int m_w, m_h;                   // ��ť���
    TFunction* pOnClick = NULL; // �����ť�¼���������ָ��
    int m_groupid = bgNONE;       // Ⱥ��id
    LONG m_tag = 0;               // ��������ֵ�ɼ�����ɫ������������������type�����京��
    int m_x, m_y;                   // ��ť������
    int m_selected = 0;           // ѡ��״̬
    CPanel* m_container; // ���ư�����

    // ���ð�ťλ��
    void setButtonPos(int x, int y);
    // ��ȡ��ť��ɫ
    COLORREF getButtonColor();
    void setButtonColor(COLORREF c);
    // ���ð�ťTag��ֵ
    void setButtonTag(LONG tag);
    LONG getButtonTag();
    // ���ư�ť
    void drawButton();
    // �жϵ�pt�Ƿ��ڰ�ť��
    int ptInButton(POINT p);
};


// ���ť����
#define MAX_BUTTON 100

// ���������뷽ʽ
#define alTOP    0
#define alBOTTOM 1
#define alLEFT   2
#define alRIGHT  3

// �ٿ��ư�����Ӱ�ť�ķ���
#define adRIGHT   0    // �Ҳ����
#define adBOTTOM  1    // �·����
#define adNEWLINE 2    // �������

// ���׿������
// ����������Ӱ�ť����Ϊʹ�ô�C����̫����ʵ�ֳ������̬��������ؼ���
class CPanel
{
public:
    int m_x;
    int m_y;
    int m_w;
    int m_h;
    CButton* m_pbuttons[MAX_BUTTON];
    int m_btnCount = 0;       // ��ť����
    int m_btnClicked = -1;    // ����������ť���±�
 //   struct tagPainter* ppainter;

    void initPanel(int size, int align);
    void addButton(CButton* pbutton);
    void addButton(CButton* pbutton, int spacing, int dir = adRIGHT);
    void drawPanel();
    void updateButtonGroup(int btnIdx);
    void initButtonGroup();
    void buttonClick(int x, int y);
    int ptInPanel(POINT p);
};


