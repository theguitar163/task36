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
                }
                // ���ƻ���Բ
                if (penType == ptELLIPSE)
                {
                }
                // �ж��Ƿ����˰�ť
                switch (m_btnIdx)
                {
                case 7: // �༭��ɫ
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
