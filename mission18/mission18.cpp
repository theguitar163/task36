// 任务：基于点阵图像的绘图板。
// 点阵图又称位图或栅格图，整个图像由 M 行、N 列的像素点构成。
// 在点阵图里画一条线，会引起若干像素颜色变化。
// 点阵图只存储每个像素的颜色，不记录引起像素颜色改变的是直线还是圆。
// 1.实现一个基于点阵图象的绘图板。需要实现的功能包括：
//   选择颜色、画直线、画圆、画矩形、保存图片、加载图片。
//   保存和加载图片不考虑多种尺寸的情况，只处理一种固定的尺寸即可。
// 2.在该任务的基础上，增加功能：
//   鼠标在图像上选取一个矩形区域，然后可以对该区域应用图像滤镜，包括：
//   马赛克效果、高斯模糊效果、灰度效果、黑白二值图效果等。

#include <graphics.h>
#include <stdio.h>
#include <conio.h>
#include <cmath>
#include <cstdlib>
#include <commdlg.h>

HWND hout;
int con = 0; // 控制切换绘图板和插图
int key = 0;
const double PI = acos(-1.0);
TCHAR szFile[MAX_PATH] = { 0 }; // 存储打开图片的路径

#define btCIRCLE  0
#define btRECT    1
#define btRDRECT  2

#define MAX_BUTTON 100

#define alTOP    0
#define alBOTTOM 1
#define alLEFT   2
#define alRIGHT  3

// 函数指针类型
typedef void (TFunction)();

// 按钮结构
typedef struct tagButton {
    TCHAR* text;          // 按钮上的文字
    COLORREF color;           // 按钮的颜色
    int type;                 // 按钮类型btCIRCLE\btRDRECT\btRECT
    int x, y, x2, y2;         // 按钮的坐标
    int w, h;
    int radius;               // 圆形按钮的半径    
    struct tagPanel* container;
    TFunction* pfun = NULL;
} TButton;

// 简易控制面板
// 可在上面添加按钮（因为使用纯C，不太方便实现抽象类多态添加其他控件）
typedef struct tagPanel {
    int x;
    int y;
    int w;
    int h;
    TButton* pbuttons[MAX_BUTTON];
    int btnCount = 0;
} TPanel;

void initButton(TButton* pbtn, int x, int y, int x2, int y2, COLORREF color, TCHAR* text, int mod)
{
    pbtn->x = x;
    pbtn->y = y;
    pbtn->x2 = x2;
    pbtn->y2 = y2;
    pbtn->w = x2 - x;
    pbtn->h = y2 - y;
    pbtn->color = color;
    pbtn->text = text;
 //   wcscpy_s(pbtn->text, text);
    pbtn->type = mod;
}

void initButton(TButton* pbtn, int x1, int y1, int radius, int mod)
{
    pbtn->x = x1;
    pbtn->y = y1;
    pbtn->radius = radius;
    pbtn->type = mod;
}

void initButton(TButton* pbtn, int x, int y, int w, int h, int mod)
{
    pbtn->x = x;
    pbtn->y = y;
    pbtn->w = w;
    pbtn->h = h;
    pbtn->type = mod;
    pbtn->color = LIGHTGRAY;
    pbtn->text = NULL;
//    wcscpy_s(pbtn->text, L"\0");
}

