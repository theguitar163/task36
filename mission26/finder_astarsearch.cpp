// �Թ�Ѱ·�㷨��������ȱ���
#include <stdio.h>
#include <conio.h>
#include "maze.h"
#include "list.h"
#include "tree.h"
#include "draw_maze.h"


void visit(TListp* plist, ITEM* pitem)
{
    Maze[pitem->x][pitem->y] = itVISITED;
    push(plist, pitem);
}

void finditROAD_astarsearch()
{
    int sx = 1, sy = 1, ex = MAX_COL - 2, ey = MAX_ROW - 2;
    TListp queue;
    initList(&queue, MAX_COL * MAX_ROW);

    ITEM* proot;
    ITEM* pend = NULL;
    ITEM* pchild;

    proot = createNode({ sx, sy });
    ITEM* pcur = proot;
    visit(&queue, pcur);
    // �Ƿ�ɹ���־
    int flag = 0;
    while (queue.size > 0) {
        pop_queue(&queue, &pcur);

        drawCell(pcur->x, pcur->y, YELLOW);
        FlushBatchDraw();
        // �Ƿ����յ�
        if (pcur->x == ex && pcur->y == ey) {
            pend = pcur;
            flag = 1;        //�ѵ����յ��־
            break;
        }

        if (hasRoad(pcur->x, pcur->y)) {
            if (Maze[pcur->x + 1][pcur->y] == itROAD) {
                pchild = createNode({ pcur->x + 1, pcur->y });
                addChild(pcur, pchild);
                visit(&queue, pchild);
            }
            if (Maze[pcur->x - 1][pcur->y] == itROAD) {
                pchild = createNode({ pcur->x - 1, pcur->y });
                addChild(pcur, pchild);
                visit(&queue, pchild);
            }
            if (Maze[pcur->x][pcur->y + 1] == itROAD) {
                pchild = createNode({ pcur->x, pcur->y + 1 });
                addChild(pcur, pchild);
                visit(&queue, pchild);
            }
            if (Maze[pcur->x][pcur->y - 1] == itROAD) {
                pchild = createNode({ pcur->x, pcur->y - 1 });
                addChild(pcur, pchild);
                visit(&queue, pchild);
            }
        }
    }
    // �ɹ������·��
    if (flag) {
        pcur = pend;
        while (true) {
            drawCell(pcur->x, pcur->y, RED);
            FlushBatchDraw();
            pcur = getParent(pcur);
            if (pcur == proot) {
                drawCell(pcur->x, pcur->y, RED);
                FlushBatchDraw();
                break;
            }
        }
    }
    freeList(&queue);
    freeTree(proot);
}

class A_Container
{
	struct Node
	{
		int x, y;
		int gVal, hVal, fVal;
		Node* parent;
		Node(int _x, int _y, int _g, int _h, Node* _p) :x(_x), y(_y), gVal(_g), hVal(_h), fVal(_g + _h), parent(_p) {}
	};
private:
	Point2 destPos;								// Ŀ��λ��
	std::vector<Node*> closeList;				// closeList ����
	std::multiset<Node*, Compare> openList;		// openList �Զ�����
	Node* p_destNode = nullptr;					// ���ڷ����յ��λ��ָ��
public:

	~A_Container()
	{
		for (auto& no : openList) delete no;
		for (auto& no : closeList) delete no;
	}

	// ��ǰ�ڵ���ھӼ��� openList
	void pushOpenList(ITEM _p2)
	{
		int gVal = 0;
		Node* par = nullptr;
		if (!closeList.empty()) {
			par = *(--closeList.end()); 
			gVal = par->gVal + 1; 
		}
		Node* temp = new Node(_p2.x, _p2.y, gVal, abs(_p2.x - destPos.x) + abs(_p2.y - destPos.y), par);
		if (_p2.x == destPos.x && _p2.y == destPos.y) 
			p_destNode = temp;
		openList.insert(temp);
		temp = nullptr;
	}

	// �� openList ��ȡ�� fVal ֵ��С�Ľڵ���� closeList
	auto getMinNode()
	{
		auto it = openList.begin();
		Point2 ret((*it)->x, (*it)->y);
		closeList.push_back(*it);
		openList.erase(it);
		return ret;
	}

	// ��ȡѰ·�յ㣬���ڻ��ݵõ����·��
	Node* getDestNode() 
	{ 
		return p_destNode; 
	}

};

ITEM findMinNode(TList* plist)
{
	int idx = 0;
	for (int i = 0; i < plist->size; i++) {
		if (plist->array[i].Gx + plist->array[i].Hx < plist->array[idx].Gx + plist->array[idx].Hx)
			idx = i;
	}
	return plist->array[idx];
}

void pushNode(TList* plist, ITEM item)
{
	int gVal = 0;
	Node* par = nullptr;
	if (closeList.size>0) {
		par = *(--closeList.end());
		gVal = par->gVal + 1;
	}
	Node* temp = createNode(item.x, item.y);
	gVal, abs(_p2.x - destPos.x) + abs(_p2.y - destPos.y), par);
	if (_p2.x == destPos.x && _p2.y == destPos.y)
		p_destNode = temp;
	openList.insert(temp);
	temp = nullptr;
}

