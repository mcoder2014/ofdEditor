#include "DocTable.h"
#include <QTextCursor>
#include "Doc/DocBlock.h"
#include "Doc/DocLayer.h"
#include "Doc/DocPage.h"
#include "Doc/DocPassage.h"
#include <QMenu>
#include <QVector>
#include <QDebug>

DocTable::DocTable(QWidget *parent)
    :QTextEdit(parent)
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
    :QTextEdit(0)
{
    QTextCursor cursor = this->textCursor();        // 获得光标
    this->_table = cursor.insertTable(rows, columns);         // 插入一个表格

    this->init();
    this->initConnection();

}


DocTable::~DocTable()
{

}

DocPassage *DocTable::getPassage()
{
    DocBlock *block = this->_block;
    if(block == NULL)
        return NULL;

    return block->getPassage();
}

DocPage *DocTable::getPage()
{
    DocBlock *block = this->_block;
    if(block == NULL)
        return NULL;

    return block->getPage();
}

DocLayer *DocTable::getLayer()
{
    DocBlock *block = this->_block;
    if(block == NULL)
        return NULL;

    return block->getLayer();
}

DocBlock *DocTable::getBlock()
{
    return this->_block;
}


///
/// \brief DocTable::getMenu
///     关于表格操作的菜单
/// \return
///
QMenu *DocTable::getMenu()
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
    this->_block = block;
    this->connect(block, SIGNAL(signal_resize(qreal,qreal,qreal,qreal)),
                  this, SLOT(blockSizeChanged()));
//    qDebug() << "set block success";
}

void DocTable::init()
{
    this->setFrameStyle(QFrame::NoFrame);
    this->setMinimumSize(10,10);

    // 关闭滚动条
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    // 设置为背景透明
    this->viewport()->setAttribute(Qt::WA_TranslucentBackground, true);
    this->setDefaultStyle();            // 设置默认样式

}

void DocTable::initMenu()
{

}

void DocTable::initConnection()
{
//    qDebug() << "init connection";
    this->connect(this, SIGNAL(textChanged()),
                  this, SLOT(blockSizeChanged()));
//    qDebug() << "init connection success";
}

///
/// \brief DocTable::blockSizeChanged
///
void DocTable::blockSizeChanged()
{
    qDebug() << "block resize";

    QTextDocument* doc = this->document();      // 获得文档
    int newHeight = doc->size().height();

    int oldWidth = (int)(this->_block->size().width() + 0.5);
    int oldHeight = (int)(this->_block->size().height() + 0.5);

    if(oldHeight - newHeight > 5
            || oldHeight - newHeight < -5)
    {
        // 如果需要调整大小
        this->_block->resize(oldWidth, newHeight);
    }

}
