#pragma once

#include "bbcode.h"

// 列表相关结构和函数
typedef struct tagContext {
    BBCodeType bbcodetype;
    COLORREF color;
    LOGFONT font;
} TContext, ITEM;

// 实体化s数组方式存储的item列表
typedef struct tagList {
    ITEM* array;
    int size;
    int maxsize;
} TList;

// 以指针数组方式存储的item列表
typedef struct tagListp {
    ITEM** array;
    int size;
    int maxsize;
} TListp;

// 保存ITEM本身，调用者无需关心ITEM的创建
void initList(TList* plist, int maxsize);
void freeList(TList* plist);
ITEM get(TList* plist, int idx);
void set(TList* plist, int idx, ITEM item);
void append(TList* plist, ITEM item);
void remove(TList* plist, int idx);
void clear(TList* plist);
void push(TList* plist, ITEM item);
int pop_stack(TList* plist, ITEM* item);
int pop_queue(TList* plist, ITEM* item);

// 仅保存指针，具体ITEM内存由调用者自行分配
void initList(TListp* plist, int maxsize);
void freeList(TListp* plist);
ITEM* get(TListp* plist, int idx);
void set(TListp* plist, int idx, ITEM* item);
void append(TListp* plist, ITEM* item);
void remove(TListp* plist, int idx);
void push(TListp* plist, ITEM* item);
int pop_stack(TListp* plist, ITEM** pitem);
int pop_queue(TListp* plist, ITEM** pitem);
void clear(TListp* plist);
