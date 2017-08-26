#include "DocTable.h"
#include <QTextCursor>


DocTable::DocTable(QWidget *parent)
    :QTextEdit(parent)
{
    QTextCursor cursor = this->textCursor();        // 获得光标
    this->_table = cursor.insertTable(3,4);         // 插入一个表格


}

DocTable::DocTable(int rows, int columns)
    :QTextEdit(0)
{

}

DocTable::~DocTable()
{

}

///
/// \brief DocTable::setTable
///         设置表格行数列数
/// \param rows
/// \param columns
///
void DocTable::setTable(int rows, int columns)
{

}

///
/// \brief DocTable::setDefaultStyle
///     将表格设置为默认的样式
///
void DocTable::setDefaultStyle()
{
    QTextDocument* doc = this->_table->document();      // 文档
    QTextFrame * rootFrame = doc->rootFrame();          // 根 框架

    QTextFrame::iterator iter;

}
