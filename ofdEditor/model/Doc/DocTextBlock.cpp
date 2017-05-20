#include "DocTextBlock.h"
#include "Doc/DocParagraph.h"

#include <QTextCursor>
#include <QPalette>
#include <QMenu>
#include <QAction>

DocTextBlock::DocTextBlock(QWidget *parent)
    :QTextEdit(parent)
{
//    QTextCursor cursor(this->textCursor());
//    cursor.insertText(tr("testsesetstsetestes"));

//    this->setBackgroundRole(QPalette::Dark);
    this->setMinimumSize(15,15);        // 为了正确显示缩放标识
}

DocTextBlock::~DocTextBlock()
{

}

/**
 * @Author Chaoqun
 * @brief  显示右键菜单
 * @param  QContextMenuEvent *event
 * @return void
 * @date   2017/05/20
 */
void DocTextBlock::contextMenuEvent(QContextMenuEvent *event)
{

    QMenu * menu = createStandardContextMenu();     // 拓展标准菜单
    menu->addAction(tr("test extend the menu"));
    menu->exec(event->globalPos());
    delete menu;        // 删除菜单
}
