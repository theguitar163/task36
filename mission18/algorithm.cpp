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
    m_pimg = img;
    m_width = width;
    m_height = height;
    m_size = 1;
    for (int i = 0; i < 6; i++)
        initButton(&button[i], 20 + i * 30, height + 15, 10, 0);

    button[6].x = 190; button[6].y = height + 5; button[6].x2 = 215; button[6].y2 = height + 25; button[6].text = L"笔"; button[6].type = 1;
    button[7].x = 220; button[7].y = height + 5; button[7].x2 = 290; button[7].y2 = height + 25; button[7].text = L"编辑颜色"; button[7].type = 1;
    button[8].x = 300; button[8].y = height + 5; button[8].x2 = 325; button[8].y2 = height + 25; button[8].text = L"□"; button[8].type = 1;
    button[9].x = 335; button[9].y = height + 5; button[9].x2 = 375; button[9].y2 = height + 25; button[9].text = L"椭圆"; button[9].type = 1;
    button[10].x = 385; button[10].y = height + 5; button[10].x2 = 455; button[10].y2 = height + 25; button[10].text = L"画笔型号"; button[10].type = 1;
    button[11].x = 470; button[11].y = height + 5; button[11].x2 = 530; button[11].y2 = height + 25; button[11].text = L"橡皮擦"; button[11].type = 1;
    button[12].x = 545; button[12].y = height + 5; button[12].x2 = 585; button[12].y2 = height + 25; button[12].text = L"保存"; button[12].type = 1;
    button[13].x = 595; button[13].y = height + 5; button[13].x2 = 635; button[13].y2 = height + 25; button[13].text = L"绘画"; button[13].type = 1;

    button[14].x = 20; button[14].y = height + 35; button[14].x2 = 90; button[14].y2 = height + 55; button[14].text = L"打开图片"; button[14].type = 1;
    button[15].x = 100; button[15].y = height + 35; button[15].x2 = 170; button[15].y2 = height + 55; button[15].text = L"水平镜像"; button[15].type = 1;
    button[16].x = 180; button[16].y = height + 35; button[16].x2 = 250; button[16].y2 = height + 55; button[16].text = L"垂直镜像"; button[16].type = 1;
    button[17].x = 260; button[17].y = height + 35; button[17].x2 = 300; button[17].y2 = height + 55; button[17].text = L"截图"; button[17].type = 1;
    button[18].x = 310; button[18].y = height + 35; button[18].x2 = 400; button[18].y2 = height + 55; button[18].text = L"黑白二值图"; button[18].type = 1;
    button[19].x = 410; button[19].y = height + 35; button[19].x2 = 480; button[19].y2 = height + 55; button[19].text = L"高斯模糊"; button[19].type = 1;
    button[20].x = 490; button[20].y = height + 35; button[20].x2 = 560; button[20].y2 = height + 55; button[20].text = L"灰度效果"; button[20].type = 1;
    button[21].x = 570; button[21].y = height + 35; button[21].x2 = 630; button[21].y2 = height + 55; button[21].text = L"马赛克"; button[21].type = 1;
    m_focusx = button[0].x; 
    m_focusy = button[0].y;
    m_focuscolor = BLUE;
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
                    setlinestyle(PS_SOLID, 1);
                    if (m.x < m_width && m.y < m_height)
                    {
                        x = m.x; y = m.y;
                        xc = m.x; xd = m.y;
                        setlinecolor(WHITE);
                        setrop2(R2_XORPEN);
                        rectangle(x, y, xc, xd);
                        while (true) {
                            m = getmessage(EM_MOUSE);
                            if (m.message == WM_MOUSEMOVE) {
                                if (m.x < m_width && m.y < m_height) {
                                    rectangle(x, y, xc, xd);
                                    xc = m.x; xd = m.y;
                                    rectangle(x, y, xc, xd);
                                }
                            }
                            else if (m.message == WM_LBUTTONUP) {
                                setlinecolor(m_focuscolor);
                                setrop2(R2_COPYPEN);
                                rectangle(x, y, xc, xd);
                                break;
                            }
                        }
                    }
                    setlinestyle(PS_SOLID, m_size);
                }
                // 控制画椭圆
                if (penType == ptELLIPSE)
                {
                    setlinestyle(PS_SOLID, 1);
                    if (m.x < m_width && m.y < m_height)
                    {
                        x = m.x; y = m.y;
                        xc = m.x; xd = m.y;
                        setlinecolor(WHITE);
                        setrop2(R2_XORPEN);
                        rectangle(x, y, xc, xd);
                        while (true)
                        {
                            m = getmessage(EM_MOUSE);
                            if (m.message == WM_MOUSEMOVE)
                            {
                                if (m.x < m_width && m.y < m_height)
                                {
                                    ellipse(x, y, xc, xd);
                                    xc = m.x; xd = m.y;
                                    ellipse(x, y, xc, xd);
                                }
                            }
                            else if (m.message == WM_LBUTTONUP)
                            {
                                setlinecolor(m_focuscolor);
                                setrop2(R2_COPYPEN);
                                ellipse(x, y, xc, xd);
                                break;
                            }
                        }
                    }
                    setlinestyle(PS_SOLID, m_size);
                }
                // 判断是否点击了按钮
                for (m_btnIdx = 0; m_btnIdx < 22; m_btnIdx++)
                {
                    if (button[m_btnIdx].type == 0)
                    {
                        if ((m.x - button[m_btnIdx].x) * (m.x - button[m_btnIdx].x) + (m.y - button[m_btnIdx].y) * (m.y - button[m_btnIdx].y) < button[m_btnIdx].radius * button[m_btnIdx].radius)
                        {
                            break;
                        }
                    }
                    else if (button[m_btnIdx].type == 1)
                    {
                        if ((m.x > button[m_btnIdx].x && m.x < button[m_btnIdx].x2) && (m.y > button[m_btnIdx].y && m.y < button[m_btnIdx].y2))
                        {
                            break;
                        }
                    }
                }
                switch (m_btnIdx)
                {
                case 0:
                    drawFocus();
                    draw_sence();
                    m_btnIdx = 0; // 防止当前选中的按钮序号丢失
                    setlinecolor(button[m_btnIdx].color); break;
                case 1:
                    drawFocus();
                    draw_sence();
                    m_btnIdx = 1;
                    setlinecolor(button[m_btnIdx].color); break;
                case 2:
                    drawFocus();
                    draw_sence();
                    m_btnIdx = 2;
                    setlinecolor(button[m_btnIdx].color); break;
                case 3:
                    drawFocus();
                    draw_sence();
                    m_btnIdx = 3;
                    setlinecolor(button[m_btnIdx].color); break;
                case 4:
                    drawFocus();
                    draw_sence();
                    m_btnIdx = 4;
                    setlinecolor(button[m_btnIdx].color); break;
                case 5:
                    drawFocus();
                    draw_sence();
                    m_btnIdx = 5;
                    setlinecolor(button[m_btnIdx].color); break;
                case 6: // 笔
                    penType = ptLINE;
                    drawFocus();
                    setlinecolor(m_focuscolor);
                    draw_sence();
                    break;
                case 7: // 编辑颜色
                    if (ChooseColor(&stChooseColor)) {
                        rgbLineColor = stChooseColor.rgbResult;
                        setlinecolor(rgbLineColor);
                    }
                    for (m_btnIdx = 0; m_btnIdx < 6; m_btnIdx++) {
                        if (m_focusx == button[m_btnIdx].x) {
                            button[m_btnIdx].color = RGB(GetRValue(rgbLineColor), GetGValue(rgbLineColor), GetBValue(rgbLineColor));
                        }
                        R[m_btnIdx] = GetRValue(button[m_btnIdx].color);
                        G[m_btnIdx] = GetGValue(button[m_btnIdx].color);
                        B[m_btnIdx] = GetBValue(button[m_btnIdx].color);
                    }
                    draw_sence(); // 更改当前喜欢的颜色，并重新绘制场景
                    break;
                case 8: // 画矩形
                    drawFocus();
                    draw_sence();
                    penType = ptRECT;
                    break;
                case 9: // 画椭圆
                    drawFocus();
                    draw_sence();
                    penType = ptELLIPSE;
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
                                setfillcolor(WHITE);
                                solidrectangle(button[m_btnIdx].x + 1, button[m_btnIdx].y + 1, button[m_btnIdx].x2 - 1, button[m_btnIdx].y2 - 1);
                                outtextxy(button[m_btnIdx].x + 5, button[m_btnIdx].y + 2, button[m_btnIdx].text);
                                setlinecolor(m_focuscolor);
                                setlinestyle(PS_SOLID, m_size);
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
                case 21: // 马赛克
                    draw_sence();
                    m_btnIdx = 21;
                    while (true)
                    {
                        if (peekmessage(&m, EM_MOUSE | EM_KEY)) {
                            if (m.message == WM_LBUTTONDOWN) {
                                while (true) {
                                    m = getmessage(EM_MOUSE);
                                    if (m.message == WM_MOUSEMOVE) {
                                        if (m.y < m_height - 12) {
                                            for (int w = -12; w < 12; w = w + 4) {
                                                for (int n = -12; n < 12; n = n + 4) {
                                                    setfillcolor(getpixel(m.x + w + rand() % 4, m.y + n + rand() % 4));
                                                    solidrectangle(m.x + w, m.y + n, m.x + w + 4, m.y + n + 4);
                                                }
                                            }
                                        }
                                    }
                                    else if (m.message == WM_LBUTTONUP) {
                                        break;
                                    }
                                }
                            }
                            if ((m.message == WM_LBUTTONUP) && (m.y < m_height)) {
                                setfillcolor(WHITE);
                                solidrectangle(button[m_btnIdx].x + 1, button[m_btnIdx].y + 1, button[m_btnIdx].x2 - 1, button[m_btnIdx].y2 - 1);
                                outtextxy(button[m_btnIdx].x + 5, button[m_btnIdx].y + 2, button[m_btnIdx].text);
                                break;
                            }
                        }
                    }
                    penType = ptMOSAIC;
                    break;
                default: break;
                }
            }
        }
    }
}
void Algorithm::draw_sence()
{
    COLORREF color = NULL;
    // 设置文字的背景模式
    setbkmode(TRANSPARENT);
    settextcolor(BLACK);
    setlinecolor(BLACK);
    setlinestyle(PS_SOLID, 1);
    if (m_width <= 640) line(0, m_height, 640, m_height);
    else line(0, m_height, m_width, m_height);
    // 绘制按钮
    for (m_btnIdx = 0; m_btnIdx < 6; m_btnIdx++) {
        setfillcolor(button[m_btnIdx].color);
        fillcircle(button[m_btnIdx].x, button[m_btnIdx].y, button[m_btnIdx].radius);
    }
    for (m_btnIdx = 6; m_btnIdx < 22; m_btnIdx++) {
        rectangle(button[m_btnIdx].x, button[m_btnIdx].y, button[m_btnIdx].x2, button[m_btnIdx].y2);
        outtextxy(button[m_btnIdx].x + 5, button[m_btnIdx].y + 2, button[m_btnIdx].text);
    }
    if (m_width <= 640)
        line(0, m_height + 30, 640, m_height + 30);
    else
        line(0, m_height + 30, m_width, m_height + 30);
    // 控制画笔型号以及当前颜色
    setlinestyle(PS_SOLID, m_size);
    setlinecolor(m_focuscolor);
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
