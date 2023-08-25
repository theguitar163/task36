// 任务：文本文件渲染器。
// 1.显示英文文本文件。
//   在磁盘上保存一个文本文件，内容为若干行的英文。用程序读取这个文档，并将其中的英文内容显示到屏幕上。
//   如果内容较长，无需考虑滚动条功能。
// 2.文本文件支持中文。
//   请自行了解 ANSI 编码、Unicode 编码（也叫 Utf - 16，分大小端）、Utf - 8 编码等常见编码格式，
//   使你的程序能正确的读取不同编码的中文文本文件，并正确在屏幕上显示中文。
// 3.富文本格式支持。
//   前面两步的文本文件并没有实现丰富的格式，这一步，要加上格式描述，使文档可以显示丰富的格式。
//   例如：文本文件中的 abc<b>def< / b>g，表示 def 为粗体，
//         <h1>xxx< / h1> 表示 1 号标题、
//         <i>xxx< / i> 表示斜体， 
//         << 表示 <， >> 表示 > 等。
//   你也可以 \bxxx / b 表示粗体，还可以用【asd】表示粗体。
//   格式描述自己定义，只要你的程序能按照你定义的格式正确显示即可。
//   注意：
//   1).格式定义要完整、没有歧义。例如，如果用 <b>xxx< / b> 表示粗体，那么，如何表示 < 这个符号呢？
//      当文档中含有 a < b 这样的内容，如何确保正确解析并显示？要在设计阶段就考虑清楚。
//   2).格式定义要允许嵌套。例如：<b>xxx<i>xxx< / i>xxx< / b> 表示粗体里面若干内容在粗体的基础上增加了斜体样式。
//   3).如果文档比较长，暂时可以不考虑滚动条的实现。
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <easyx.h>
#include "textdoc.h"
#include "textview.h"


int main()
{
    initgraph(800, 800);
    setbkcolor(WHITE);
    cleardevice();
    settextcolor(BLACK);

    TTextDoc doc;
    loadTextFile(&doc);

    TTextView view;
    initView(&view, &doc);

    paintView(&view);
    _getch();
    cleardevice();
    paintTokenView(&view);

    _getch();
    freeText(&doc);
    closegraph();
}


