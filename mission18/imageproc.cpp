#include <easyx.h>
#include <graphics.h>
#include <math.h>
#include "painter.h"

#define PI 3.141592653589793

void SetPenColor(TPainter* ppainter)
{
    int btnIdx = ppainter->ppanel->btnFocused;
    ppainter->penColor = ppainter->ppanel->pbuttons[btnIdx]->color;
}

void SetFillColor(TPainter* ppainter)
{
    int btnIdx = ppainter->ppanel->btnFocused;
    ppainter->fillColor = ppainter->ppanel->pbuttons[btnIdx]->color;
}

void SetPenLine(TPainter* ppainter)
{
    ppainter->penType = ptLINE;
}

void SetPenRect(TPainter* ppainter)
{
    ppainter->penType = ptRECT;
}

void SetPenEllipse(TPainter* ppainter)
{
    ppainter->penType = ptELLIPSE;
}

void SetPenEraser(TPainter* ppainter)
{
    ppainter->penType = ptERASER;
}

void SetPenMosaic(TPainter* ppainter)
{
    ppainter->penType = ptMOSAIC;
}

// 从电脑中获取图片
void LoadImage(TPainter* ppainter)
{
    backupPainter(ppainter);
    OPENFILENAME ofn;
    TCHAR szFile[MAX_PATH] = { 0 };	//用于接收文件名
    ZeroMemory(&ofn, sizeof(ofn));
    ofn.hwndOwner = ppainter->hwnd;
    ofn.lStructSize = sizeof(ofn); // 结构大小
    ofn.lpstrFile = szFile;	       //接收返回的文件名，注意第一个字符需要为NULL
    ofn.nMaxFile = MAX_PATH;       // 路径大小
    ofn.lpstrFilter = TEXT("图片文件(bmp; jpg; png; tiff; tif; jpeg; gif)\0*.bmp; *.jpg; *.png; *.tiff; *.jpeg; *.gif; *.tif\0\0"); // 文件类型
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST; // 标志
    if (GetOpenFileName(&ofn)) {
        IMAGE img;
        loadimage(&img, ofn.lpstrFile);
        double ratio, ratiox = 1, ratioy = 1;
        if (img.getwidth() > ppainter->w)
            ratiox = (double)ppainter->w / img.getwidth();
        if (img.getheight() > ppainter->h)
            ratioy = (double)ppainter->h / img.getheight();
        ratio = (ratiox < ratioy) ? ratiox : ratioy;
        if (ratio < 1)
            loadimage(&img, ofn.lpstrFile, ratio * img.getwidth(), ratio * img.getheight());
        putimage(0, 0, ppainter->w, ppainter->h, &img, 0, 0);
    }
}

void SaveImage(TPainter* ppainter)
{
    OPENFILENAME ofn;
    TCHAR szFile[MAX_PATH] = { 0 };	//用于接收文件名
    ZeroMemory(&ofn, sizeof(ofn));
    ofn.hwndOwner = ppainter->hwnd;
    ofn.lStructSize = sizeof(ofn); // 结构大小
    ofn.lpstrFile = szFile;	       //接收返回的文件名，注意第一个字符需要为NULL
    ofn.lpstrDefExt = L"jpg";
    ofn.nMaxFile = MAX_PATH;       // 路径大小
    ofn.lpstrFilter = TEXT("JPG文件\0*.jpg\0BMP文件\0*.bmp\0PNG文件\0*.png\0\0"); // 文件类型
    ofn.nFilterIndex = 1;
    ofn.Flags = OFN_HIDEREADONLY | OFN_PATHMUSTEXIST | OFN_OVERWRITEPROMPT | OFN_EXPLORER; // 标志
    if (GetSaveFileName(&ofn)) {
        IMAGE img;
        getimage(&img, ppainter->x, ppainter->y, ppainter->w, ppainter->h);
        saveimage(ofn.lpstrFile, &img);
    }
}

void SaveClip(TPainter* ppainter)
{
 /*   IMAGE store;
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
    }*/
}

