#pragma once

#include "maze.h"

ITEM* createNode(ITEM item);
void addChild(ITEM* pparent, ITEM* pchild);
// �ݹ�������нڵ㣬���ͷ��ڴ�
void freeTree(ITEM* pnode);
ITEM* getParent(ITEM* pnode);