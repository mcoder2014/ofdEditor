#include "DocTable.h"
#include <QTextCursor>
#include "Doc/DocBlock.h"
#include "Doc/DocLayer.h"
#include "Doc/DocPage.h"
#include "Doc/DocPassage.h"
#include <QMenu>
#include <QVector>
#include <QDebug>
#include <QApplication>
#include <QClipboard>
#include "Widget/TableSettingDialog.h"

DocTable::DocTable(QWidget *parent)
    :DocTextBlock(1.0)
{
    QTextCursor cursor = this->textCursor();        // 获得光标
    this->_table = cursor.insertTable(3,4);         // 插入一个表格

    this->init();


}

///
/// \brief DocTable::DocTable
///    固定的行列数初始化表格
/// \param rows
/// \param columns
///
DocTable::DocTable(int rows, int columns)
    :DocTextBlock(1.0)
{
    QTextCursor cursor = this->textCursor();        // 获得光标
    this->_table = cursor.insertTable(rows, columns);         // 插入一个表格

    this->init();
}


DocTable::~DocTable()
{

}

///
/// \brief DocTable::getType
/// \return
///
QString DocTable::getType()
{
    return tr("DocTable");
}

///
/// \brief DocTable::getMenu
///     关于表格操作的菜单
/// \return
///
QMenu *DocTable::getMenu()
{
    this->ContextMenu->clear();     //清空菜单
    this->ContextMenu->setTitle(this->getType());

    this->ContextMenu->addAction(this->actionCut);
    this->ContextMenu->addAction(this->actionCopy);
    this->ContextMenu->addAction(this->actionPaste);
    this->ContextMenu->addAction(this->actionSelectAll);

    this->ContextMenu->addSeparator();                      // 增加分界线
    this->ContextMenu->addAction(this->actionBold);         // 加粗
    this->ContextMenu->addAction(this->actionItalic);       // 斜体
    this->ContextMenu->addAction(this->actionColor);        // 颜色

    this->ContextMenu->addSeparator();                      // 分界线
    this->ContextMenu->addAction(this->actionFontSetTest);  // 字体
    this->ContextMenu->addAction(this->actionParagraph);    // 段落设置

    // 检验粘贴键的状态
    QClipboard *board = QApplication::clipboard();
    if(board->text().length() <= 0)
    {
        this->actionPaste->setEnabled(false);
    }
    else
    {
        this->actionPaste->setEnabled(true);
    }

    // 选择表格的状态
    QTextCursor cursor = this->textCursor();            // 文本光标
    this->selectedCells(cursor);

//    qDebug() << "firstRow, numRows, firstColumn, numColumns"
//             << this->firstRow
//             << this->rows
//             << this->firstColumn
//             << this->columns;

    this->ContextMenu->addSeparator();      // ------------------
    this->ContextMenu->addAction(this->actionTableSetting); // 表格设置
    this->ContextMenu->addMenu(this->menu_insertRowOrCol);  // 插入行或列

    if(this->rows > 1 || this->columns > 1)
    {
        this->ContextMenu->addAction(this->actionMergeCells);   // 合并单元格
        this->ContextMenu->addAction(this->actionSplitCells);   // 拆分单元格
    }

    this->ContextMenu->addAction(this->actionDeleteRow);    // 删除该行
    this->ContextMenu->addAction(this->actionDeleteColumn); // 删除该列

    return this->ContextMenu;
}

///
/// \brief DocTable::getTableColor
///     获得表格线条的颜色
/// \return
///
QColor DocTable::getTableColor()
{
    QTextTableFormat format = this->_table->format();
    QBrush brush = format.borderBrush();
    return brush.color();
}

QVector<QTextLength> DocTable::getColumnWidth()
{
    QTextTableFormat format = this->_table->format();
    QVector<QTextLength> vector = format.columnWidthConstraints();
    return vector;
}

///
/// \brief DocTable::setTable
///         设置表格行数列数
/// \param rows
/// \param columns
///
void DocTable::setTable(int rows, int columns)
{
    this->_table->resize(rows,columns);
}

