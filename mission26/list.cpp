#include <stdlib.h>
#include "list.h"

void initList(TList* plist, int maxsize)
{
    plist->array = (ITEM*)malloc(sizeof(ITEM) * maxsize);
    plist->size = 0;
    plist->maxsize = maxsize;
}

void freeList(TList* plist)
{
    free(plist->array);
    plist->array = NULL;
    plist->size = 0;
}

ITEM get(TList* plist, int idx)
{
    return plist->array[idx];
}

void set(TList* plist, int idx, ITEM it)
{
    plist->array[idx] = it;
}

void append(TList* plist, ITEM item)
{
    if (plist->size < plist->maxsize) {
        plist->array[plist->size] = item;
        plist->size++;
    }
}
void remove(TList* plist, int idx)
{
    if (plist->size > 0) {
        for (int i = idx; i < plist->size; i++)
            plist->array[i] = plist->array[i + 1];
        plist->size--;
    }
}

void push(TList* plist, ITEM item)
{
    append(plist, item);
}

int pop_stack(TList* plist, ITEM* item)
{
    if (plist->size > 0) {
        item->x = plist->array[plist->size - 1].x;
        item->y = plist->array[plist->size - 1].y;
        plist->size--;
        return 1;
    }
    else
        return 0;
}

int pop_queue(TList* plist, ITEM* item)
{
    if (plist->size > 0) {
        item->x = plist->array[0].x;
        item->y = plist->array[0].y;
        remove(plist, 0);
        return 1;
    }
    else
        return 0;
}

void clear(TList* plist)
{
    plist->size = 0;
}
