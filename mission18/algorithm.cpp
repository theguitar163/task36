#include <easyx.h>
#include <graphics.h>
#include <math.h>
#include "algorithm.h"
#include "painter.h"

#define PI 3.141592653589793

// ѡȡ״̬
void Algorithm::drawFocus()
{
    if (m_btnIdx < 6) // ���Ƶ�ǰ��ɫ��ѡȡ״̬
    {
        setfillcolor(RGB(255, 255, 255));
        solidrectangle(m_focusx - 12, m_focusy - 12, m_focusx + 13, m_focusy + 13);
        m_focusx = button[m_btnIdx].x;
        m_focusy = button[m_btnIdx].y;
        m_focuscolor = button[m_btnIdx].color;
        setfillcolor(RGB(187, 255, 255));
        solidrectangle(m_focusx - 12, m_focusy - 12, m_focusx + 13, m_focusy + 13);
    }
    else // Ϊ��ǰѡ�а�ť���Ƶ���ɫ������Ϊѡȡ״̬
    {
        setfillcolor(RGB(187, 255, 255));
        solidrectangle(button[m_btnIdx].x + 1, button[m_btnIdx].y + 1, button[m_btnIdx].x2 - 1, button[m_btnIdx].y2 - 1);
        outtextxy(button[m_btnIdx].x + 5, button[m_btnIdx].y + 2, button[m_btnIdx].text);
    }
}
// ��ȡ��ִ����Ϣ
void Algorithm::message_proce(IMAGE read_img)
{
}
void Algorithm::draw_sence()
{
}
// ˮƽ����
void Algorithm::per_img(IMAGE read_img)
{
}
// ��ֱ����
void Algorithm::ver_img(IMAGE read_img)
{
}
// �Ҷ�ͼ��
void Algorithm::Gray()
{
}
// �ڰ׶�ֵ
void Algorithm::Black_White()
{
}
// ��ȡ�˲���
void Algorithm::getGaussianArray()
{
}
