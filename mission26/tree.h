#pragma once

#include "maze.h"

ITEM* createNode(ITEM item);
void addChild(ITEM* pparent, ITEM* pchild);
// 递归遍历所有节点，并释放内存
void freeTree(ITEM* pnode);
ITEM* getParent(ITEM* pnode);