// 绘制按钮
void drawButton(TButton* pbtn)
{
    int ox, oy;
    // 获取容器原点坐标，无容器则为窗口原点
    if (pbtn->container == NULL) {
        ox = 0;
        oy = 0;
    }
    else {
        ox = pbtn->container->x;
        oy = pbtn->container->y;
    }

    // 绘制按钮，包括椭圆形、圆角矩形、矩形三种
    setfillcolor(pbtn->color);
    RECT r = { ox + pbtn->x, oy + pbtn->y, ox + pbtn->x + pbtn->w, oy + pbtn->y + pbtn->h };
    if (pbtn->type==btCIRCLE) {
        fillellipse(r.left, r.top, r.right, r.bottom);
    }
    else if (pbtn->type == btRDRECT) {
        int esize = ((pbtn->w > pbtn->h) ? pbtn->h : pbtn->w) / 4;
        fillroundrect(r.left, r.top, r.right, r.bottom, esize, esize);
    }
    else {
        fillrectangle(r.left, r.top, r.right, r.bottom);
    }
    // 绘制按钮标题
    if (pbtn->text != NULL)
        drawtext(pbtn->text, &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE | DT_END_ELLIPSIS);
}

// 判断点pt是否在按钮中
int ptInButton(POINT p, TButton* pbtn)
{
    int bx, by;
    // 获取容器原点坐标，无容器则为窗口原点
    if (pbtn->container == NULL) {
        bx = pbtn->x;
        by = pbtn->y;
    }
    else {
        bx = pbtn->container->x + pbtn->x;
        by = pbtn->container->y + pbtn->y;
    }
    // 椭圆按钮，计算电是否在椭圆内
    if (pbtn->type == btCIRCLE) {
        double a = pbtn->w / 2;
        double b = pbtn->h / 2;
        double dx = p.x - (bx + a);
        double dy = p.y - (by + b);
        if ((dx * dx) / (a * a) + (dy * dy) / (b * b) <= 1.0)
            return 1;
    }
    else {
        if (p.x >= bx && p.x <= bx + pbtn->w && p.y >= by && p.y <= by + pbtn->h)
            return 1;
    }
    return 0;
}

void initPanel(TPanel* ppanel, int x, int y, int w, int h)
{
    ppanel->x = x;
    ppanel->y = y;
    ppanel->w = w;
    ppanel->h = h;
}

void initPanel(TPanel* ppanel, int size, int align)
{
    switch (align) {
    case alTOP:
        ppanel->x = 0;
        ppanel->y = 0;
        ppanel->w = getwidth();
        ppanel->h = size;
        break;
    case alBOTTOM:
        ppanel->x = 0;
        ppanel->y = getheight() - size;
        ppanel->w = getwidth();
        ppanel->h = size;
        break;
    case alLEFT:
        ppanel->x = 0;
        ppanel->y = 0;
        ppanel->w = size;
        ppanel->h = getheight();
        break;
    case alRIGHT:
        ppanel->x = getwidth()-size;
        ppanel->y = 0;
        ppanel->w = size;
        ppanel->h = getheight();
    }
}

void addButton(TPanel* ppanel, TButton* pbutton)
{
    if (ppanel->btnCount < MAX_BUTTON - 1) {
        ppanel->pbuttons[ppanel->btnCount] = pbutton;
        pbutton->container = ppanel;
        ppanel->btnCount++;
    }
}

void drawPanel(TPanel* ppanel)
{
    for (int i = 0; i < ppanel->btnCount; i++) {
        drawButton(ppanel->pbuttons[i]);
    }
}

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

COLORREF chooseColor(HWND hwnd)
{
    CHOOSECOLOR cc;                 // common dialog box structure 
    static COLORREF acrCustClr[16]; // array of custom colors 
//    HWND hwnd;                      // owner window
    HBRUSH hbrush;                  // brush handle
    static DWORD rgbCurrent;        // initial color selection

    // Initialize CHOOSECOLOR 
    ZeroMemory(&cc, sizeof(cc));
    cc.lStructSize = sizeof(cc);
    cc.hwndOwner = hwnd;
    cc.lpCustColors = (LPDWORD)acrCustClr;
    cc.rgbResult = rgbCurrent;
    cc.Flags = CC_FULLOPEN | CC_RGBINIT;

    if (ChooseColor(&cc) == TRUE)
    {
        hbrush = CreateSolidBrush(cc.rgbResult);
        rgbCurrent = cc.rgbResult;
    }
    return rgbCurrent;
}

