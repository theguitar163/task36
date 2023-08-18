
#include <stdlib.h>
#include "maze.h"
#include "tree.h"

ITEM* createNode(ITEM item)
{
    ITEM* pnode = (ITEM*)malloc(sizeof(ITEM));
    *pnode = item;
    return pnode;
}

void addChild(ITEM* pparent, ITEM* pchild)
{

    for (int i = 0; i < MAX_CHILDREN; i++) {
        if (pparent->pchildren[i] == NULL) {
            pparent->pchildren[i] = pchild;
            pchild->pparent = pparent;
            break;
        }
    }
}

// �ݹ�������нڵ㣬���ͷ��ڴ�
void freeTree(ITEM* node)
{
    for (int i = 0; i < MAX_CHILDREN; i++) {
        if (node->pchildren[i] != NULL) {
            freeTree(node->pchildren[i]);
        }
    }
    free(node);
}

ITEM* getParent(ITEM* pnode)
{
    return pnode->pparent;
}