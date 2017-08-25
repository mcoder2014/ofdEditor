#include "DocTable.h"



DocTable::DocTable(QWidget *parent)
    :QTextEdit(parent)
{

}

DocTable::DocTable(int rows, int columns)
    :QTextEdit(0)
{

}

DocTable::~DocTable()
{

}

// 设置表格长和宽
DocTable::setTable(int rows, int columns)
{

}
