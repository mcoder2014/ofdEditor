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

DocTable::DocTable(QWidget *parent)
    :DocTextBlock(1.0)
{
    QTextCursor cursor = this->textCursor();        // 获得光标
    this->_table = cursor.insertTable(3,4);         // 插入一个表格

    this->init();
    this->initConnection();

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
    this->initConnection();

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

    return this->ContextMenu;
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

    this->_table->setFormat(format);
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

void DocTable::focusInEvent(QFocusEvent *e)
{
    emitFormatSignals();
    QTextEdit::focusInEvent(e);
}

void DocTable::focusOutEvent(QFocusEvent *e)
{
    QTextEdit::focusOutEvent(e);
}

void DocTable::init()
{
//    this->setFrameStyle(QFrame::NoFrame);
    this->setMinimumSize(50,10);

//    // 关闭滚动条
//    this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
//    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
//    // 设置为背景透明
//    this->viewport()->setAttribute(Qt::WA_TranslucentBackground, true);
    this->setDefaultStyle();            // 设置默认样式

    this->initMenu();

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

    qDebug() << "init Menu" <<this->ContextMenu;

}

void DocTable::initConnection()
{
    this->connect(this, SIGNAL(textChanged()),
                  this, SLOT(blockSizeChanged()));
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