// 定义一个处理图像的类
class Algorithm
{
public:
    Algorithm(IMAGE* img, int width, int heigth);
    void per_img(IMAGE read_img);
    void ver_img(IMAGE read_img);
    void draw_sence();
    void Gray();
    void Black_White();
    void Gauss();
    void getGaussianArray();
    void message_proce(IMAGE read_img);
    void bluebk();

    int Height; // 图片(画板)高度
    int Width; // 图片(画板)宽度
    int i; // 控制选择的按钮
    int bluex, bluey; // 记录蓝色选中状态的位置
    int size; // 记录画笔型号
    double** Gaussian_Ker; // 高斯滤盒
    IMAGE* pimg;
    COLORREF bluecolor;
    TButton button[30]; // 预置三十个按钮
};

// 初始化绘图场景
Algorithm::Algorithm(IMAGE* img, int width, int height)
{
    pimg = img;
    Width = width;
    Height = height;
    size = 1;
    for (int i = 0; i<6; i++)
        initButton(&button[i], 20+i*30, height + 15, 10, 0);

/*    button[6].x = 190; button[6].y = height + 5; button[6].x2 = 215; button[6].y2 = height + 25; button[6].text = TCHAR("笔"); button[6].type = 1;
    button[7].x = 220; button[7].y = height + 5; button[7].x2 = 290; button[7].y2 = height + 25; wcscpy_s(button[7].text,L"编辑颜色"); button[7].type = 1;
    button[8].x = 300; button[8].y = height + 5; button[8].x2 = 325; button[8].y2 = height + 25; wcscpy_s(button[8].text,L"□"); button[8].type = 1;
    button[9].x = 335; button[9].y = height + 5; button[9].x2 = 375; button[9].y2 = height + 25; wcscpy_s(button[9].text,L"椭圆"); button[9].type = 1;
    button[10].x = 385; button[10].y = height + 5; button[10].x2 = 455; button[10].y2 = height + 25; wcscpy_s(button[10].text,L"画笔型号"); button[10].type = 1;
    button[11].x = 470; button[11].y = height + 5; button[11].x2 = 530; button[11].y2 = height + 25; wcscpy_s(button[11].text,L"橡皮擦"); button[11].type = 1;
    button[12].x = 545; button[12].y = height + 5; button[12].x2 = 585; button[12].y2 = height + 25; wcscpy_s(button[12].text,L"保存"); button[12].type = 1;
    button[13].x = 595; button[13].y = height + 5; button[13].x2 = 635; button[13].y2 = height + 25; wcscpy_s(button[13].text,L"绘画"); button[13].type = 1;

    button[14].x = 20; button[14].y = height + 35; button[14].x2 = 90; button[14].y2 = height + 55; wcscpy_s(button[14].text,L"打开图片"); button[14].type = 1;
    button[15].x = 100; button[15].y = height + 35; button[15].x2 = 170; button[15].y2 = height + 55; wcscpy_s(button[15].text,L"水平镜像"); button[15].type = 1;
    button[16].x = 180; button[16].y = height + 35; button[16].x2 = 250; button[16].y2 = height + 55; wcscpy_s(button[16].text,L"垂直镜像"); button[16].type = 1;
    button[17].x = 260; button[17].y = height + 35; button[17].x2 = 300; button[17].y2 = height + 55; wcscpy_s(button[17].text,L"截图"); button[17].type = 1;
    button[18].x = 310; button[18].y = height + 35; button[18].x2 = 400; button[18].y2 = height + 55; wcscpy_s(button[18].text,L"黑白二值图"); button[18].type = 1;
    button[19].x = 410; button[19].y = height + 35; button[19].x2 = 480; button[19].y2 = height + 55; wcscpy_s(button[19].text,L"高斯模糊"); button[19].type = 1;
    button[20].x = 490; button[20].y = height + 35; button[20].x2 = 560; button[20].y2 = height + 55; wcscpy_s(button[20].text,L"灰度效果"); button[20].type = 1;
    button[21].x = 570; button[21].y = height + 35; button[21].x2 = 630; button[21].y2 = height + 55; wcscpy_s(button[21].text,L"马赛克"); button[21].type = 1;
 */   bluex = button[0].x; bluey = button[0].y;
    bluecolor = RGB(GetPrivateProfileInt(_T("COLOR0"), _T("R"), 0, _T("color.ini")), GetPrivateProfileInt(_T("COLOR0"), _T("G"), 0, _T("color.ini")), GetPrivateProfileInt(_T("COLOR0"), _T("B"), 0, _T("color.ini")));
}
// 选取状态
void Algorithm::bluebk()
{
    if (i < 6) // 控制当前颜色的选取状态
    {
        setfillcolor(RGB(255, 255, 255));
        solidrectangle(bluex - 12, bluey - 12, bluex + 13, bluey + 13);
        bluex = button[i].x;
        bluey = button[i].y;
        bluecolor = button[i].color;
        setfillcolor(RGB(187, 255, 255));
        solidrectangle(bluex - 12, bluey - 12, bluex + 13, bluey + 13);
    }
    else // 为当前选中按钮绘制淡蓝色背景作为选取状态
    {
        setfillcolor(RGB(187, 255, 255));
        solidrectangle(button[i].x + 1, button[i].y + 1, button[i].x2 - 1, button[i].y2 - 1);
        outtextxy(button[i].x + 5, button[i].y + 2, button[i].text);
    }
}
// 获取并执行消息
void Algorithm::message_proce(IMAGE read_img)
{
    int pen = 0;                            // 控制画笔
    int openr = 0;                            // 控制画矩形
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
    solidrectangle(bluex - 12, bluey - 12, bluex + 13, bluey + 13);
    solidrectangle(button[6].x + 1, button[6].y + 1, button[6].x2 - 1, button[6].y2 - 1);
    draw_sence(); // 绘制场景
    while (key == 0)
    {
        if (peekmessage(&m, EM_MOUSE | EM_KEY))
        {
            // 左键单击判断
            if (m.message == WM_LBUTTONDOWN) {
                // 控制画线
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
                // 控制画矩形
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
                // 控制画椭圆
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
                // 判断是否点击了按钮
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
                    bluebk();
                    draw_sence();
                    i = 0; // 防止当前选中的按钮序号丢失
                    setlinecolor(button[i].color); break;
                case 1:
                    bluebk();
                    draw_sence();
                    i = 1;
                    setlinecolor(button[i].color); break;
                case 2:
                    bluebk();
                    draw_sence();
                    i = 2;
                    setlinecolor(button[i].color); break;
                case 3:
                    bluebk();
                    draw_sence();
                    i = 3;
                    setlinecolor(button[i].color); break;
                case 4:
                    bluebk();
                    draw_sence();
                    i = 4;
                    setlinecolor(button[i].color); break;
                case 5:
                    bluebk();
                    draw_sence();
                    i = 5;
                    setlinecolor(button[i].color); break;
                case 6: // 笔
                    pen = 0;
                    openr = 1;
                    opene = 1;
                    setfillcolor(WHITE);
                    solidrectangle(button[8].x + 1, button[8].y + 1, button[8].x2 - 1, button[8].y2 - 1);
                    solidrectangle(button[9].x + 1, button[9].y + 1, button[9].x2 - 1, button[9].y2 - 1);
                    outtextxy(button[8].x + 5, button[8].y + 2, button[8].text);
                    outtextxy(button[9].x + 5, button[9].y + 2, button[9].text);
                    bluebk();
                    setlinecolor(bluecolor);
                    draw_sence();
                    break;
                case 7: // 编辑颜色
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
                    // 写配置文件，记录喜爱的颜色
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
                    draw_sence(); // 更改当前喜欢的颜色，并重新绘制场景
                    break;
                case 8: // 画矩形
                    bluebk();
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
                case 9: // 画椭圆
                    bluebk();
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
                case 10: // 画笔型号
                    InputBox(str, 5, _T("请输入画笔型号(1~5)"));
                    swscanf_s(str, _T("%d"), &i);
                    if (i >= 1 && i <= 5)
                    {
                        size = i;
                        setlinestyle(PS_SOLID, size);
                    }
                    else
                    {
                        MessageBox(hout, _T("输入错误，不在范围内"), _T("错误提示"), MB_OK);
                    }
                    break;
                case 11: // 橡皮擦
                    setfillcolor(WHITE);
                    solidrectangle(button[6].x + 1, button[6].y + 1, button[6].x2 - 1, button[6].y2 - 1);
                    solidrectangle(button[8].x + 1, button[8].y + 1, button[8].x2 - 1, button[8].y2 - 1);
                    solidrectangle(button[9].x + 1, button[9].y + 1, button[9].x2 - 1, button[9].y2 - 1);
                    outtextxy(button[6].x + 5, button[6].y + 2, button[6].text);
                    outtextxy(button[8].x + 5, button[8].y + 2, button[8].text);
                    outtextxy(button[9].x + 5, button[9].y + 2, button[9].text);
                    bluebk();
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
                case 12: // 保存
                    setfillcolor(WHITE);
                    solidrectangle(button[6].x + 1, button[6].y + 1, button[6].x2 - 1, button[6].y2 - 1);
                    solidrectangle(button[8].x + 1, button[8].y + 1, button[8].x2 - 1, button[8].y2 - 1);
                    solidrectangle(button[9].x + 1, button[9].y + 1, button[9].x2 - 1, button[9].y2 - 1);
                    outtextxy(button[6].x + 5, button[6].y + 2, button[6].text);
                    outtextxy(button[8].x + 5, button[8].y + 2, button[8].text);
                    outtextxy(button[9].x + 5, button[9].y + 2, button[9].text);
                    saveimage(_T("D:\\效果图.jpg"));
                    loadimage(pimg, _T("D:\\效果图.jpg"));
                    getimage(pimg, 0, 0, Width, Height);
                    saveimage(_T("D:\\效果图.jpg"), pimg);
                    setfillcolor(WHITE);
                    pen = 1;
                    opene = 1;
                    openr = 1;
                    break;
                case 13: // 绘图
                    key = 1;
                    con = 0;
                    pen = 1;
                    closegraph();
                    break;
                case 14: // 打开图片
                    if (FileDialog(szFile))
                    {
                        con = 1;
                        key = 1;
                    }
                    pen = 1;
                    break;
                case 15: // 水平镜像
                    per_img(read_img);
                    pen = 1;
                    openr = 1;
                    opene = 1;
                    break;
                case 16: // 垂直镜像
                    ver_img(read_img);
                    pen = 1;
                    openr = 1;
                    opene = 1;
                    break;
                case 17: // 截图
                    setfillcolor(WHITE);
                    solidrectangle(button[6].x + 1, button[6].y + 1, button[6].x2 - 1, button[6].y2 - 1);
                    solidrectangle(button[8].x + 1, button[8].y + 1, button[8].x2 - 1, button[8].y2 - 1);
                    solidrectangle(button[9].x + 1, button[9].y + 1, button[9].x2 - 1, button[9].y2 - 1);
                    outtextxy(button[6].x + 5, button[6].y + 2, button[6].text);
                    outtextxy(button[8].x + 5, button[8].y + 2, button[8].text);
                    outtextxy(button[9].x + 5, button[9].y + 2, button[9].text);
                    bluebk();
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
                                saveimage(_T("D:\\截图.jpg"), store);
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
                case 18: // 黑白二值图
                    setfillcolor(WHITE);
                    solidrectangle(button[6].x + 1, button[6].y + 1, button[6].x2 - 1, button[6].y2 - 1);
                    solidrectangle(button[8].x + 1, button[8].y + 1, button[8].x2 - 1, button[8].y2 - 1);
                    solidrectangle(button[9].x + 1, button[9].y + 1, button[9].x2 - 1, button[9].y2 - 1);
                    outtextxy(button[6].x + 5, button[6].y + 2, button[6].text);
                    outtextxy(button[8].x + 5, button[8].y + 2, button[8].text);
                    outtextxy(button[9].x + 5, button[9].y + 2, button[9].text);
                    Black_White();
                    pen = 1;
                    openr = 1;
                    opene = 1;
                    break;
                case 19: // 高斯模糊
                    setfillcolor(WHITE);
                    solidrectangle(button[6].x + 1, button[6].y + 1, button[6].x2 - 1, button[6].y2 - 1);
                    solidrectangle(button[8].x + 1, button[8].y + 1, button[8].x2 - 1, button[8].y2 - 1);
                    solidrectangle(button[9].x + 1, button[9].y + 1, button[9].x2 - 1, button[9].y2 - 1);
                    outtextxy(button[6].x + 5, button[6].y + 2, button[6].text);
                    outtextxy(button[8].x + 5, button[8].y + 2, button[8].text);
                    outtextxy(button[9].x + 5, button[9].y + 2, button[9].text);
                    getGaussianArray();
                    pen = 1;
                    openr = 1;
                    opene = 1;
                    break;
                case 20: // 灰度效果
                    setfillcolor(WHITE);
                    solidrectangle(button[6].x + 1, button[6].y + 1, button[6].x2 - 1, button[6].y2 - 1);
                    solidrectangle(button[8].x + 1, button[8].y + 1, button[8].x2 - 1, button[8].y2 - 1);
                    solidrectangle(button[9].x + 1, button[9].y + 1, button[9].x2 - 1, button[9].y2 - 1);
                    outtextxy(button[6].x + 5, button[6].y + 2, button[6].text);
                    outtextxy(button[8].x + 5, button[8].y + 2, button[8].text);
                    outtextxy(button[9].x + 5, button[9].y + 2, button[9].text);
                    Gray();
                    pen = 1;
                    openr = 1;
                    opene = 1;
                    break;
                case 21: // 马赛克
                    setfillcolor(WHITE);
                    solidrectangle(button[6].x + 1, button[6].y + 1, button[6].x2 - 1, button[6].y2 - 1);
                    solidrectangle(button[8].x + 1, button[8].y + 1, button[8].x2 - 1, button[8].y2 - 1);
                    solidrectangle(button[9].x + 1, button[9].y + 1, button[9].x2 - 1, button[9].y2 - 1);
                    outtextxy(button[6].x + 5, button[6].y + 2, button[6].text);
                    outtextxy(button[8].x + 5, button[8].y + 2, button[8].text);
                    outtextxy(button[9].x + 5, button[9].y + 2, button[9].text);
                    bluebk();
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
    // 从配置文件中读取颜色信息
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
    // 设置文字的背景模式
    setbkmode(TRANSPARENT);
    settextcolor(BLACK);
    setlinecolor(BLACK);
    setlinestyle(PS_SOLID, 1);
    if (Width <= 640) line(0, Height, 640, Height);
    else line(0, Height, Width, Height);
    // 绘制按钮
    for (i = 0; i < 6; i++)    {
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
    // 控制画笔型号以及当前颜色
    setlinestyle(PS_SOLID, size);
    setlinecolor(bluecolor);
}
// 水平镜像
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
// 垂直镜像
void Algorithm::ver_img(IMAGE read_img)
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
            pMem1[(Height - 1 - i) * Width + j] = pMem2[i * Width + j];
        }
    }
    putimage(0, 0, pimg);
}
// 灰度图像
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
        p[i] = RGB(c, c, c);        // 由于是灰度值，无需再执行 BGR 转换
    }
    putimage(0, 0, pimg);
}
// 黑白二值
void Algorithm::Black_White()
{
    int j;
    TCHAR str[10];
    if (con == 0)
    {
        getimage(pimg, 0, 0, 640, 480);
    }
    Gray();
    InputBox(str, 5, _T("请输入阀值(1~254)"));
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
        MessageBox(hout, _T("输入错误，不在范围内"), _T("错误提示"), MB_OK);
    }
}
// 高斯模糊
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
// 获取滤波器
void Algorithm::getGaussianArray()
{
    int j, k, d;
    double sigma;
    double sigma2;
    double a, sum = 0;
    TCHAR str[10];
    InputBox(str, 5, _T("请输入滤波窗口大小(3~15 奇数)"));
    swscanf_s(str, _T("%d"), &i);
    if (i >= 3 && i <= 15 && (i % 2 == 1))
    {
        Gaussian_Ker = (double**)calloc(i, sizeof(double*));
        for (j = 0; j < i; j++)Gaussian_Ker[j] = (double*)calloc(i, sizeof(double));
        InputBox(str, 5, _T("请输入滤波窗口大小(0.5~3.5)"));
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
            MessageBox(hout, _T("输入错误，不在范围内"), _T("错误提示"), MB_OK);
        }
    }
    else
    {
        MessageBox(hout, _T("输入错误，不在范围内"), _T("错误提示"), MB_OK);
    }
}

