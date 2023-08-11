#include <easyx.h>
#include <graphics.h>
#include <math.h>
#include "painter.h"
#include "imageproc.h"

#define PI 3.141592653589793

void onSetPenColor(TPainter* ppainter)
{
    int btnIdx = ppainter->ppanel->btnClicked;
    updateButtonGroup(ppainter->ppanel, btnIdx);
    ppainter->penColor = ppainter->ppanel->pbuttons[btnIdx]->color;
}

void onSetFillColor(TPainter* ppainter)
{
    int btnIdx = ppainter->ppanel->btnClicked;
    updateButtonGroup(ppainter->ppanel, btnIdx);
    ppainter->fillColor = ppainter->ppanel->pbuttons[btnIdx]->color;
}

void onSetPenLine(TPainter* ppainter)
{
    int btnIdx = ppainter->ppanel->btnClicked;
    updateButtonGroup(ppainter->ppanel, btnIdx);
    ppainter->penType = ptLINE;
}

void onSetPenRect(TPainter* ppainter)
{
    int btnIdx = ppainter->ppanel->btnClicked;
    updateButtonGroup(ppainter->ppanel, btnIdx);
    ppainter->penType = ptRECT;
}

void onSetPenEllipse(TPainter* ppainter)
{
    int btnIdx = ppainter->ppanel->btnClicked;
    updateButtonGroup(ppainter->ppanel, btnIdx);
    ppainter->penType = ptELLIPSE;
}

void onSetPenEraser(TPainter* ppainter)
{
    int btnIdx = ppainter->ppanel->btnClicked;
    updateButtonGroup(ppainter->ppanel, btnIdx);
    ppainter->penType = ptERASER;
}

void onSetPenMosaic(TPainter* ppainter)
{
    int btnIdx = ppainter->ppanel->btnClicked;
    updateButtonGroup(ppainter->ppanel, btnIdx);
    ppainter->penType = ptMOSAIC;
}

void onSetPenSelect(TPainter* ppainter)
{
    int btnIdx = ppainter->ppanel->btnClicked;
    updateButtonGroup(ppainter->ppanel, btnIdx);
    ppainter->penType = ptSELECT;
}
// 从电脑中获取图片
void onLoadImage(TPainter* ppainter)
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

void onSaveImage(TPainter* ppainter)
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

void onSaveClip(TPainter* ppainter)
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

void onChoosePenColor(TPainter* ppainter)
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

    int btnIdx = ppainter->ppanel->btnClicked;
    updateButtonGroup(ppainter->ppanel, btnIdx);
    if (ChooseColor(&stChooseColor)) {
        ppainter->ppanel->pbuttons[btnIdx]->color = stChooseColor.rgbResult;
        ppainter->penColor = stChooseColor.rgbResult;
    }
}

void onChooseFillColor(TPainter* ppainter)
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

    int btnIdx = ppainter->ppanel->btnClicked;
    updateButtonGroup(ppainter->ppanel, btnIdx);
    if (ChooseColor(&stChooseColor)) {
        ppainter->ppanel->pbuttons[btnIdx]->color = stChooseColor.rgbResult;
        ppainter->fillColor = stChooseColor.rgbResult;
    }
}

void onSetFill(TPainter* ppainter)
{
    TPanel* pl = ppainter->ppanel;
    int isfill = pl->pbuttons[pl->btnClicked]->tag;
    ppainter->isFill = (isfill) ? 0 : 1;
    pl->pbuttons[pl->btnClicked]->tag = ppainter->isFill;
}

void onChoosePenThickness(TPainter* ppainter)
{
    TCHAR str[20];
    int num;
    InputBox(str, 5, L"请输入画笔宽度(1-5)");
    swscanf_s(str, L"%d", &num);
    if (num >= 1 && num <= 5) {
        TPanel* pl = ppainter->ppanel;
        pl->pbuttons[pl->btnClicked]->tag = num;
        ppainter->penThickness = num;
        setlinestyle(PS_SOLID, num);
    }
    else {
        MessageBox(ppainter->hwnd, L"输入错误，不在范围内", L"错误提示", MB_OK);
    }
}

void onClearCanvas(TPainter* ppainter)
{
    backupPainter(ppainter);
    clearPainter(ppainter);
}

void onUndoAction(TPainter* ppainter)
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
void onHorizontalMirrorImage(TPainter* ppainter)
{
    backupPainter(ppainter);
    RECT r;
    if (ppainter->selectState)
        r = ppainter->selectRect;
    else
        r = { ppainter->x, ppainter->y, ppainter->x + ppainter->w, ppainter->y + ppainter->h };
    HorizontalMirrorImage(r);
}

// 垂直镜像
void onVerticalMirrorImage(TPainter* ppainter)
{
    backupPainter(ppainter);
    RECT r;
    if (ppainter->selectState)
        r = ppainter->selectRect;
    else
        r = { ppainter->x, ppainter->y, ppainter->x + ppainter->w, ppainter->y + ppainter->h };
    VerticalMirrorImage(r);
}

// 灰度图像
void onGrayImage(TPainter* ppainter)
{
    backupPainter(ppainter);
    RECT r;
    if (ppainter->selectState)
        r = ppainter->selectRect;
    else
        r = { ppainter->x, ppainter->y, ppainter->x + ppainter->w, ppainter->y + ppainter->h };
    GrayImage(r);
}

// 黑白二值
void onBlackWhiteImage(TPainter* ppainter)
{
    backupPainter(ppainter);
    RECT r;
    if (ppainter->selectState)
        r = ppainter->selectRect;
    else
        r = { ppainter->x, ppainter->y, ppainter->x + ppainter->w, ppainter->y + ppainter->h };
    BlackWhiteImage(r);
}

// 高斯模糊
void onGaussImage(TPainter* ppainter)
{
    backupPainter(ppainter);
    RECT r;
    if (ppainter->selectState)
        r = ppainter->selectRect;
    else
        r = { ppainter->x, ppainter->y, ppainter->x + ppainter->w, ppainter->y + ppainter->h };
    GaussImage(r);
}

