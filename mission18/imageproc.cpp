#include <easyx.h>
#include <math.h>

#define PI 3.141592653589793

// 从电脑中获取图片
 TCHAR* FileDialog()
{
    OPENFILENAME ofn;
    TCHAR szFile[MAX_PATH] = { 0 };	//用于接收文件名
    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn); // 结构大小
    ofn.lpstrFile = szFile;	//接收返回的文件名，注意第一个字符需要为NULL
    ofn.nMaxFile = MAX_PATH;       // 路径大小
    ofn.lpstrFilter = TEXT("图片文件(bmp; jpg; png; tiff; tif; jpeg; gif)\0*.bmp; *.jpg; *.png; *.tiff; *.jpeg; *.gif; *.tif\0\0"); // 文件类型
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST; // 标志
    if (GetOpenFileName(&ofn))
        return ofn.lpstrFile;
    else
        return NULL;
}

void LoadImage()
{
    IMAGE img;
    TCHAR* szFile = FileDialog();
    if (szFile != NULL) {
        loadimage(&img, L"D:\\SynologyDrive\\project\\三八节.png");
        putimage(0, 0, &img);
    }
}
// 水平镜像
void HorizontalMirrorImage()
{
    IMAGE img;
    getimage(&img, 0, 0, getwidth(), getheight() - 90);      // 取得当前窗口图像
    DWORD* psrc = GetImageBuffer(&img);  // 取得当前窗口图像显示缓存区数据
    DWORD* pdst = GetImageBuffer();      // 直接处理绘图窗口
    
    int w = img.getwidth();
    int h = img.getheight();
    for (int i = 0; i < h; i++) {
        for (int j = 0; j < w; j++) {
            pdst[i * getwidth() + j] = psrc[i * w + w - 1 - j];
        }
    }
}
// 垂直镜像
void VerticalMirrorImage()
{
    IMAGE img;
    getimage(&img, 0, 0, getwidth(), getheight() - 90);      // 取得当前窗口图像
    DWORD* psrc = GetImageBuffer(&img);  // 取得当前窗口图像显示缓存区数据
    DWORD* pdst = GetImageBuffer();      // 直接处理绘图窗口

    int w = img.getwidth();
    int h = img.getheight();
    for (int i = 0; i < h; i++) {
        for (int j = 0; j < w; j++) {
            pdst[(h - 1 - i) * getwidth() + j] = psrc[i * w + j];
        }
    }
}
// 灰度图像
void GrayImage()
{
    IMAGE img;
    getimage(&img, 0, 0, 640, 480);
    DWORD* p = GetImageBuffer(&img);
    COLORREF c;
    for (int i = img.getwidth() * img.getheight() - 1; i >= 0; i--) {
        c = BGR(p[i]);
        c = (GetRValue(c) * 299 + GetGValue(c) * 587 + GetBValue(c) * 114 + 500) / 1000;
        p[i] = RGB(c, c, c);        // 由于是灰度值，无需再执行 BGR 转换
    }
    putimage(0, 0, &img);
}

// 黑白二值
void BlackWhiteImage()
{
    int threshold = 127;
    IMAGE img;
    COLORREF c;

    getimage(&img, 0, 0, 640, 480);
    GrayImage();
    DWORD* p = GetImageBuffer(&img);
    for (int i = img.getwidth() * img.getheight() - 1; i >= 0; i--) {
        c = BGR(p[i]);
        if (GetRValue(c) < threshold)c = 0;
        else if (GetRValue(c) >= threshold)c = 255;
        p[i] = RGB(c, c, c);
    }
    putimage(0, 0, &img);
}

// 高斯模糊
void GaussImage()
{
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

    int w = getwidth();
    int h = getheight() - 90;
    R = (BYTE*)calloc(w * h, sizeof(BYTE));
    G = (BYTE*)calloc(w * h, sizeof(BYTE));
    B = (BYTE*)calloc(w * h, sizeof(BYTE));

    IMAGE img;
    getimage(&img, 0, 0, w, h);
    DWORD* pMem = GetImageBuffer(&img);
    for (j = 0; j < w * h; j++) {
        R[j] = GetRValue(pMem[j]);
        G[j] = GetGValue(pMem[j]);
        B[j] = GetBValue(pMem[j]);
    }
    for (j = filterSize / 2; j < h - filterSize / 2 - 1; j++) {
        for (k = filterSize / 2; k < w - filterSize / 2 - 1; k++) {
            for (n = 0; n < filterSize; n++) {
                for (m = 0; m < filterSize; m++) {
                    SR += R[(j - filterSize / 2 + n) * w + k - filterSize / 2 + m] * Gaussian_Ker[n][m];
                    SG += G[(j - filterSize / 2 + n) * w + k - filterSize / 2 + m] * Gaussian_Ker[n][m];
                    SB += B[(j - filterSize / 2 + n) * w + k - filterSize / 2 + m] * Gaussian_Ker[n][m];
                }
            }
            pMem[j * w + k] = RGB(SR, SG, SB);
            SR = SB = SG = 0;
        }
    }
    putimage(0, 0, &img);
    free(R);
    free(G);
    free(B);
    R = G = B = NULL;
}
