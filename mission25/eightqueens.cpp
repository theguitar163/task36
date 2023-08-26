#include <easyx.h>
#include "conio.h"
#include "eightqueens.h"

// �ʹ�ͼ�� -------------------------------------------

// ���ػʹ�ͼ�������(�ļ�)
void initCrownFile(TCrownImage* pcrown, const TCHAR* fimage, const TCHAR* fmask)
{
    loadimage(&pcrown->image, fimage); // Դͼ��
    loadimage(&pcrown->mask, fmask);   // ����ͼ��
}

// ���ػʹ�ͼ�������(��Դ�ļ�)
void initCrownRes(TCrownImage* pcrown, const TCHAR* rtype, const TCHAR* rimage, const TCHAR* rmask)
{
    loadimage(&pcrown->image, rtype, rimage); // Դͼ��
    loadimage(&pcrown->mask, rtype, rmask);   // ����ͼ��
}

// ���ƻʹ�͸��ͼ��
void drawCrown(TCrownImage* pcrown, int cx, int cy)
{
    int x = BORDER_WIDTH + cx * CELL_WIDTH;
    int y = BORDER_WIDTH + cy * CELL_WIDTH;
    putimage(x, y, &pcrown->mask, SRCAND);       // ����ͼ�뱳����
    putimage(x, y, &pcrown->image, SRCPAINT);    // Դͼ���뱳����
}

// ���̸� ---------------------------------------------

// ��ʼ����Ԫ��
// pcrown���ʹ�ͼ��ָ��
void initBoard(TBoard* pboard, TCrownImage* pcrown)
{
    // �״γ�ʼ��ʱ���贫��ʹ�ͼ�����ָ��
    if (pcrown != NULL)
        pboard->pcrown = pcrown;
    for (int i = 0; i < MAX_CELL; i++)
        pboard->rows[i] = -1;   // -1��ʾ����δ���ûʺ�
    pboard->solvecount = 0;
}