// ����ŷʽ����
int calcHx(ITEM it1, ITEM it2)
{
	return abs(it1.x - it2.x) + abs(it1.y - it2.y);
}

boolean canAddopenList(int x, int y) 
{
	// ����Խ��
	if (x < 0 || x >= map.n || y < 0 || y >= map.n)
		return 0;
	// ���ܵ���ǽ
	if (map.map[y][x] == itWALL)
		return 0;
	if (isInList(&closeList, x, y))
		return 0;
	if (isInList(&openList, x, y))
		return 0;
	// �����ж��ж϶������ϣ����Լ���
	return 1;
}

void addNode(ITEM curr, ITEM next)
{
	if (hasRoad(curr.x, curr.y)) {
		// ��ǰ�ľ����1
		int Gx = curr.Gx + 1;
		// ������Ŀ��ľ���
		int Hx = calcHx(curr, end);
		// �ж��Ƿ����յ�
		if (isEnd(next, end)) {
			next = end;
			next.pparent = &curr;
			next.Gx = Gx;
			next.Hx = calcHx(next, end);
		}
		// ������ڣ�����Ѱ��
		else
			next = new Point(next, curr, Gx, Hx);
		// ���������ƶ���·��
		append(&openList, next);
	}
}

int isInList(TList* plist, int x, int y)
{
	for (int i = 0; i < plist->size; i++) {
		if (x == plist->array[i].x && y == plist->array[i].y)
			return 1;
	}
	return 0;
}

void Go() 
{
	TList openList;
	TList closeList;
	initList(&openList, MAX_COL * MAX_ROW);
	initList(&closeList, MAX_COL * MAX_ROW);

	openList.add(start);
	while (openList.size>0) {
		ITEM curr = findMinNode(&openList);
		remove(&openList, curr);
		append(&closeList, curr);
		int x = curr.x;
		int y = curr.y;

		addNode(curr, x - 1, y);	//��
		addNode(curr, x + 1, y);	//��
		addNode(curr, x, y - 1);	//��
		addNode(curr, x, y + 1);	//��
		if (isInList(&closeList, end.x, end.y)) {
			drawPath(map.map, map.end);
			break;
		}
	}
}

void A_find()
{
	TList openList;		// ���ƶ�·������Fֵ�Զ�����
	TList closeList;    // ���߹�·��

	// ��Ŀ��λ�ÿ�ʼѰ����㣬֮����ݼ��ɵõ���㵽�յ��·��
	ITEM start, end;
	ITEM pcur = end;
	// ������Ч����
	ITEM destPos = start;
	// ��Ŀ��λ�ü��� openList
	push(&openList, pcur);
	// �����������У����� A* Ѱ·�������ø�Ч����Ѱ�����·��
	while (true) {
		drawCell(pcur.x, pcur.y, RGB(0, 255, 255));
		// �� openList �� F ֵ��С�Ľڵ�ŵ� closeList ��
		pcur = cont.getMinNode();
		// ��ǽڵ���뵽 openList �еĽڵ�
		Maze[pcur.x][pcur.y] = itVISITED;
		drawCell(pcur.x, pcur.y, RGB(0, 0, 255));
		// ���� if ����Χͨ·���� openList
		if (Maze[pcur.x - 1][pcur.y] == itROAD) {
			Maze[pcur.x - 1][pcur.y] = itVISITED;
			push(&openList, Point2(pcur.x - 1, pcur.y));
			drawCell(pcur.x - 1, pcur.y, RGB(0, 255, 255));
			if (start == Point2(pcur.x - 1, pcur.y)) break;
		}
		if (Maze[pcur.x][pcur.y - 1] == itROAD) {
			Maze[pcur.x][pcur.y - 1] = itVISITED;
			push(&openList, Point2(pcur.x, pcur.y - 1));
			drawCell(pcur.x, pcur.y - 1, RGB(0, 255, 255));
			if (start == Point2(pcur.x, pcur.y - 1)) break;
		}
		if (Maze[pcur.x + 1][pcur.y] == itROAD) {
			Maze[pcur.x + 1][pcur.y] = itVISITED;
			push(&openList, Point2(pcur.x + 1, pcur.y));
			drawCell(pcur.x + 1, pcur.y, RGB(0, 255, 255));
			if (start == Point2(pcur.x + 1, pcur.y)) break;
		}
		if (Maze[pcur.x][pcur.y + 1] == itROAD) {
			Maze[pcur.x][pcur.y + 1] = itVISITED;
			push(&openList, Point2(pcur.x, pcur.y + 1));
			drawCell(pcur.x, pcur.y + 1, RGB(0, 255, 255));
			if (start == Point2(pcur.x, pcur.y + 1)) break;
		}
	}

	// �ҵ����󣬻�ȡ��Ч�����е���㣬���ݵõ����·��
	auto st = cont.getDestNode();
	while (st->parent != nullptr)
	{
		drawCell(st->x, st->y, RGB(255, 255, 0));
		st = st->parent;
		drawCell(st->x, st->y, RGB(255, 0, 0));
	}
}
