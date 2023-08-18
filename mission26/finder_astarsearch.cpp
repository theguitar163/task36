// 迷宫寻路算法：广度优先遍历
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
    // 是否成功标志
    int flag = 0;
    while (queue.size > 0) {
        pop_queue(&queue, &pcur);

        drawCell(pcur->x, pcur->y, YELLOW);
        FlushBatchDraw();
        // 是否是终点
        if (pcur->x == ex && pcur->y == ey) {
            pend = pcur;
            flag = 1;        //已到达终点标志
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
    // 成功则输出路径
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
	Point2 destPos;								// 目标位置
	std::vector<Node*> closeList;				// closeList 容器
	std::multiset<Node*, Compare> openList;		// openList 自动排序
	Node* p_destNode = nullptr;					// 用于返回终点的位置指针
public:

	~A_Container()
	{
		for (auto& no : openList) delete no;
		for (auto& no : closeList) delete no;
	}

	// 当前节点的邻居加入 openList
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

	// 从 openList 中取出 fVal 值最小的节点加入 closeList
	auto getMinNode()
	{
		auto it = openList.begin();
		Point2 ret((*it)->x, (*it)->y);
		closeList.push_back(*it);
		openList.erase(it);
		return ret;
	}

	// 获取寻路终点，用于回溯得到最短路径
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

// 计算欧式距离
int calcHx(ITEM it1, ITEM it2)
{
	return abs(it1.x - it2.x) + abs(it1.y - it2.y);
}

boolean canAddopenList(int x, int y) 
{
	// 不能越界
	if (x < 0 || x >= map.n || y < 0 || y >= map.n)
		return 0;
	// 不能等于墙
	if (map.map[y][x] == itWALL)
		return 0;
	if (isInList(&closeList, x, y))
		return 0;
	if (isInList(&openList, x, y))
		return 0;
	// 以上判断判断都不符合，可以加入
	return 1;
}

void addNode(ITEM curr, ITEM next)
{
	if (hasRoad(curr.x, curr.y)) {
		// 当前的距离加1
		int Gx = curr.Gx + 1;
		// 这是与目标的距离
		int Hx = calcHx(curr, end);
		// 判断是否在终点
		if (isEnd(next, end)) {
			next = end;
			next.pparent = &curr;
			next.Gx = Gx;
			next.Hx = calcHx(next, end);
		}
		// 如果不在，继续寻找
		else
			next = new Point(next, curr, Gx, Hx);
		// 加入放入可移动的路径
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

		addNode(curr, x - 1, y);	//左
		addNode(curr, x + 1, y);	//右
		addNode(curr, x, y - 1);	//上
		addNode(curr, x, y + 1);	//下
		if (isInList(&closeList, end.x, end.y)) {
			drawPath(map.map, map.end);
			break;
		}
	}
}

void A_find()
{
	TList openList;		// 可移动路径（按F值自动排序）
	TList closeList;    // 已走过路径

	// 从目标位置开始寻找起点，之后回溯即可得到起点到终点的路径
	ITEM start, end;
	ITEM pcur = end;
	// 创建高效容器
	ITEM destPos = start;
	// 将目标位置加入 openList
	push(&openList, pcur);
	// 后续迭代进行，根据 A* 寻路规则，利用高效容器寻找最短路径
	while (true) {
		drawCell(pcur.x, pcur.y, RGB(0, 255, 255));
		// 将 openList 中 F 值最小的节点放到 closeList 中
		pcur = cont.getMinNode();
		// 标记节点加入到 openList 中的节点
		Maze[pcur.x][pcur.y] = itVISITED;
		drawCell(pcur.x, pcur.y, RGB(0, 0, 255));
		// 后续 if 将周围通路加入 openList
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

	// 找到起点后，获取高效容器中的起点，回溯得到最短路径
	auto st = cont.getDestNode();
	while (st->parent != nullptr)
	{
		drawCell(st->x, st->y, RGB(255, 255, 0));
		st = st->parent;
		drawCell(st->x, st->y, RGB(255, 0, 0));
	}
}
