#include <easyx.h>
#include <graphics.h>
#include <math.h>
#include "algorithm.h"
#include "painter.h"

#define PI 3.141592653589793

HWND hout;
int con = 0; // 控制切换绘图板和插图
int key = 0;
TCHAR szFile[MAX_PATH] = { 0 }; // 存储打开图片的路径

// 从电脑中获取图片
int FileDialog(TCHAR* path)
{
    OPENFILENAME ofn;
    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn); // 结构大小
    ofn.lpstrFile = path; // 路径
    ofn.nMaxFile = MAX_PATH; // 路径大小
    ofn.lpstrFilter = TEXT("图片文件(bmp; jpg; png; tiff; tif; jpeg; gif)\0*.bmp; *.jpg; *.png; *.tiff; *.jpeg; *.gif; *.tif\0\0"); // 文件类型
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST; // 标志
    return GetOpenFileName(&ofn);
}

// 初始化绘图场景
Algorithm::Algorithm(IMAGE* img, int width, int height)
{
}
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
    int pen = 0;                            // 控制画笔
    int penType = 0;                            // 控制画矩形
    int opene = 0;                            // 控制画椭圆
    ExMessage m;                        // 鼠标消息
    TCHAR str[10];                        // 字符数组
    static int x, y;                    // 记录上一次鼠标位置
    int xc, xd;                            // 用于辅助画矩形（椭圆、截图）
    IMAGE* store;                        // 存储截图图片
    int R[6], G[6], B[6];                // 暂时存储配置文件信息

    CHOOSECOLOR stChooseColor;            // 声明一个颜色选取的结构体变量
    COLORREF rgbLineColor = NULL;            // 编辑的存储选择的颜色
    COLORREF dwCustColors[16];            // 为结构体变量赋初值
    stChooseColor.lStructSize = sizeof(CHOOSECOLOR);
    stChooseColor.hwndOwner = hout;
    stChooseColor.rgbResult = rgbLineColor;
    stChooseColor.lpCustColors = (LPDWORD)dwCustColors;
    stChooseColor.Flags = CC_RGBINIT;
    stChooseColor.lCustData = 0;
    stChooseColor.lpfnHook = NULL;
    stChooseColor.lpTemplateName = NULL;

    setfillcolor(RGB(187, 255, 255)); // 默认在打开画板的时候选中“笔”
    solidrectangle(m_focusx - 12, m_focusy - 12, m_focusx + 13, m_focusy + 13);
    solidrectangle(button[6].x + 1, button[6].y + 1, button[6].x2 - 1, button[6].y2 - 1);
    draw_sence(); // 绘制场景
    while (key == 0)
    {
        if (peekmessage(&m, EM_MOUSE | EM_KEY))
        {
            // 左键单击判断
            if (m.message == WM_LBUTTONDOWN) {
                // 控制画线
                if (penType == ptLINE) {
                    if (m.x < m_width && m.y < m_height) {
                        x = m.x; y = m.y;
                        while (true) {
                            m = getmessage(EM_MOUSE);
                            if (m.message == WM_MOUSEMOVE) {
                                if (m.x < m_width && m.y < m_height) {
                                    line(x, y, m.x, m.y);
                                    x = m.x; y = m.y;
                                }
                            }
                            else if (m.message == WM_LBUTTONUP) {
                                break;
                            }
                        }
                    }
                }
                // 控制画矩形
                if (penType == ptRECT)
                {
                }
                // 控制画椭圆
                if (penType == ptELLIPSE)
                {
                }
                // 判断是否点击了按钮
                switch (m_btnIdx)
                {
                case 7: // 编辑颜色
                    break;
                case 10: // 画笔型号
                    InputBox(str, 5, _T("请输入画笔型号(1~5)"));
                    swscanf_s(str, _T("%d"), &m_btnIdx);
                    if (m_btnIdx >= 1 && m_btnIdx <= 5) {
                        m_size = m_btnIdx;
                        setlinestyle(PS_SOLID, m_size);
                    }
                    else {
                        MessageBox(hout, _T("输入错误，不在范围内"), _T("错误提示"), MB_OK);
                    }
                    break;
                case 11: // 橡皮擦
                    drawFocus();
                    draw_sence();
                    m_btnIdx = 11;
                    setlinecolor(BLACK);
                    setfillcolor(WHITE);
                    setlinestyle(PS_SOLID, 1);
                    while (true) {
                        if (peekmessage(&m, EM_MOUSE | EM_KEY)) {
                            if (m.message == WM_LBUTTONDOWN) {
                                while (true) {
                                    if (m.y < m_height - 21) {
                                        x = m.x; 
                                        y = m.y;
                                    }
                                    m = getmessage(EM_MOUSE);
                                    if (m.message == WM_MOUSEMOVE) {
                                        if (m.y < m_height - 21) {
                                            if (con == 0) {
                                                solidcircle(x, y, 21);
                                                fillcircle(m.x, m.y, 20);
                                            }
                                            else {
                                                putimage(x - 20, y - 20, 41, 41, &read_img, x - 20, y - 20);
                                                fillrectangle(m.x - 19, m.y - 19, m.x + 19, m.y + 19);
                                            }
                                        }
                                        else {
                                            if (con == 0)
                                                solidcircle(x, y, 21);
                                            else
                                                putimage(x - 20, y - 20, 41, 41, &read_img, x - 20, y - 20);
                                        }
                                    }
                                    else if (m.message == WM_LBUTTONUP) {
                                        break;
                                    }
                                }
                            }
                            if ((m.message == WM_LBUTTONUP) && (m.y < m_height)) {
                                if (con == 0)
                                    solidcircle(m.x, m.y, 21);
                                else if (con == 1)
                                    putimage(m.x - 20, m.y - 20, 41, 41, &read_img, m.x - 20, m.y - 20);
                                break;
                            }
                        }
                    }
                    setlinecolor(m_focuscolor);
                    penType = ptERASER;
                    break;
                case 12: // 保存
                    saveimage(_T("D:\\效果图.jpg"));
                    loadimage(m_pimg, _T("D:\\效果图.jpg"));
                    getimage(m_pimg, 0, 0, m_width, m_height);
                    saveimage(_T("D:\\效果图.jpg"), m_pimg);
                    break;
                case 13: // 绘图
                    key = 1;
                    con = 0;
                    pen = 1;
                    closegraph();
                    break;
                case 17: // 截图
                    draw_sence();
                    m_btnIdx = 17;
                    store = new IMAGE();
                    while (true)
                    {
                        if (peekmessage(&m, EM_MOUSE | EM_KEY))
                        {
                            if (m.message == WM_LBUTTONDOWN)
                            {
                                x = m.x; y = m.y;
                                xc = m.x; xd = m.y;
                                setlinecolor(WHITE);
                                setlinestyle(PS_SOLID, 1);
                                setrop2(R2_XORPEN);
                                rectangle(x, y, xc, xd);
                                while (true)
                                {
                                    m = getmessage(EM_MOUSE);
                                    if (m.message == WM_MOUSEMOVE)
                                    {
                                        if (m.x < m_width && m.y < m_height)
                                        {
                                            rectangle(x, y, xc, xd);
                                            xc = m.x; xd = m.y;
                                            rectangle(x, y, xc, xd);
                                        }
                                    }
                                    else if (m.message == WM_LBUTTONUP)
                                    {
                                        break;
                                    }
                                }
                            }
                            if ((m.message == WM_LBUTTONUP) && (m.y < m_height))
                            {
                                rectangle(x, y, xc, xd);
                                getimage(store, x, y, m.x - x, m.y - y);
                                getimage(m_pimg, 0, 0, m_width, m_height);
                                saveimage(_T("D:\\截图.jpg"), store);
                                putimage(0, 0, m_pimg);
                                setrop2(R2_COPYPEN);
                                break;
                            }
                        }
                    }
                    delete store;
                    store = NULL;
                    pen = 1;
                    penType = 1;
                    opene = 1;
                    break;
                default: break;
                }
            }
        }
    }
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
