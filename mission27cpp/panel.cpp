#include <easyx.h>
#include "panel.h"

void CButton::setButtonColor(COLORREF c)
{
    m_color = c;
}

COLORREF CButton::getButtonColor()
{
    return m_color;
}

// ���ð�ťλ��
void CButton::setButtonPos(int x, int y)
{
    m_x = x;
    m_y = y;
}

// ���ð�ťTag��ֵ
void CButton::setButtonTag(LONG tag)
{
    m_tag = tag;
}

LONG CButton::getButtonTag()
{
    return m_tag;
}

// ���ư�ť
void CButton::drawButton()
{
    int ox, oy;
    // ��ȡ����ԭ�����꣬��������Ϊ����ԭ��
    if (m_container == NULL) {
        ox = 0;
        oy = 0;
    }
    else {
        ox = m_container->m_x;
        oy = m_container->m_y;
    }

    // ���ư�ť��������Բ�Ρ�Բ�Ǿ��Ρ���������
    setlinecolor(BLACK);
    settextcolor(BLACK);
    RECT r = { ox + m_x, oy + m_y, ox + m_x + m_w, oy + m_y + m_h };
    if (m_shape == bsCIRCLE) {
        if (m_selected) {
            setlinecolor(LIGHTBLUE);
            ellipse(r.left - 2, r.top - 2, r.right + 2, r.bottom + 2);
        }
        setlinecolor(BLACK);
        setfillcolor(m_color);
        fillellipse(r.left, r.top, r.right, r.bottom);
    }
    else if (m_shape == bsRDRECT) {
        int esize = ((m_w > m_h) ? m_h : m_w) / 4;
        if (m_selected) {
            setlinecolor(LIGHTBLUE);
            roundrect(r.left - 2, r.top - 2, r.right + 2, r.bottom + 2, esize + 2, esize + 2);
        }
        setlinecolor(BLACK);
        setfillcolor(m_color);
        fillroundrect(r.left, r.top, r.right, r.bottom, esize, esize);
    }
    else {
        if (m_selected) {
            setlinecolor(LIGHTBLUE);
            rectangle(r.left - 2, r.top - 2, r.right + 2, r.bottom + 2);
        }
        setlinecolor(BLACK);
        setfillcolor(m_color);
        fillrectangle(r.left, r.top, r.right, r.bottom);
    }
    // ���ư�ť����
    if (m_text != NULL) {
        TCHAR str[200];
        if (m_type == btNUM)
            swprintf_s(str, L"%s[%d]", m_text, m_tag);
        else if (m_type == btBOOL)
            swprintf_s(str, L"%s[%s]", m_text, (m_tag == 0) ? L"��" : L"��");
        else
            wcscpy_s(str, m_text);
        settextstyle(12, 0, L"����");
        // �Զ�����������ɫ����Ҫ��������
        COLORREF c = m_color;
        float h, s, l;
        RGBtoHSL(c, &h, &s, &l);
        settextcolor(HSLtoRGB(h, s, (l > 0.5) ? 0 : 1));
        drawtext(str, &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE | DT_END_ELLIPSIS);
    }
}

// �жϵ�pt�Ƿ��ڰ�ť��
int CButton::ptInButton(POINT p)
{
    int bx, by;
    // ��ȡ����ԭ�����꣬��������Ϊ����ԭ��
    if (m_container == NULL) {
        bx = m_x;
        by = m_y;
    }
    else {
        bx = m_container->m_x + m_x;
        by = m_container->m_y + m_y;
    }
    // ��Բ��ť��������Ƿ�����Բ��
    if (m_shape == bsCIRCLE) {
        double a = m_w / 2;
        double b = m_h / 2;
        double dx = p.x - (bx + a);
        double dy = p.y - (by + b);
        if ((dx * dx) / (a * a) + (dy * dy) / (b * b) <= 1.0)
            return 1;
    }
    else {
        if (p.x >= bx && p.x <= bx + m_w && p.y >= by && p.y <= by + m_h)
            return 1;
    }
    return 0;
}