int main()
{
    int Height, Width;
    IMAGE image;
    IMAGE read_img;
    Height = 480;
    Width = 640;
    
    hout = initgraph(800, 800);
    setbkcolor(WHITE);
    cleardevice();

    TPanel panel;
    TButton buttons[22] = {
        {NULL, RED, btCIRCLE}
    };

 /*   button[6].x = 190; button[6].y = height + 5; button[6].x2 = 215; button[6].y2 = height + 25; wcscpy_s(button[6].text, L"笔"); button[6].type = 1;
    button[7].x = 220; button[7].y = height + 5; button[7].x2 = 290; button[7].y2 = height + 25; wcscpy_s(button[7].text, L"编辑颜色"); button[7].type = 1;
    button[8].x = 300; button[8].y = height + 5; button[8].x2 = 325; button[8].y2 = height + 25; wcscpy_s(button[8].text, L"□"); button[8].type = 1;
    button[9].x = 335; button[9].y = height + 5; button[9].x2 = 375; button[9].y2 = height + 25; wcscpy_s(button[9].text, L"椭圆"); button[9].type = 1;
    button[10].x = 385; button[10].y = height + 5; button[10].x2 = 455; button[10].y2 = height + 25; wcscpy_s(button[10].text, L"画笔型号"); button[10].type = 1;
    button[11].x = 470; button[11].y = height + 5; button[11].x2 = 530; button[11].y2 = height + 25; wcscpy_s(button[11].text, L"橡皮擦"); button[11].type = 1;
    button[12].x = 545; button[12].y = height + 5; button[12].x2 = 585; button[12].y2 = height + 25; wcscpy_s(button[12].text, L"保存"); button[12].type = 1;
    button[13].x = 595; button[13].y = height + 5; button[13].x2 = 635; button[13].y2 = height + 25; wcscpy_s(button[13].text, L"绘画"); button[13].type = 1;

    button[14].x = 20; button[14].y = height + 35; button[14].x2 = 90; button[14].y2 = height + 55; wcscpy_s(button[14].text, L"打开图片"); button[14].type = 1;
    button[15].x = 100; button[15].y = height + 35; button[15].x2 = 170; button[15].y2 = height + 55; wcscpy_s(button[15].text, L"水平镜像"); button[15].type = 1;
    button[16].x = 180; button[16].y = height + 35; button[16].x2 = 250; button[16].y2 = height + 55; wcscpy_s(button[16].text, L"垂直镜像"); button[16].type = 1;
    button[17].x = 260; button[17].y = height + 35; button[17].x2 = 300; button[17].y2 = height + 55; wcscpy_s(button[17].text, L"截图"); button[17].type = 1;
    button[18].x = 310; button[18].y = height + 35; button[18].x2 = 400; button[18].y2 = height + 55; wcscpy_s(button[18].text, L"黑白二值图"); button[18].type = 1;
    button[19].x = 410; button[19].y = height + 35; button[19].x2 = 480; button[19].y2 = height + 55; wcscpy_s(button[19].text, L"高斯模糊"); button[19].type = 1;
    button[20].x = 490; button[20].y = height + 35; button[20].x2 = 560; button[20].y2 = height + 55; wcscpy_s(button[20].text, L"灰度效果"); button[20].type = 1;
    button[21].x = 570; button[21].y = height + 35; button[21].x2 = 630; button[21].y2 = height + 55; wcscpy_s(button[21].text, L"马赛克"); button[21].type = 1;
*/
    initPanel(&panel, 60, alBOTTOM);
    for (int i = 0; i < 6; i++) {
        initButton(&buttons[i], 20 + i * 30, 15, 20, 20, btCIRCLE);
        addButton(&panel, &buttons[i]);
    }
    drawPanel(&panel);


 /*   getimage(&image, 0, 0, 640, 480);
    Algorithm myimg(&image, Width, Height);
    myimg.message_proce(image);


    if (con == 1)// 插图绘图板
    {
        image = new IMAGE();
        read_img = new IMAGE();
        loadimage(read_img, szFile);
        loadimage(image, szFile);
        Height = image.getheight();
        Width = image.getwidth();
        Algorithm mying(&image, Width, Height);
        if (Width < 640)
        {
            hout = initgraph(640, Height + 60);
            setbkcolor(WHITE);
            cleardevice();
        }
        if (Height >= 480 && Width >= 640)
        {
            hout = initgraph(Width, Height + 60);
            setbkcolor(WHITE);
            cleardevice();
        }
        putimage(0, 0, &image);
        key = 0;
        while (key == 0)
        {
            mying.message_proce(*read_img);
        }
    }*/
    _getch();
    return 0;
}