// ���������Լ������ϵĻʺ�
void drawBoard(TBoard* pboard)
{
    setbkcolor(WHITE);
    setbkmode(TRANSPARENT);  // ����ַ�ʱ����͸������
    cleardevice();

    // �������̱߿�ʵ����ȫͼ��䣩
    setfillcolor(0x212476);
    solidrectangle(0, 0, 8 * CELL_WIDTH + BORDER_WIDTH * 2, 8 * CELL_WIDTH + BORDER_WIDTH * 2);

    // �������̸���
    for (int i = 0; i < MAX_CELL; i++) {        // ��i�У�cy
        for (int j = 0; j < MAX_CELL; j++) {    // ��j�У�cx
            // ����������ǳ��ɫ
            if ((i + j) % 2 == 0)
                setfillcolor(0xafdaf5);
            else
                setfillcolor(0x366ecc);
            // ��䵥Ԫ��
            solidrectangle(BORDER_WIDTH + j * CELL_WIDTH,
                BORDER_WIDTH + i * CELL_WIDTH,
                BORDER_WIDTH + (j + 1) * CELL_WIDTH,
                BORDER_WIDTH + (i + 1) * CELL_WIDTH);
        }
        // ����ÿ�лʺ�λ�ã����ƻʹ�ͼ��
        if (pboard->rows[i] >= 0)
            drawCrown(pboard->pcrown, pboard->rows[i], i);  // ����ֵΪ��cx�������±�Ϊ��cy
    }

    RECT r;
    TCHAR str[100];
    // �ڶ����߿��ϻ��Ƶ�ǰ�� n �ֽⷨ
    if (pboard->solvecount > 0) {
        r = { 0, 0, BORDER_WIDTH * 2 + MAX_CELL * CELL_WIDTH, BORDER_WIDTH };
        settextcolor(WHITE);
        swprintf_s(str, L"�˻ʺ����⣺�� %d �ֽⷨ", pboard->solvecount);
        drawtext(str, &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
    }

    // �����͵ײ��߿��ϻ������̺�������
    settextcolor(WHITE);
    for (int i = 0; i < MAX_CELL; i++) {
        // ������������ֵ1-8
        swprintf_s(str, L"%d", i + 1);
        r = { 0, BORDER_WIDTH + i * CELL_WIDTH, BORDER_WIDTH, BORDER_WIDTH + (i + 1) * CELL_WIDTH };
        drawtext(str, &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
        // ���ƺ�������ֵA-H
        swprintf_s(str, L"%c", 'A' + i);
        r = { BORDER_WIDTH + i * CELL_WIDTH, BORDER_WIDTH + MAX_CELL * CELL_WIDTH,
            BORDER_WIDTH + (i + 1) * CELL_WIDTH, BORDER_WIDTH * 2 + MAX_CELL * CELL_WIDTH };
        drawtext(str, &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
    }
}

// ����λ���ܷ����ʺ�
// �˴������� [0 .. cy-1] ���Ƿ��г�ͻ
int checkQueenPos(TBoard* pboard, int cx, int cy)
{
    for (int i = 0; i < cy; i++) {      // ���0��cy-1��
        if (pboard->rows[i] == cx ||    // ͬһ��
            (pboard->rows[i] - i) == (cx - cy) ||   // ���б�ߣ�x-y���
            (pboard->rows[i] + i) == (cx + cy)) {   // �Ҳ�б�ߣ�x+y���
            return 0;
        }
    }
    return 1;
}

// ������ʱ�ȴ�
int wait()
{
    static int delay = 500;
    if (_kbhit()) {
        switch (_getch()) {
        case VK_ESCAPE:
            delay = 0;      // ESC��������ʱ
            break;
        case VK_SPACE:
            _getch();       // �ո������ͣ
            break;
        default:            // ������������һ��
            delay = 0.5 * delay;
        }
    }
    return delay;
}

/*
�����㷨��
����A�Ļʺ���ڵ�һ���Ժ��� B�Ļʺ���ڵ�һ���Ѿ�������ͻ����ʱ�򲻱ؼ���
���� C�Ļʺ󣬶��ǵ�����B�Ļʺ󵽵ڶ��У�������ͻ�ŵ����У�����ͻ�˲ſ�ʼ��
����C����˿����η�����A��E�Ļʺ���ͼ 2��ʾ����ÿ���ʺ����ұߺ���б��
���ĵ�λ�ò��ǣ�������F�Ļʺ��޴�������ʱ���ݵ��� E�Ļʺ󣬽���λ���ɵ�
4�е���Ϊ��8�У�������F�����ֻʺ���Ȼ�޴������ٴλ�����E����ʱ��E�Ѿ�ö��
������������������� D�������ɵ�2��������7�У��ٽ�����E�����������㷨������
���ҵ���ͼ 3��ʾ�Ľ⣬�ɹ��������������8������ƽ�������Ļʺ�
��������ȫ���Ľ⹲92����
�����㷨���˻ʺ������ԭ���ǣ��г�ͻ�����ͻ��û�г�ͻ��ǰ�ߣ���·��������
�ˣ��ߵ�����Ǵ𰸡�Ϊ�˼ӿ����޳�ͻ���ж��ٶȣ����Ը�ÿ�к����������ÿ����
�����Ƿ��лʺ�ռ�ݽ�����־���顣����һ���»ʺ�����־������ʱŲ��һ���ɻʺ���
����־��
*/
// ��cy����ӻʺ�
// ��ӳɹ��������������cy+1����ӻʺ�
void addQueen(TBoard* pboard, int cy)
{
    int delay = wait();

    // ����������[0..MAX_CELL-1]��������
    if (cy > MAX_CELL - 1) {
        pboard->solvecount++;
        drawBoard(pboard);
        FlushBatchDraw();
        Sleep(delay);
        return;
    }

    // ��cxѭ��
    for (int cx = 0; cx < MAX_CELL; cx++) {
        // �ڵ�ǰ��cy������ÿһ��cx���ҳ����ʵ�λ��
        if (checkQueenPos(pboard, cx, cy)) {
            // �ҵ��󣬸��¸���[cy]�����ֵΪ����cx
            pboard->rows[cy] = cx;
            // ͨ���ݹ���ã�������cy+1����ӻʺ�
            addQueen(pboard, cy + 1);
        }
    }
}