///
/// \brief DocTable::setDefaultStyle
///     将表格设置为默认的样式
///
void DocTable::setDefaultStyle()
{
    int length = this->_table->columns();
    QVector<QTextLength> colWidth;

    for(int i = 0; i < length; i++)
    {
        QTextLength l(QTextLength::PercentageLength, 1.0 / length * 100);
        colWidth.append(l);
    }
    QTextTableFormat format = this->_table->format();
    format.setColumnWidthConstraints(colWidth);

    // 设置为单线
    QBrush blackBrush(Qt::SolidPattern);
    format.setBorderBrush(blackBrush);
    format.setBorder(1);
    format.setBorderStyle(QTextFrameFormat::BorderStyle_Solid);
    format.setCellSpacing(0);
//    format.setCellPadding(0);
//    format.setCellPadding(5);

    this->_table->setFormat(format);
}

void DocTable::setDefaultStyle(bool flag)
{
    this->isDefaultStyle = flag;
    if(flag == true)
        updateStyle();
}

///
/// \brief DocTable::setColumnWidth
///     设置每列的宽度
/// \param vector
///
void DocTable::setColumnWidth(QVector<QTextLength> &vector)
{
    this->colWidth.clear();
    this->colWidth = vector;
    this->updateStyle();
}

///
/// \brief DocTable::setBlock
/// \param block
///
void DocTable::setBlock(DocBlock *block)
{
    this->block = block;
    this->connect(block, SIGNAL(signal_resize(qreal,qreal,qreal,qreal)),
                  this, SLOT(blockSizeChanged()));
    //    qDebug() << "set block success table";
}

///
/// \brief DocTable::setCellPadding
///     设置单元格留白
/// \param cellpadding
///
void DocTable::setCellPadding(double cellpadding)
{
    QTextTableFormat format = this->_table->format();
    format.setCellPadding(cellpadding);
    this->_table->setFormat(format);
}

void DocTable::setTableColor(QColor color)
{
    QTextTableFormat format = this->_table->format();
    format.setBorderBrush(color);
    this->_table->setFormat(format);
}

///
/// \brief DocTable::setTableWidth
///     设置表格总体宽度
/// \param width
///
void DocTable::setTableWidth(double width)
{
    this->block->resize(width,this->block->size().rheight());
}

///
/// \brief DocTable::insertRowBefore
///     在之前插入行
///
void DocTable::insertRowBefore()
{
    QTextCursor cursor = this->textCursor();
    this->insertRow(cursor, -1);
}

///
/// \brief DocTable::insertRow
///     插入行
/// \param cursor
/// \param flag
///
void DocTable::insertRow(QTextCursor cursor, int flag)
{
    this->selectedCells(cursor);

    // 插入行
    if(flag == -1)
    {
        // 在选择区域上方插入一行
        this->_table->insertRows(this->firstRow,1);
    }
    else if(flag == 1)
    {
        this->_table->insertRows(this->firstRow + 1, 1);
    }

}

///
/// \brief DocTable::insertRowAfter
///
void DocTable::insertRowAfter()
{
    QTextCursor cursor = this->textCursor();
    this->insertRow(cursor, 1);
}

void DocTable::insertColBefore()
{
    QTextCursor cursor = this->textCursor();
    this->insertCol(cursor, -1);
}

void DocTable::insertColAfter()
{
    QTextCursor cursor = this->textCursor();
    this->insertCol(cursor, 1);
}

void DocTable::insertCol(QTextCursor cursor, int flag)
{
    this->selectedCells(cursor);

    // 插入行
    if(flag == -1)
    {
        // 在选择区域前
        this->_table->insertColumns(this->firstColumn,1);
    }
    else if(flag == 1)
    {
        this->_table->insertColumns(this->firstColumn + 1, 1);
    }

    if(!this->isDefaultStyle)
    {
        // 如果是自定义列宽
        double width = this->colWidth[this->firstColumn].rawValue();
        this->colWidth.remove(this->firstColumn);
        this->colWidth.insert(
                    this->firstColumn,
                    QTextLength(
                        QTextLength::PercentageLength,
                        width / 2));
        this->colWidth.insert(
                    this->firstColumn,
                    QTextLength(
                        QTextLength::PercentageLength,
                        width / 2));
    }
    this->updateStyle();
}

void DocTable::mergeCells()
{
    QTextCursor cursor = this->textCursor();
    this->_table->mergeCells(cursor);
}