// ��ʼ�����ư�
// ���ݶ��뷽ʽ���Զ����ÿ��ư�λ��
void CPanel::initPanel(int size, int align)
{
    switch (align) {
    case alTOP:
        m_x = 0;
        m_y = 0;
        m_w = getwidth();
        m_h = size;
        break;
    case alBOTTOM:
        m_x = 0;
        m_y = getheight() - size;
        m_w = getwidth();
        m_h = size;
        break;
    case alLEFT:
        m_x = 0;
        m_y = 0;
        m_w = size;
        m_h = getheight();
        break;
    case alRIGHT:
        m_x = getwidth() - size;
        m_y = 0;
        m_w = size;
        m_h = getheight();
    }
}

// ��Ӱ�ť�������ǰ�ťλ��
void CPanel::addButton(CButton* pbutton)
{
    if (m_btnCount < MAX_BUTTON - 1) {
        m_pbuttons[m_btnCount] = pbutton;
        pbutton->m_container = this;
        m_btnCount++;
    }
}

// ��Ӱ�ť�����ư���
// ����ָ������ӷ�ʽ�ͼ���Զ����ð�ť��λ��
void CPanel::addButton(CButton* pbutton, int spacing, int dir)
{
    if (m_btnCount < MAX_BUTTON - 1) {
        CButton* p = m_pbuttons[m_btnCount - 1];
        if (m_btnCount == 0) {
            pbutton->m_x = spacing;
            pbutton->m_y = spacing;
        }
        else if (dir == adBOTTOM) {
            pbutton->m_x = p->m_x;
            pbutton->m_y = p->m_y + p->m_h + spacing;
        }
        else if (dir == adNEWLINE) {
            pbutton->m_x = spacing;
            pbutton->m_y = p->m_y + p->m_h + spacing;
        }
        else { // Ĭ��Ϊ�Ҳ����
            pbutton->m_x = p->m_x + p->m_w + spacing;
            pbutton->m_y = p->m_y;
        }
        m_pbuttons[m_btnCount] = pbutton;
        pbutton->m_container = this;
        m_btnCount++;
    }
}

// ���ƿ��ư弰�������İ�ť
void CPanel::drawPanel()
{
    setlinecolor(BLACK);
    setfillcolor(WHITE);
    setlinestyle(PS_SOLID, 1);
    fillrectangle(m_x, m_y, m_x + m_w, m_y + m_h);
    for (int i = 0; i < m_btnCount; i++) {
        m_pbuttons[i]->drawButton();
    }
}

// ��ť����¼����ɴ���
void CPanel::buttonClick(int x, int y)
{
    // �������а�ť
    for (int i = 0; i < m_btnCount; i++) {
        // �жϵ���������Ƿ��ڰ�ť��
        if (m_pbuttons[i]->ptInButton({ x, y })) {
            m_btnClicked = i;
            TFunction* pfun = m_pbuttons[i]->pOnClick;
            if (pfun != NULL)
                // (*pfun)(m_ppainter);
                (*pfun)();
            int btnIdx = m_btnClicked;
            updateButtonGroup(btnIdx);
            drawPanel();
            FlushBatchDraw();

            break;
        }
    }
}

// Ⱥ�鰴ť������Զ����°�ťȺ��������ť״̬
void CPanel::updateButtonGroup(int btnIdx)
{
    int gid = m_pbuttons[btnIdx]->m_groupid;
    if (gid == bgNONE) return;
    for (int i = 0; i < m_btnCount; i++) {
        if (m_pbuttons[i]->m_groupid == gid) {
            if (btnIdx == i)
                m_pbuttons[i]->m_selected = 1;
            else
                m_pbuttons[i]->m_selected = 0;
        }
    }
}

// ΪȺ�鰴ť����Ĭ��ѡ�а�ť
// ���հ�ť��ӵ�˳��ÿ��Ⱥ���һ�����ֵİ�ťΪĬ��ѡ��״̬
void CPanel::initButtonGroup()
{
    int gid = bgNONE;
    for (int i = 0; i < m_btnCount; i++) {
        if (m_pbuttons[i]->m_groupid != bgNONE && m_pbuttons[i]->m_groupid != gid ) {
            updateButtonGroup(i);
            gid = m_pbuttons[i]->m_groupid;
        }
    }
}

int CPanel::ptInPanel(POINT p)
{
    int left = m_x;
    int right = m_x + m_w;
    int top = m_y;
    int bottom = m_y + m_h;
    if (p.x > left && p.x < right && p.y > top && p.y < bottom)
        return 1;
    return 0;
}