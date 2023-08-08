#include <easyx.h>
#include <graphics.h>
#include <math.h>
#include "algorithm.h"
#include "painter.h"

#define PI 3.141592653589793

HWND hout;
int con = 0; // �����л���ͼ��Ͳ�ͼ
int key = 0;
TCHAR szFile[MAX_PATH] = { 0 }; // �洢��ͼƬ��·��

// �ӵ����л�ȡͼƬ
int FileDialog(TCHAR* path)
{
    OPENFILENAME ofn;
    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn); // �ṹ��С
    ofn.lpstrFile = path; // ·��
    ofn.nMaxFile = MAX_PATH; // ·����С
    ofn.lpstrFilter = TEXT("ͼƬ�ļ�(bmp; jpg; png; tiff; tif; jpeg; gif)\0*.bmp; *.jpg; *.png; *.tiff; *.jpeg; *.gif; *.tif\0\0"); // �ļ�����
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST; // ��־
    return GetOpenFileName(&ofn);
}

// ��ʼ����ͼ����
Algorithm::Algorithm(IMAGE* img, int width, int height)
{
    m_pimg = img;
    m_width = width;
    m_height = height;
    m_size = 1;
    for (int i = 0; i < 6; i++)
        initButton(&button[i], 20 + i * 30, height + 15, 10, 0);

    button[6].x = 190; button[6].y = height + 5; button[6].x2 = 215; button[6].y2 = height + 25; button[6].text = L"��"; button[6].type = 1;
    button[7].x = 220; button[7].y = height + 5; button[7].x2 = 290; button[7].y2 = height + 25; button[7].text = L"�༭��ɫ"; button[7].type = 1;
    button[8].x = 300; button[8].y = height + 5; button[8].x2 = 325; button[8].y2 = height + 25; button[8].text = L"��"; button[8].type = 1;
    button[9].x = 335; button[9].y = height + 5; button[9].x2 = 375; button[9].y2 = height + 25; button[9].text = L"��Բ"; button[9].type = 1;
    button[10].x = 385; button[10].y = height + 5; button[10].x2 = 455; button[10].y2 = height + 25; button[10].text = L"�����ͺ�"; button[10].type = 1;
    button[11].x = 470; button[11].y = height + 5; button[11].x2 = 530; button[11].y2 = height + 25; button[11].text = L"��Ƥ��"; button[11].type = 1;
    button[12].x = 545; button[12].y = height + 5; button[12].x2 = 585; button[12].y2 = height + 25; button[12].text = L"����"; button[12].type = 1;
    button[13].x = 595; button[13].y = height + 5; button[13].x2 = 635; button[13].y2 = height + 25; button[13].text = L"�滭"; button[13].type = 1;

    button[14].x = 20; button[14].y = height + 35; button[14].x2 = 90; button[14].y2 = height + 55; button[14].text = L"��ͼƬ"; button[14].type = 1;
    button[15].x = 100; button[15].y = height + 35; button[15].x2 = 170; button[15].y2 = height + 55; button[15].text = L"ˮƽ����"; button[15].type = 1;
    button[16].x = 180; button[16].y = height + 35; button[16].x2 = 250; button[16].y2 = height + 55; button[16].text = L"��ֱ����"; button[16].type = 1;
    button[17].x = 260; button[17].y = height + 35; button[17].x2 = 300; button[17].y2 = height + 55; button[17].text = L"��ͼ"; button[17].type = 1;
    button[18].x = 310; button[18].y = height + 35; button[18].x2 = 400; button[18].y2 = height + 55; button[18].text = L"�ڰ׶�ֵͼ"; button[18].type = 1;
    button[19].x = 410; button[19].y = height + 35; button[19].x2 = 480; button[19].y2 = height + 55; button[19].text = L"��˹ģ��"; button[19].type = 1;
    button[20].x = 490; button[20].y = height + 35; button[20].x2 = 560; button[20].y2 = height + 55; button[20].text = L"�Ҷ�Ч��"; button[20].type = 1;
    button[21].x = 570; button[21].y = height + 35; button[21].x2 = 630; button[21].y2 = height + 55; button[21].text = L"������"; button[21].type = 1;
    m_focusx = button[0].x; 
    m_focusy = button[0].y;
    m_focuscolor = BLUE;
}
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
    int pen = 0;                            // ���ƻ���
    int penType = 0;                            // ���ƻ�����
    int opene = 0;                            // ���ƻ���Բ
    ExMessage m;                        // �����Ϣ
    TCHAR str[10];                        // �ַ�����
    static int x, y;                    // ��¼��һ�����λ��
    int xc, xd;                            // ���ڸ��������Σ���Բ����ͼ��
    IMAGE* store;                        // �洢��ͼͼƬ
    int R[6], G[6], B[6];                // ��ʱ�洢�����ļ���Ϣ

    CHOOSECOLOR stChooseColor;            // ����һ����ɫѡȡ�Ľṹ�����
    COLORREF rgbLineColor = NULL;            // �༭�Ĵ洢ѡ�����ɫ
    COLORREF dwCustColors[16];            // Ϊ�ṹ���������ֵ
    stChooseColor.lStructSize = sizeof(CHOOSECOLOR);
    stChooseColor.hwndOwner = hout;
    stChooseColor.rgbResult = rgbLineColor;
    stChooseColor.lpCustColors = (LPDWORD)dwCustColors;
    stChooseColor.Flags = CC_RGBINIT;
    stChooseColor.lCustData = 0;
    stChooseColor.lpfnHook = NULL;
    stChooseColor.lpTemplateName = NULL;

    setfillcolor(RGB(187, 255, 255)); // Ĭ���ڴ򿪻����ʱ��ѡ�С��ʡ�
    solidrectangle(m_focusx - 12, m_focusy - 12, m_focusx + 13, m_focusy + 13);
    solidrectangle(button[6].x + 1, button[6].y + 1, button[6].x2 - 1, button[6].y2 - 1);
    draw_sence(); // ���Ƴ���
    while (key == 0)
    {
        if (peekmessage(&m, EM_MOUSE | EM_KEY))
        {
            // ��������ж�
            if (m.message == WM_LBUTTONDOWN) {
                // ���ƻ���
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
                // ���ƻ�����
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
                // ���ƻ���Բ
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
                // �ж��Ƿ����˰�ť
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
                    m_btnIdx = 0; // ��ֹ��ǰѡ�еİ�ť��Ŷ�ʧ
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
                case 6: // ��
                    penType = ptLINE;
                    drawFocus();
                    setlinecolor(m_focuscolor);
                    draw_sence();
                    break;
                case 7: // �༭��ɫ
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
                    draw_sence(); // ���ĵ�ǰϲ������ɫ�������»��Ƴ���
                    break;
                case 8: // ������
                    drawFocus();
                    draw_sence();
                    penType = ptRECT;
                    break;
                case 9: // ����Բ
                    drawFocus();
                    draw_sence();
                    penType = ptELLIPSE;
                    break;
                case 10: // �����ͺ�
                    InputBox(str, 5, _T("�����뻭���ͺ�(1~5)"));
                    swscanf_s(str, _T("%d"), &m_btnIdx);
                    if (m_btnIdx >= 1 && m_btnIdx <= 5) {
                        m_size = m_btnIdx;
                        setlinestyle(PS_SOLID, m_size);
                    }
                    else {
                        MessageBox(hout, _T("������󣬲��ڷ�Χ��"), _T("������ʾ"), MB_OK);
                    }
                    break;
                case 11: // ��Ƥ��
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
                case 12: // ����
                    saveimage(_T("D:\\Ч��ͼ.jpg"));
                    loadimage(m_pimg, _T("D:\\Ч��ͼ.jpg"));
                    getimage(m_pimg, 0, 0, m_width, m_height);
                    saveimage(_T("D:\\Ч��ͼ.jpg"), m_pimg);
                    break;
                case 13: // ��ͼ
                    key = 1;
                    con = 0;
                    pen = 1;
                    closegraph();
                    break;
                case 17: // ��ͼ
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
                                saveimage(_T("D:\\��ͼ.jpg"), store);
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
                case 21: // ������
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
    // �������ֵı���ģʽ
    setbkmode(TRANSPARENT);
    settextcolor(BLACK);
    setlinecolor(BLACK);
    setlinestyle(PS_SOLID, 1);
    if (m_width <= 640) line(0, m_height, 640, m_height);
    else line(0, m_height, m_width, m_height);
    // ���ư�ť
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
    // ���ƻ����ͺ��Լ���ǰ��ɫ
    setlinestyle(PS_SOLID, m_size);
    setlinecolor(m_focuscolor);
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
