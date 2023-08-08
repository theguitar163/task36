#pragma once

void PaintLine(TPainter* ppainter, int startx, int starty);
void PaintRect(TPainter* ppainter, int startx, int starty);
void PaintEllipse(TPainter* ppainter, int startx, int starty);

void LoadImage(TPainter* ppainter);
// 水平镜像
void HorizontalMirrorImage(TPainter* ppainter);
// 垂直镜像
void VerticalMirrorImage(TPainter* ppainter);
// 灰度图像
void GrayImage(TPainter* ppainter);
// 黑白二值
void BlackWhiteImage(TPainter* ppainter);
// 高斯模糊
void GaussImage(TPainter* ppainter);

void SetPenLine(TPainter* ppainter);
void SetPenRect(TPainter* ppainter);
void SetPenEllipse(TPainter* ppainter);
