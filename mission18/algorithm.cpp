#include <easyx.h>
#include <graphics.h>
#include <math.h>
#include "algorithm.h"
#include "painter.h"

#define PI 3.141592653589793

// 选取状态
void Algorithm::drawFocus()
{
    if (m_btnIdx < 6) // 控制当前颜色的选取状态
    {
        setfillcolor(RGB(255, 255, 255));
        solidrectangle(m_focusx - 12, m_focusy - 12, m_focusx + 13, m_focusy + 13);
        m_focusx = button[m_btnIdx].x;
        m_focusy = button[m_btnIdx].y;
        m_focuscolor = button[m_btnIdx].color;
        setfillcolor(RGB(187, 255, 255));
        solidrectangle(m_focusx - 12, m_focusy - 12, m_focusx + 13, m_focusy + 13);
    }
    else // 为当前选中按钮绘制淡蓝色背景作为选取状态
    {
        setfillcolor(RGB(187, 255, 255));
        solidrectangle(button[m_btnIdx].x + 1, button[m_btnIdx].y + 1, button[m_btnIdx].x2 - 1, button[m_btnIdx].y2 - 1);
        outtextxy(button[m_btnIdx].x + 5, button[m_btnIdx].y + 2, button[m_btnIdx].text);
    }
}
// 获取并执行消息
void Algorithm::message_proce(IMAGE read_img)
{
}
void Algorithm::draw_sence()
{
}
// 水平镜像
void Algorithm::per_img(IMAGE read_img)
{
}
// 垂直镜像
void Algorithm::ver_img(IMAGE read_img)
{
}
// 灰度图像
void Algorithm::Gray()
{
}
// 黑白二值
void Algorithm::Black_White()
{
}
// 获取滤波器
void Algorithm::getGaussianArray()
{
}