///
/// \brief DocTable::splitCells
///     将已经合并的项给分离开来
///
void DocTable::splitCells()
{
    QTextCursor cursor = this->textCursor();
    this->selectedCells(cursor);

    this->_table->splitCell(
                this->firstRow,
                this->firstColumn,
                1,
                1);
//    qDebug() <<"split cells";
}

///
/// \brief DocTable::delRow
///     删除当前行
///
void DocTable::delRow()
{
    QTextCursor cursor = this->textCursor();
    this->selectedCells(cursor);

    this->_table->removeRows(this->firstRow, 1);
}

///
/// \brief DocTable::delCol
///     删除当前列
///
void DocTable::delCol()
{
    QTextCursor cursor = this->textCursor();
    this->selectedCells(cursor);

    this->_table->removeColumns(this->firstColumn, 1);
    this->updateStyle();
}

///
/// \brief DocTable::tableSetting
///     给出表格设置窗口，然后设置表格
///
void DocTable::tableSetting()
{
    TableSettingDialog* dialog = TableSettingDialog::getInstance();
    dialog->init(this);

    dialog->exec();
}

void DocTable::updateStyle()
{

    if(this->isDefaultStyle)
    {
        this->setDefaultStyle();
    }
    else
    {
        // 按照设置的比例调节
        QTextTableFormat format = this->_table->format();
        if(format.columnWidthConstraints().size() == this->getColumns())
        {
            format.setColumnWidthConstraints(this->colWidth);
            this->_table->setFormat(format);
        }

    }
}

void DocTable::focusInEvent(QFocusEvent *e)
{
    emitFormatSignals();
    QTextEdit::focusInEvent(e);
}

void DocTable::focusOutEvent(QFocusEvent *e)
{
    QTextEdit::focusOutEvent(e);
}

void DocTable::selectedCells(QTextCursor &cursor)
{
    // 这里可以检测是否复选单元格
    cursor.selectedTableCells(
                &this->firstRow,
                &this->rows,
                &this->firstColumn,
                &this->columns);

    if(
            this->firstRow == -1
            && this->rows == -1
            && this->firstColumn == -1
            && this->columns == -1)
    {
        QTextTableCell cell = this->_table->cellAt(cursor);
        this->firstRow = cell.row();
        this->rows = cell.rowSpan();
        this->firstColumn = cell.column();
        this->columns = cell.columnSpan();
    }

}

///
/// \brief DocTable::checkCursorInTable
///     与光标改变事件相关联，如果光标出现在了表格外部，
/// 则将它重新移动回到表格内部
///
void DocTable::checkCursorInTable()
{
    QTextCursor cursor = this->textCursor();
    int cursorPosition = cursor.position();                 // 光标位置

    if(!(cursorPosition >= this->_table->firstPosition()
            && cursorPosition <= this->_table->lastPosition()))
    {
        // 如果光标离开了表格的范围
        this->setTextCursor(this->_table->firstCursorPosition());
    }

}

void DocTable::init()
{
//    this->setFrameStyle(QFrame::NoFrame);
    this->setMinimumSize(50,10);

    this->firstRow = 0;
    this->firstColumn = 0;
    this->rows = 1;
    this->columns = 1;

    this->isDefaultStyle = true;
    this->updateStyle();            // 更新样式
    this->setCellPadding(5);

    this->initAction();
    this->initMenu();
    this->initConnection();

//    this->setCellPadding(5);

}

///
/// \brief DocTable::initAction
///     初始化QAction
void DocTable::initAction()
{
    // 插入
    this->actionInsertColAfter = new QAction(tr("insert Col After"), this);
    this->actionInsertColBefore = new QAction(tr("insert Col Before"), this);
    this->actionInsertRowAfter = new QAction(tr("insert row after"), this);
    this->actionInsertRowBefore = new QAction(tr("insert row before"), this);

    this->actionMergeCells = new QAction(tr("Merge Cells"), this);      // 合并单元格
    this->actionSplitCells = new QAction(tr("Split Cells"), this);      // 拆分单元格

    this->actionDeleteColumn = new QAction(tr("delete column"), this);  // 删除列
    this->actionDeleteRow = new QAction(tr("delete row"), this);        // 删除整行
    this->actionTableSetting = new QAction(tr("table setting"), this);  // 表格设置

}

