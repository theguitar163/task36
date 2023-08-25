#pragma once

typedef struct tagTextView {
    RECT r;
    LOGFONT font;
    int linespace;
    TTextDoc* pdoc;
} TTextView;