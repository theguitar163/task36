#include <easyx.h>
#include <graphics.h>
#include <math.h>
#include "algorithm.h"

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
    pimg = img;
    Width = width;
    Height = height;
    size = 1;
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
    bluex = button[0].x; bluey = button[0].y;
    bluecolor = RGB(GetPrivateProfileInt(_T("COLOR0"), _T("R"), 0, _T("color.ini")), GetPrivateProfileInt(_T("COLOR0"), _T("G"), 0, _T("color.ini")), GetPrivateProfileInt(_T("COLOR0"), _T("B"), 0, _T("color.ini")));
}
// ѡȡ״̬
void Algorithm::drawFocus()
{
    if (i < 6) // ���Ƶ�ǰ��ɫ��ѡȡ״̬
    {
        setfillcolor(RGB(255, 255, 255));
        solidrectangle(bluex - 12, bluey - 12, bluex + 13, bluey + 13);
        bluex = button[i].x;
        bluey = button[i].y;
        bluecolor = button[i].color;
        setfillcolor(RGB(187, 255, 255));
        solidrectangle(bluex - 12, bluey - 12, bluex + 13, bluey + 13);
    }
    else // Ϊ��ǰѡ�а�ť���Ƶ���ɫ������Ϊѡȡ״̬
    {
        setfillcolor(RGB(187, 255, 255));
        solidrectangle(button[i].x + 1, button[i].y + 1, button[i].x2 - 1, button[i].y2 - 1);
        outtextxy(button[i].x + 5, button[i].y + 2, button[i].text);
    }
}
// ��ȡ��ִ����Ϣ
void Algorithm::message_proce(IMAGE read_img)
{
    int pen = 0;                            // ���ƻ���
    int openr = 0;                            // ���ƻ�����
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
    solidrectangle(bluex - 12, bluey - 12, bluex + 13, bluey + 13);
    solidrectangle(button[6].x + 1, button[6].y + 1, button[6].x2 - 1, button[6].y2 - 1);
    draw_sence(); // ���Ƴ���
    while (key == 0)
    {
        if (peekmessage(&m, EM_MOUSE | EM_KEY))
        {
            // ��������ж�
            if (m.message == WM_LBUTTONDOWN) {
                // ���ƻ���
                if (pen == 0) {
                    if (m.x < Width && m.y < Height)
                    {
                        x = m.x; y = m.y;
                        while (true)
                        {
                            m = getmessage(EM_MOUSE);
                            if (m.message == WM_MOUSEMOVE)
                            {
                                if (m.x < Width && m.y < Height)
                                {
                                    line(x, y, m.x, m.y);
                                    x = m.x; y = m.y;
                                }
                            }
                            else if (m.message == WM_LBUTTONUP)
                            {
                                break;
                            }

                        }
                    }
                }
                // ���ƻ�����
                if (openr == 0)
                {
                    setlinestyle(PS_SOLID, 1);
                    if (m.x < Width && m.y < Height)
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
                                if (m.x < Width && m.y < Height)
                                {
                                    rectangle(x, y, xc, xd);
                                    xc = m.x; xd = m.y;
                                    rectangle(x, y, xc, xd);
                                }
                            }
                            else if (m.message == WM_LBUTTONUP)
                            {
                                setlinecolor(bluecolor);
                                setrop2(R2_COPYPEN);
                                rectangle(x, y, xc, xd);
                                break;
                            }
                        }
                    }
                    setlinestyle(PS_SOLID, size);
                }
                // ���ƻ���Բ
                if (opene == 0)
                {
                    setlinestyle(PS_SOLID, 1);
                    if (m.x < Width && m.y < Height)
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
                                if (m.x < Width && m.y < Height)
                                {
                                    ellipse(x, y, xc, xd);
                                    xc = m.x; xd = m.y;
                                    ellipse(x, y, xc, xd);
                                }
                            }
                            else if (m.message == WM_LBUTTONUP)
                            {
                                setlinecolor(bluecolor);
                                setrop2(R2_COPYPEN);
                                ellipse(x, y, xc, xd);
                                break;
                            }
                        }
                    }
                    setlinestyle(PS_SOLID, size);
                }
                // �ж��Ƿ����˰�ť
                for (i = 0; i < 22; i++)
                {
                    if (button[i].type == 0)
                    {
                        if ((m.x - button[i].x) * (m.x - button[i].x) + (m.y - button[i].y) * (m.y - button[i].y) < button[i].radius * button[i].radius)
                        {
                            break;
                        }
                    }
                    else if (button[i].type == 1)
                    {
                        if ((m.x > button[i].x && m.x < button[i].x2) && (m.y > button[i].y && m.y < button[i].y2))
                        {
                            break;
                        }
                    }
                }
                switch (i)
                {
                case 0:
                    drawFocus();
                    draw_sence();
                    i = 0; // ��ֹ��ǰѡ�еİ�ť��Ŷ�ʧ
                    setlinecolor(button[i].color); break;
                case 1:
                    drawFocus();
                    draw_sence();
                    i = 1;
                    setlinecolor(button[i].color); break;
                case 2:
                    drawFocus();
                    draw_sence();
                    i = 2;
                    setlinecolor(button[i].color); break;
                case 3:
                    drawFocus();
                    draw_sence();
                    i = 3;
                    setlinecolor(button[i].color); break;
                case 4:
                    drawFocus();
                    draw_sence();
                    i = 4;
                    setlinecolor(button[i].color); break;
                case 5:
                    drawFocus();
                    draw_sence();
                    i = 5;
                    setlinecolor(button[i].color); break;
                case 6: // ��
                    pen = 0;
                    openr = 1;
                    opene = 1;
                    setfillcolor(WHITE);
                    solidrectangle(button[8].x + 1, button[8].y + 1, button[8].x2 - 1, button[8].y2 - 1);
                    solidrectangle(button[9].x + 1, button[9].y + 1, button[9].x2 - 1, button[9].y2 - 1);
                    outtextxy(button[8].x + 5, button[8].y + 2, button[8].text);
                    outtextxy(button[9].x + 5, button[9].y + 2, button[9].text);
                    drawFocus();
                    setlinecolor(bluecolor);
                    draw_sence();
                    break;
                case 7: // �༭��ɫ
                    if (ChooseColor(&stChooseColor))
                    {
                        rgbLineColor = stChooseColor.rgbResult;
                        setlinecolor(rgbLineColor);
                    }
                    for (i = 0; i < 6; i++)
                    {
                        if (bluex == button[i].x)
                        {
                            button[i].color = RGB(GetRValue(rgbLineColor), GetGValue(rgbLineColor), GetBValue(rgbLineColor));
                        }
                        R[i] = GetRValue(button[i].color);
                        G[i] = GetGValue(button[i].color);
                        B[i] = GetBValue(button[i].color);
                    }
                    // д�����ļ�����¼ϲ������ɫ
                    swprintf_s(str, _T("%d"), R[0]);
                    ::WritePrivateProfileString(_T("COLOR0"), _T("R"), str, _T("color.ini"));
                    swprintf_s(str, _T("%d"), G[0]);
                    ::WritePrivateProfileString(_T("COLOR0"), _T("G"), str, _T("color.ini"));
                    swprintf_s(str, _T("%d"), B[0]);
                    ::WritePrivateProfileString(_T("COLOR0"), _T("B"), str, _T("color.ini"));
                    swprintf_s(str, _T("%d"), R[1]);
                    ::WritePrivateProfileString(_T("COLOR1"), _T("R"), str, _T("color.ini"));
                    swprintf_s(str, _T("%d"), G[1]);
                    ::WritePrivateProfileString(_T("COLOR1"), _T("G"), str, _T("color.ini"));
                    swprintf_s(str, _T("%d"), B[1]);
                    ::WritePrivateProfileString(_T("COLOR1"), _T("B"), str, _T("color.ini"));
                    swprintf_s(str, _T("%d"), R[2]);
                    ::WritePrivateProfileString(_T("COLOR2"), _T("R"), str, _T("color.ini"));
                    swprintf_s(str, _T("%d"), G[2]);
                    ::WritePrivateProfileString(_T("COLOR2"), _T("G"), str, _T("color.ini"));
                    swprintf_s(str, _T("%d"), B[2]);
                    ::WritePrivateProfileString(_T("COLOR2"), _T("B"), str, _T("color.ini"));
                    swprintf_s(str, _T("%d"), R[3]);
                    ::WritePrivateProfileString(_T("COLOR3"), _T("R"), str, _T("color.ini"));
                    swprintf_s(str, _T("%d"), G[3]);
                    ::WritePrivateProfileString(_T("COLOR3"), _T("G"), str, _T("color.ini"));
                    swprintf_s(str, _T("%d"), B[3]);
                    ::WritePrivateProfileString(_T("COLOR3"), _T("B"), str, _T("color.ini"));
                    swprintf_s(str, _T("%d"), R[4]);
                    ::WritePrivateProfileString(_T("COLOR4"), _T("R"), str, _T("color.ini"));
                    swprintf_s(str, _T("%d"), G[4]);
                    ::WritePrivateProfileString(_T("COLOR4"), _T("G"), str, _T("color.ini"));
                    swprintf_s(str, _T("%d"), B[4]);
                    ::WritePrivateProfileString(_T("COLOR4"), _T("B"), str, _T("color.ini"));
                    swprintf_s(str, _T("%d"), R[5]);
                    ::WritePrivateProfileString(_T("COLOR5"), _T("R"), str, _T("color.ini"));
                    swprintf_s(str, _T("%d"), G[5]);
                    ::WritePrivateProfileString(_T("COLOR5"), _T("G"), str, _T("color.ini"));
                    swprintf_s(str, _T("%d"), B[5]);
                    ::WritePrivateProfileString(_T("COLOR5"), _T("B"), str, _T("color.ini"));
                    draw_sence(); // ���ĵ�ǰϲ������ɫ�������»��Ƴ���
                    break;
                case 8: // ������
                    drawFocus();
                    draw_sence();
                    openr = 0;
                    pen = 1;
                    opene = 1;
                    setfillcolor(WHITE);
                    solidrectangle(button[6].x + 1, button[6].y + 1, button[6].x2 - 1, button[6].y2 - 1);
                    solidrectangle(button[9].x + 1, button[9].y + 1, button[9].x2 - 1, button[9].y2 - 1);
                    outtextxy(button[6].x + 5, button[6].y + 2, button[6].text);
                    outtextxy(button[9].x + 5, button[9].y + 2, button[9].text);
                    break;
                case 9: // ����Բ
                    drawFocus();
                    draw_sence();
                    openr = 1;
                    pen = 1;
                    opene = 0;
                    setfillcolor(WHITE);
                    solidrectangle(button[6].x + 1, button[6].y + 1, button[6].x2 - 1, button[6].y2 - 1);
                    solidrectangle(button[8].x + 1, button[8].y + 1, button[8].x2 - 1, button[8].y2 - 1);
                    outtextxy(button[6].x + 5, button[6].y + 2, button[6].text);
                    outtextxy(button[8].x + 5, button[8].y + 2, button[8].text);
                    break;
                case 10: // �����ͺ�
                    InputBox(str, 5, _T("�����뻭���ͺ�(1~5)"));
                    swscanf_s(str, _T("%d"), &i);
                    if (i >= 1 && i <= 5)
                    {
                        size = i;
                        setlinestyle(PS_SOLID, size);
                    }
                    else
                    {
                        MessageBox(hout, _T("������󣬲��ڷ�Χ��"), _T("������ʾ"), MB_OK);
                    }
                    break;
                case 11: // ��Ƥ��
                    setfillcolor(WHITE);
                    solidrectangle(button[6].x + 1, button[6].y + 1, button[6].x2 - 1, button[6].y2 - 1);
                    solidrectangle(button[8].x + 1, button[8].y + 1, button[8].x2 - 1, button[8].y2 - 1);
                    solidrectangle(button[9].x + 1, button[9].y + 1, button[9].x2 - 1, button[9].y2 - 1);
                    outtextxy(button[6].x + 5, button[6].y + 2, button[6].text);
                    outtextxy(button[8].x + 5, button[8].y + 2, button[8].text);
                    outtextxy(button[9].x + 5, button[9].y + 2, button[9].text);
                    drawFocus();
                    draw_sence();
                    i = 11;
                    setlinecolor(BLACK);
                    setfillcolor(WHITE);
                    setlinestyle(PS_SOLID, 1);
                    while (true)
                    {
                        if (peekmessage(&m, EM_MOUSE | EM_KEY))
                        {
                            if (m.message == WM_LBUTTONDOWN)
                            {
                                while (true)
                                {
                                    if (m.y < Height - 21)
                                    {
                                        x = m.x; y = m.y;
                                    }
                                    m = getmessage(EM_MOUSE);
                                    if (m.message == WM_MOUSEMOVE)
                                    {
                                        if (m.y < Height - 21)
                                        {
                                            if (con == 0)
                                            {
                                                solidcircle(x, y, 21);
                                                fillcircle(m.x, m.y, 20);
                                            }
                                            else
                                            {
                                                putimage(x - 20, y - 20, 41, 41, &read_img, x - 20, y - 20);
                                                fillrectangle(m.x - 19, m.y - 19, m.x + 19, m.y + 19);
                                            }
                                        }
                                        else
                                        {
                                            if (con == 0)
                                            {
                                                solidcircle(x, y, 21);
                                            }
                                            else
                                            {
                                                putimage(x - 20, y - 20, 41, 41, &read_img, x - 20, y - 20);
                                            }
                                        }
                                    }
                                    else if (m.message == WM_LBUTTONUP)
                                    {
                                        break;
                                    }
                                }
                            }
                            if ((m.message == WM_LBUTTONUP) && (m.y < Height))
                            {
                                if (con == 0)solidcircle(m.x, m.y, 21);
                                else if (con == 1)putimage(m.x - 20, m.y - 20, 41, 41, &read_img, m.x - 20, m.y - 20);
                                solidrectangle(button[i].x + 1, button[i].y + 1, button[i].x2 - 1, button[i].y2 - 1);
                                outtextxy(button[i].x + 5, button[i].y + 2, button[i].text);
                                setlinestyle(PS_SOLID, size);
                                break;
                            }
                        }
                    }
                    setlinecolor(bluecolor);
                    pen = 1;
                    openr = 1;
                    opene = 1;
                    break;
                case 12: // ����
                    saveimage(_T("D:\\Ч��ͼ.jpg"));
                    loadimage(pimg, _T("D:\\Ч��ͼ.jpg"));
                    getimage(pimg, 0, 0, Width, Height);
                    saveimage(_T("D:\\Ч��ͼ.jpg"), pimg);
                    pen = 1;
                    opene = 1;
                    openr = 1;
                    break;
                case 13: // ��ͼ
                    key = 1;
                    con = 0;
                    pen = 1;
                    closegraph();
                    break;
                case 14: // ��ͼƬ
                    if (FileDialog(szFile)) {
                        con = 1;
                        key = 1;
                    }
                    pen = 1;
                    break;
                case 15: // ˮƽ����
                    per_img(read_img);
                    pen = 1;
                    openr = 1;
                    opene = 1;
                    break;
                case 16: // ��ֱ����
                    ver_img(read_img);
                    pen = 1;
                    openr = 1;
                    opene = 1;
                    break;
                case 17: // ��ͼ
                    draw_sence();
                    i = 17;
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
                                        if (m.x < Width && m.y < Height)
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
                            if ((m.message == WM_LBUTTONUP) && (m.y < Height))
                            {
                                rectangle(x, y, xc, xd);
                                getimage(store, x, y, m.x - x, m.y - y);
                                getimage(pimg, 0, 0, Width, Height);
                                saveimage(_T("D:\\��ͼ.jpg"), store);
                                putimage(0, 0, pimg);
                                setrop2(R2_COPYPEN);
                                setfillcolor(WHITE);
                                solidrectangle(button[i].x + 1, button[i].y + 1, button[i].x2 - 1, button[i].y2 - 1);
                                outtextxy(button[i].x + 5, button[i].y + 2, button[i].text);
                                setlinecolor(bluecolor);
                                setlinestyle(PS_SOLID, size);
                                break;
                            }
                        }
                    }
                    delete store;
                    store = NULL;
                    pen = 1;
                    openr = 1;
                    opene = 1;
                    break;
                case 18: // �ڰ׶�ֵͼ
                    Black_White();
                    pen = 1;
                    openr = 1;
                    opene = 1;
                    break;
                case 19: // ��˹ģ��
                    getGaussianArray();
                    pen = 1;
                    openr = 1;
                    opene = 1;
                    break;
                case 20: // �Ҷ�Ч��
                    Gray();
                    pen = 1;
                    openr = 1;
                    opene = 1;
                    break;
                case 21: // ������
                    draw_sence();
                    i = 21;
                    while (true)
                    {
                        if (peekmessage(&m, EM_MOUSE | EM_KEY))
                        {
                            if (m.message == WM_LBUTTONDOWN)
                            {
                                while (true)
                                {
                                    m = getmessage(EM_MOUSE);
                                    if (m.message == WM_MOUSEMOVE)
                                    {
                                        if (m.y < Height - 12)
                                        {
                                            for (int w = -12; w < 12; w = w + 4)
                                            {
                                                for (int n = -12; n < 12; n = n + 4)
                                                {
                                                    setfillcolor(getpixel(m.x + w + rand() % 4, m.y + n + rand() % 4));
                                                    solidrectangle(m.x + w, m.y + n, m.x + w + 4, m.y + n + 4);
                                                }
                                            }
                                        }
                                    }
                                    else if (m.message == WM_LBUTTONUP)
                                    {
                                        break;
                                    }
                                }
                            }
                            if ((m.message == WM_LBUTTONUP) && (m.y < Height))
                            {
                                setfillcolor(WHITE);
                                solidrectangle(button[i].x + 1, button[i].y + 1, button[i].x2 - 1, button[i].y2 - 1);
                                outtextxy(button[i].x + 5, button[i].y + 2, button[i].text);
                                break;
                            }
                        }
                    }
                    pen = 1;
                    openr = 1;
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
    COLORREF color = NULL;
    int R[6], G[6], B[6];
    // �������ļ��ж�ȡ��ɫ��Ϣ
    R[0] = GetPrivateProfileInt(_T("COLOR0"), _T("R"), 0, _T("color.ini"));
    G[0] = GetPrivateProfileInt(_T("COLOR0"), _T("G"), 0, _T("color.ini"));
    B[0] = GetPrivateProfileInt(_T("COLOR0"), _T("B"), 0, _T("color.ini"));
    R[1] = GetPrivateProfileInt(_T("COLOR1"), _T("R"), 255, _T("color.ini"));
    G[1] = GetPrivateProfileInt(_T("COLOR1"), _T("G"), 0, _T("color.ini"));
    B[1] = GetPrivateProfileInt(_T("COLOR1"), _T("B"), 0, _T("color.ini"));
    R[2] = GetPrivateProfileInt(_T("COLOR2"), _T("R"), 0, _T("color.ini"));
    G[2] = GetPrivateProfileInt(_T("COLOR2"), _T("G"), 255, _T("color.ini"));
    B[2] = GetPrivateProfileInt(_T("COLOR2"), _T("B"), 0, _T("color.ini"));
    R[3] = GetPrivateProfileInt(_T("COLOR3"), _T("R"), 0, _T("color.ini"));
    G[3] = GetPrivateProfileInt(_T("COLOR3"), _T("G"), 0, _T("color.ini"));
    B[3] = GetPrivateProfileInt(_T("COLOR3"), _T("B"), 255, _T("color.ini"));
    R[4] = GetPrivateProfileInt(_T("COLOR4"), _T("R"), 160, _T("color.ini"));
    G[4] = GetPrivateProfileInt(_T("COLOR4"), _T("G"), 32, _T("color.ini"));
    B[4] = GetPrivateProfileInt(_T("COLOR4"), _T("B"), 240, _T("color.ini"));
    R[5] = GetPrivateProfileInt(_T("COLOR5"), _T("R"), 255, _T("color.ini"));
    G[5] = GetPrivateProfileInt(_T("COLOR5"), _T("G"), 215, _T("color.ini"));
    B[5] = GetPrivateProfileInt(_T("COLOR5"), _T("B"), 0, _T("color.ini"));
    for (int j = 0; j < 6; j++) {
        button[j].color = RGB(R[j], G[j], B[j]);
    }
    // �������ֵı���ģʽ
    setbkmode(TRANSPARENT);
    settextcolor(BLACK);
    setlinecolor(BLACK);
    setlinestyle(PS_SOLID, 1);
    if (Width <= 640) line(0, Height, 640, Height);
    else line(0, Height, Width, Height);
    // ���ư�ť
    for (i = 0; i < 6; i++) {
        setfillcolor(button[i].color);
        fillcircle(button[i].x, button[i].y, button[i].radius);
    }
    for (i = 6; i < 22; i++) {
        rectangle(button[i].x, button[i].y, button[i].x2, button[i].y2);
        outtextxy(button[i].x + 5, button[i].y + 2, button[i].text);
    }
    if (Width <= 640)
        line(0, Height + 30, 640, Height + 30);
    else
        line(0, Height + 30, Width, Height + 30);
    // ���ƻ����ͺ��Լ���ǰ��ɫ
    setlinestyle(PS_SOLID, size);
    setlinecolor(bluecolor);
}
// ˮƽ����
void Algorithm::per_img(IMAGE read_img)
{
    setfillcolor(WHITE);
    solidrectangle(button[6].x + 1, button[6].y + 1, button[6].x2 - 1, button[6].y2 - 1);
    solidrectangle(button[8].x + 1, button[8].y + 1, button[8].x2 - 1, button[8].y2 - 1);
    solidrectangle(button[9].x + 1, button[9].y + 1, button[9].x2 - 1, button[9].y2 - 1);
    outtextxy(button[6].x + 5, button[6].y + 2, button[6].text);
    outtextxy(button[8].x + 5, button[8].y + 2, button[8].text);
    outtextxy(button[9].x + 5, button[9].y + 2, button[9].text);
    if (con == 0)
    {
        getimage(pimg, 0, 0, 640, 480);
        getimage(&read_img, 0, 0, 640, 480);
    }
    else if (con == 1)
    {
        getimage(&read_img, 0, 0, Width, Height);
    }
    DWORD* pMem1 = GetImageBuffer(pimg);
    DWORD* pMem2 = GetImageBuffer(&read_img);
    for (i = 0; i < Height; i++)
    {
        for (int j = 0; j < Width; j++)
        {
            pMem1[i * Width + j] = pMem2[i * Width + Width - 1 - j];
        }
    }
    putimage(0, 0, pimg);
}
// ��ֱ����
void Algorithm::ver_img(IMAGE read_img)
{
    if (con == 0) {
        getimage(pimg, 0, 0, 640, 480);
        getimage(&read_img, 0, 0, 640, 480);
    }
    else if (con == 1) {
        getimage(&read_img, 0, 0, Width, Height);
    }
    DWORD* pMem1 = GetImageBuffer(pimg);
    DWORD* pMem2 = GetImageBuffer(&read_img);
    for (i = 0; i < Height; i++) {
        for (int j = 0; j < Width; j++) {
            pMem1[(Height - 1 - i) * Width + j] = pMem2[i * Width + j];
        }
    }
    putimage(0, 0, pimg);
}
// �Ҷ�ͼ��
void Algorithm::Gray()
{
    if (con == 0)
    {
        getimage(pimg, 0, 0, 640, 480);
    }
    DWORD* p = GetImageBuffer(pimg);
    COLORREF c;
    for (i = pimg->getwidth() * pimg->getheight() - 1; i >= 0; i--)
    {
        c = BGR(p[i]);
        c = (GetRValue(c) * 299 + GetGValue(c) * 587 + GetBValue(c) * 114 + 500) / 1000;
        p[i] = RGB(c, c, c);        // �����ǻҶ�ֵ��������ִ�� BGR ת��
    }
    putimage(0, 0, pimg);
}
// �ڰ׶�ֵ
void Algorithm::Black_White()
{
    int j;
    TCHAR str[10];
    if (con == 0)
    {
        getimage(pimg, 0, 0, 640, 480);
    }
    Gray();
    InputBox(str, 5, _T("�����뷧ֵ(1~254)"));
    swscanf_s(str, _T("%d"), &j);
    if (j >= 1 && j <= 254)
    {
        DWORD* p = GetImageBuffer(pimg);
        COLORREF c;
        for (i = pimg->getwidth() * pimg->getheight() - 1; i >= 0; i--)
        {
            c = BGR(p[i]);
            if (GetRValue(c) < j)c = 0;
            else if (GetRValue(c) >= j)c = 255;
            p[i] = RGB(c, c, c);
        }
        putimage(0, 0, pimg);
    }
    else
    {
        MessageBox(hout, _T("������󣬲��ڷ�Χ��"), _T("������ʾ"), MB_OK);
    }
}
// ��˹ģ��
void Algorithm::Gauss()
{
    int j, k, n, m;
    BYTE* R;
    BYTE* G;
    BYTE* B;
    double SR = 0, SG = 0, SB = 0;
    R = (BYTE*)calloc(Height * Width, sizeof(BYTE));
    G = (BYTE*)calloc(Height * Width, sizeof(BYTE));
    B = (BYTE*)calloc(Height * Width, sizeof(BYTE));
    if (con == 0)
    {
        getimage(pimg, 0, 0, 640, 480);
    }
    DWORD* pMem = GetImageBuffer(pimg);
    for (j = 0; j < Height * Width; j++)
    {
        R[j] = GetRValue(pMem[j]);
        G[j] = GetGValue(pMem[j]);
        B[j] = GetBValue(pMem[j]);
    }
    for (j = i / 2; j < Height - i / 2 - 1; j++)
    {
        for (k = i / 2; k < Width - i / 2 - 1; k++)
        {
            for (n = 0; n < i; n++)
            {
                for (m = 0; m < i; m++)
                {
                    SR += R[(j - i / 2 + n) * Width + k - i / 2 + m] * Gaussian_Ker[n][m];
                    SG += G[(j - i / 2 + n) * Width + k - i / 2 + m] * Gaussian_Ker[n][m];
                    SB += B[(j - i / 2 + n) * Width + k - i / 2 + m] * Gaussian_Ker[n][m];
                }
            }
            pMem[j * Width + k] = RGB(SR, SG, SB);
            SR = SB = SG = 0;
        }
    }
    putimage(0, 0, pimg);
    free(R);
    free(G);
    free(B);
    R = G = B = NULL;
}
// ��ȡ�˲���
void Algorithm::getGaussianArray()
{
    int j, k, d;
    double sigma;
    double sigma2;
    double a, sum = 0;
    TCHAR str[10];
    InputBox(str, 5, _T("�������˲����ڴ�С(3~15 ����)"));
    swscanf_s(str, _T("%d"), &i);
    if (i >= 3 && i <= 15 && (i % 2 == 1))
    {
        Gaussian_Ker = (double**)calloc(i, sizeof(double*));
        for (j = 0; j < i; j++)
            Gaussian_Ker[j] = (double*)calloc(i, sizeof(double));
        InputBox(str, 5, _T("�������˲����ڴ�С(0.5~3.5)"));
        swscanf_s(str, _T("%lf"), &sigma);
        if (sigma >= 0.5 && sigma <= 3.5)
        {
            sigma2 = sigma * sigma;
            a = 1.0 / (2 * PI * sigma2);
            for (j = 0; j < i; j++)
            {
                for (k = 0; k < i; k++)
                {
                    d = (j - i / 2) * (j - i / 2) + (k - i / 2) * (k - i / 2);
                    Gaussian_Ker[j][k] = a * exp(-d / (2.0 * sigma2));
                    sum += Gaussian_Ker[j][k];
                }
            }
            for (j = 0; j < i; j++)
            {
                for (k = 0; k < i; k++)
                {
                    Gaussian_Ker[j][k] = Gaussian_Ker[j][k] / sum;
                }
            }
            Gauss();
        }
        else
        {
            MessageBox(hout, _T("������󣬲��ڷ�Χ��"), _T("������ʾ"), MB_OK);
        }
    }
    else
    {
        MessageBox(hout, _T("������󣬲��ڷ�Χ��"), _T("������ʾ"), MB_OK);
    }
}
