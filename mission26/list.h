#pragma once

// 列表相关结构和函数
typedef struct tagItem {
    int x;
    int y;
}ITEM;

typedef struct tagList {
    ITEM* array;
    int size;
    int maxsize;
} TList;

void initList(TList* plist, int maxsize);
void freeList(TList* plist);
ITEM get(TList* plist, int idx);
void set(TList* plist, int idx, ITEM it);
void append(TList* plist, ITEM item);
void remove(TList* plist, int idx);
void clear(TList* plist);
void push(TList* plist, ITEM it);
int pop(TList* plist, ITEM* it);
