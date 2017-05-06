#include "DocBlock.h"

DocBlock::DocBlock(QWidget *parent)
    :QTextEdit(parent)
{
    this->boundary.set(0,0,0,0);        // 默认是一个空的包围矩形
}