int main1()
{
    int Height, Width;
    IMAGE* m_img;
    IMAGE* read_img;
    while (true)
    {
        Height = 480;
        Width = 640;
        if (con == 0)// 空白绘图板
        {
            m_img = new IMAGE();
            read_img = new IMAGE();
            hout = initgraph(Width, Height + 60);

            setbkcolor(WHITE);
            cleardevice();
            getimage(m_img, 0, 0, 640, 480);
            getimage(read_img, 0, 0, 640, 480);
            Algorithm myimg(m_img, Width, Height);
            key = 0;
            while (key == 0)
            {
                myimg.message_proce(*m_img);
            }
            delete m_img;
            delete read_img;
            m_img = read_img = NULL;
        }
        if (con == 1)// 插图绘图板
        {
            m_img = new IMAGE();
            read_img = new IMAGE();
            loadimage(read_img, szFile);
            loadimage(m_img, szFile);
            Height = (*m_img).getheight();
            Width = (*m_img).getwidth();
            Algorithm mying(m_img, Width, Height);
            if (Width < 640)
            {
                hout = initgraph(640, Height + 60);
                setbkcolor(WHITE);
                cleardevice();
            }
            if (Height >= 480 && Width >= 640)
            {
                hout = initgraph(Width, Height + 60);
                setbkcolor(WHITE);
                cleardevice();
            }
            putimage(0, 0, m_img);
            key = 0;
            while (key == 0)
            {
                mying.message_proce(*read_img);
            }
            delete m_img;
            delete read_img;
            m_img = read_img = NULL;
        }
    }
    _getch();
    return 0;
}