void DocTable::initMenu()
{
    this->ContextMenu = new QMenu(tr("DocTable"));

    // 基本功能
    this->ContextMenu->addAction(this->actionCut);
    this->ContextMenu->addAction(this->actionCopy);
    this->ContextMenu->addAction(this->actionPaste);
    this->ContextMenu->addAction(this->actionSelectAll);

    this->ContextMenu->addSeparator();                      // 增加分界线
    this->ContextMenu->addAction(this->actionBold);         // 加粗
    this->ContextMenu->addAction(this->actionItalic);       // 斜体
    this->ContextMenu->addAction(this->actionColor);        // 颜色

    this->ContextMenu->addSeparator();                      // 分界线
    this->ContextMenu->addAction(this->actionFontSetTest);  // 字体
    this->ContextMenu->addAction(this->actionParagraph);    // 段落设置

    this->ContextMenu->addSeparator();                      // -------------
    this->ContextMenu->addAction(this->actionTableSetting); // 表格设置

    this->menu_insertRowOrCol = new QMenu(tr("Insert Row or Col"));     // 插入行或列
    this->menu_insertRowOrCol->addAction(this->actionInsertRowBefore);
    this->menu_insertRowOrCol->addAction(this->actionInsertRowAfter);
    this->menu_insertRowOrCol->addAction(this->actionInsertColBefore);
    this->menu_insertRowOrCol->addAction(this->actionInsertColAfter);

    this->ContextMenu->addMenu(this->menu_insertRowOrCol);
    this->ContextMenu->addAction(this->actionMergeCells);   // 合并单元格
    this->ContextMenu->addAction(this->actionSplitCells);   // 拆分单元格
    this->ContextMenu->addAction(this->actionDeleteRow);    // 删除该行
    this->ContextMenu->addAction(this->actionDeleteColumn); // 删除该列

}

void DocTable::initConnection()
{

    // 在当前单元格之前插入列
    this->connect(this->actionInsertColBefore, SIGNAL(triggered(bool)),
                  this, SLOT(insertColBefore()));

    // 在当前单元格之后插入列
    this->connect(this->actionInsertColAfter, SIGNAL(triggered(bool)),
                  this, SLOT(insertColAfter()));

    // 在当前单元格之前插入行
    this->connect(this->actionInsertRowBefore, SIGNAL(triggered(bool)),
                  this, SLOT(insertRowBefore()));

    // 在当前单元格之后插入
    this->connect(this->actionInsertRowAfter, SIGNAL(triggered(bool)),
                  this, SLOT(insertRowAfter()));

    // 合并单元格
    this->connect(this->actionMergeCells, SIGNAL(triggered(bool)),
                  this, SLOT(mergeCells()));

    // 拆分单元格-------我觉得拆分单元格要慎重-就只把原本合并的单元格分开就好了？
    this->connect(this->actionSplitCells, SIGNAL(triggered(bool)),
                  this, SLOT(splitCells()));

    // 删除整行
    this->connect(this->actionDeleteRow, SIGNAL(triggered(bool)),
                  this, SLOT(delRow()));

    // 删除整列
    this->connect(this->actionDeleteColumn, SIGNAL(triggered(bool)),
                  this, SLOT(delCol()));

    // 表格设置
    this->connect(this->actionTableSetting, SIGNAL(triggered(bool)),
                    this, SLOT(tableSetting()));

    // 自动调整大小
    this->connect(this, SIGNAL(textChanged()),
                  this, SLOT(blockSizeChanged()));

    // 防止光标移动到表格以外
    this->connect(this, SIGNAL(cursorPositionChanged()),
                  this, SLOT(checkCursorInTable()));
}

///
/// \brief DocTable::blockSizeChanged
///
void DocTable::blockSizeChanged()
{
//    qDebug() << "block resize";

    QTextDocument* doc = this->document();      // 获得文档
    int newHeight = doc->size().height();

    int oldWidth = (int)(this->block->size().width() + 0.5);
    int oldHeight = (int)(this->block->size().height() + 0.5);

    if(oldHeight - newHeight > 5
            || oldHeight - newHeight < -5)
    {
        // 如果需要调整大小
        this->block->resize(oldWidth, newHeight);
    }

}