void ChoosePenColor(TPainter* ppainter)
{
    CHOOSECOLOR stChooseColor;            // 声明一个颜色选取的结构体变量
    COLORREF rgbLineColor = NULL;         // 编辑的存储选择的颜色
    COLORREF dwCustColors[16];            // 为结构体变量赋初值
    stChooseColor.lStructSize = sizeof(CHOOSECOLOR);
    stChooseColor.hwndOwner = ppainter->hwnd;
    stChooseColor.rgbResult = rgbLineColor;
    stChooseColor.lpCustColors = (LPDWORD)dwCustColors;
    stChooseColor.Flags = CC_RGBINIT;
    stChooseColor.lCustData = 0;
    stChooseColor.lpfnHook = NULL;
    stChooseColor.lpTemplateName = NULL;
    if (ChooseColor(&stChooseColor)) {
        int btnIdx = ppainter->ppanel->btnFocused;
        ppainter->ppanel->pbuttons[btnIdx]->color = stChooseColor.rgbResult;
        ppainter->penColor = stChooseColor.rgbResult;
    }
}

void ChooseFillColor(TPainter* ppainter)
{
    CHOOSECOLOR stChooseColor;            // 声明一个颜色选取的结构体变量
    COLORREF rgbLineColor = NULL;         // 编辑的存储选择的颜色
    COLORREF dwCustColors[16];            // 为结构体变量赋初值
    stChooseColor.lStructSize = sizeof(CHOOSECOLOR);
    stChooseColor.hwndOwner = ppainter->hwnd;
    stChooseColor.rgbResult = rgbLineColor;
    stChooseColor.lpCustColors = (LPDWORD)dwCustColors;
    stChooseColor.Flags = CC_RGBINIT;
    stChooseColor.lCustData = 0;
    stChooseColor.lpfnHook = NULL;
    stChooseColor.lpTemplateName = NULL;
    if (ChooseColor(&stChooseColor)) {
        int btnIdx = ppainter->ppanel->btnFocused;
        ppainter->ppanel->pbuttons[btnIdx]->color = stChooseColor.rgbResult;
        ppainter->fillColor = stChooseColor.rgbResult;
    }
}

void SetFill(TPainter* ppainter)
{
    TPanel* pl = ppainter->ppanel;
    int isfill = pl->pbuttons[pl->btnFocused]->value;
    ppainter->isFill = (isfill) ? 0 : 1;
    pl->pbuttons[pl->btnFocused]->value = ppainter->isFill;
}

void ChoosePenThickness(TPainter* ppainter)
{
    TCHAR str[20];
    int num;
    InputBox(str, 5, L"请输入画笔宽度(1-5)");
    swscanf_s(str, L"%d", &num);
    if (num >= 1 && num <= 5) {
        ppainter->penThickness = num;
        setlinestyle(PS_SOLID, num);
    }
    else {
        MessageBox(ppainter->hwnd, L"输入错误，不在范围内", L"错误提示", MB_OK);
    }
}

void ClearCanvas(TPainter* ppainter)
{
    backupPainter(ppainter);
    clearPainter(ppainter);
}

void UndoAction(TPainter* ppainter)
{
    IMAGE tmp;
    // 临时存放当前图像至tmp
    getimage(&tmp, ppainter->x, ppainter->y, ppainter->w, ppainter->h);
    // 在画布上绘制备份的图像imgbackup
    putimage(ppainter->x, ppainter->y, &ppainter->imgBackup);
    // 将临时图像tmp绘制到imgBackup中
    SetWorkingImage(&ppainter->imgBackup);
    putimage(ppainter->x, ppainter->y, &tmp);
    // 将当前绘制对象恢复
    SetWorkingImage(NULL);
}
// 水平镜像
void HorizontalMirrorImage(TPainter* ppainter)
{
    backupPainter(ppainter);
    IMAGE img;
    getimage(&img, ppainter->x, ppainter->y, ppainter->w, ppainter->h);      // 取得当前窗口图像
    DWORD* psrc = GetImageBuffer(&img);  // 取得当前窗口图像显示缓存区数据
    DWORD* pdst = GetImageBuffer();      // 直接处理绘图窗口
    
    for (int i = 0; i < ppainter->h; i++) {
        for (int j = 0; j < ppainter->w; j++) {
            pdst[i * ppainter->w + j] = psrc[i * ppainter->w + ppainter->w - 1 - j];
        }
    }
}
// 垂直镜像
void VerticalMirrorImage(TPainter* ppainter)
{
    backupPainter(ppainter);
    IMAGE img;
    getimage(&img, ppainter->x, ppainter->y, ppainter->w, ppainter->h);      // 取得当前窗口图像
    DWORD* psrc = GetImageBuffer(&img);  // 取得当前窗口图像显示缓存区数据
    DWORD* pdst = GetImageBuffer();      // 直接处理绘图窗口

    for (int i = 0; i < ppainter->h; i++) {
        for (int j = 0; j < ppainter->w; j++) {
            pdst[(ppainter->h - 1 - i) * ppainter->w + j] = psrc[i * ppainter->w + j];
        }
    }
}
// 灰度图像
void GrayImage(TPainter* ppainter)
{
    backupPainter(ppainter);
    IMAGE img;
    getimage(&img, ppainter->x, ppainter->y, ppainter->w, ppainter->h);
    DWORD* p = GetImageBuffer(&img);
    COLORREF c;
    for (int i = ppainter->w * ppainter->h - 1; i >= 0; i--) {
        c = BGR(p[i]);
        c = (GetRValue(c) * 299 + GetGValue(c) * 587 + GetBValue(c) * 114 + 500) / 1000;
        p[i] = RGB(c, c, c);        // 由于是灰度值，无需再执行 BGR 转换
    }
    putimage(0, 0, &img);
}

