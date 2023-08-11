#include <easyx.h>
#include <graphics.h>
#include <math.h>

#define PI 3.141592653589793

// 水平镜像
void HorizontalMirrorImage(RECT r)
{
    IMAGE imgsrc, imgdst;
    int w = r.right - r.left;
    int h = r.bottom - r.top;
    getimage(&imgsrc, r.left, r.top, w, h);    // 取得当前窗口图像
    getimage(&imgdst, r.left, r.top, w, h);    // 取得当前窗口图像
    DWORD* psrc = GetImageBuffer(&imgsrc);  // 取得当前窗口图像显示缓存区数据
    DWORD* pdst = GetImageBuffer(&imgdst);  // 直接处理绘图窗口
    
    for (int i = 0; i < h; i++) {
        for (int j = 0; j < w; j++) {
            pdst[i * w + j] = psrc[i * w + w - 1 - j];
        }
    }
    putimage(r.left, r.top, &imgdst);
}

// 垂直镜像
void VerticalMirrorImage(RECT r)
{
    IMAGE imgsrc, imgdst;
    int w = r.right - r.left;
    int h = r.bottom - r.top;
    getimage(&imgsrc, r.left, r.top, w, h);    // 取得当前窗口图像
    getimage(&imgdst, r.left, r.top, w, h);    // 取得当前窗口图像
    DWORD* psrc = GetImageBuffer(&imgsrc);     // 取得当前窗口图像显示缓存区数据
    DWORD* pdst = GetImageBuffer(&imgdst);     // 直接处理绘图窗口

    for (int i = 0; i < h; i++) {
        for (int j = 0; j < w; j++) {
            pdst[(h - 1 - i) * w + j] = psrc[i * w + j];
        }
    }
    putimage(r.left, r.top, &imgdst);
}

// 灰度图像
void GrayImage(RECT r)
{
    IMAGE img;
    int w = r.right - r.left;
    int h = r.bottom - r.top;
    getimage(&img, r.left, r.top, w, h);
    DWORD* p = GetImageBuffer(&img);
    COLORREF c;
    for (int i = w * h - 1; i >= 0; i--) {
        c = BGR(p[i]);
        c = (GetRValue(c) * 299 + GetGValue(c) * 587 + GetBValue(c) * 114 + 500) / 1000;
        p[i] = RGB(c, c, c);        // 由于是灰度值，无需再执行 BGR 转换
    }
    putimage(r.left, r.top, &img);
}

// 黑白二值
void BlackWhiteImage(RECT r)
{
    int threshold = 127;
    IMAGE img;
    COLORREF c;
    int w = r.right - r.left;
    int h = r.bottom - r.top;

    getimage(&img, r.left, r.top, w, h);
    GrayImage(r);
    DWORD* p = GetImageBuffer(&img);
    for (int i = w * h - 1; i >= 0; i--) {
        c = BGR(p[i]);
        if (GetRValue(c) < threshold)
            c = 0;
        else if (GetRValue(c) >= threshold)
            c = 255;
        p[i] = RGB(c, c, c);
    }
    putimage(r.left, r.top, &img);
}

// 高斯模糊
void GaussImage(RECT r)
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

    int w = r.right - r.left;
    int h = r.bottom - r.top;
    BYTE* R = (BYTE*)calloc(w * h, sizeof(BYTE));
    BYTE* G = (BYTE*)calloc(w * h, sizeof(BYTE));
    BYTE* B = (BYTE*)calloc(w * h, sizeof(BYTE));
    double SR = 0, SG = 0, SB = 0;

    IMAGE img;
    getimage(&img, r.left, r.top, w, h);
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
    putimage(r.left, r.top, &img);
    free(R);
    free(G);
    free(B);
    R = G = B = NULL;
}