// 黑白二值
void BlackWhiteImage(TPainter* ppainter)
{
    backupPainter(ppainter);
    int threshold = 127;
    IMAGE img;
    COLORREF c;

    getimage(&img, ppainter->x, ppainter->y, ppainter->w, ppainter->h);
    GrayImage(ppainter);
    DWORD* p = GetImageBuffer(&img);
    for (int i = ppainter->w * ppainter->h - 1; i >= 0; i--) {
        c = BGR(p[i]);
        if (GetRValue(c) < threshold)c = 0;
        else if (GetRValue(c) >= threshold)c = 255;
        p[i] = RGB(c, c, c);
    }
    putimage(0, 0, &img);
}

// 高斯模糊
void GaussImage(TPainter* ppainter)
{
    backupPainter(ppainter);
    int filterSize = 7;
    double sigma = 2.0;

    int j, k, d, n, m;
    double sigma2;
    double a, sum = 0;

    double** Gaussian_Ker = (double**)calloc(filterSize, sizeof(double*));
    for (j = 0; j < filterSize; j++)
        Gaussian_Ker[j] = (double*)calloc(filterSize, sizeof(double));
    sigma2 = sigma * sigma;
    a = 1.0 / (2 * PI * sigma2);
    for (j = 0; j < filterSize; j++) {
        for (k = 0; k < filterSize; k++) {
            d = (j - filterSize / 2) * (j - filterSize / 2) + (k - filterSize / 2) * (k - filterSize / 2);
            Gaussian_Ker[j][k] = a * exp(-d / (2.0 * sigma2));
            sum += Gaussian_Ker[j][k];
        }
    }
    for (j = 0; j < filterSize; j++) {
        for (k = 0; k < filterSize; k++) {
            Gaussian_Ker[j][k] = Gaussian_Ker[j][k] / sum;
        }
    }

    BYTE* R;
    BYTE* G;
    BYTE* B;
    double SR = 0, SG = 0, SB = 0;

    R = (BYTE*)calloc(ppainter->w * ppainter->h, sizeof(BYTE));
    G = (BYTE*)calloc(ppainter->w * ppainter->h, sizeof(BYTE));
    B = (BYTE*)calloc(ppainter->w * ppainter->h, sizeof(BYTE));

    IMAGE img;
    getimage(&img, ppainter->x, ppainter->y, ppainter->w, ppainter->h);
    DWORD* pMem = GetImageBuffer(&img);
    for (j = 0; j < ppainter->w * ppainter->h; j++) {
        R[j] = GetRValue(pMem[j]);
        G[j] = GetGValue(pMem[j]);
        B[j] = GetBValue(pMem[j]);
    }
    for (j = filterSize / 2; j < ppainter->h - filterSize / 2 - 1; j++) {
        for (k = filterSize / 2; k < ppainter->w - filterSize / 2 - 1; k++) {
            for (n = 0; n < filterSize; n++) {
                for (m = 0; m < filterSize; m++) {
                    SR += R[(j - filterSize / 2 + n) * ppainter->w + k - filterSize / 2 + m] * Gaussian_Ker[n][m];
                    SG += G[(j - filterSize / 2 + n) * ppainter->w + k - filterSize / 2 + m] * Gaussian_Ker[n][m];
                    SB += B[(j - filterSize / 2 + n) * ppainter->w + k - filterSize / 2 + m] * Gaussian_Ker[n][m];
                }
            }
            pMem[j * ppainter->w + k] = RGB(SR, SG, SB);
            SR = SB = SG = 0;
        }
    }
    putimage(0, 0, &img);
    free(R);
    free(G);
    free(B);
    R = G = B = NULL;
}

