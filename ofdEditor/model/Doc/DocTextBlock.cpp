#include "DocTextBlock.h"
#include "Doc/DocParagraph.h"

#include <QTextCursor>
#include <QPalette>
#include <QMenu>
#include <QAction>
#include <QString>

DocTextBlock::DocTextBlock(QWidget *parent)
    :QTextEdit(parent)
{
//    QTextCursor cursor(this->textCursor());
//    cursor.insertText(tr("testsesetstsetestes"));

//    this->setBackgroundRole(QPalette::Dark);
    this->init();   // 调用初始化函数

}

DocTextBlock::~DocTextBlock()
{

}

/**
 * @Author Chaoqun
 * @brief  设置文本内容
 * @param  QString str
 * @return void
 * @date   2017/05/20
 */
void DocTextBlock::setContent(QString str)
{
    QTextCursor cursor(this->textCursor());
    cursor.insertText(str);
}

/**
 * @Author Chaoqun
 * @brief  用来合并格式
 * @param  const QTextCharFormat &format
 * @return void
 * @date   2017/05/20
 */
void DocTextBlock::mergeFormatOnWordOrSelection(const QTextCharFormat &format)
{
    QTextCursor cursor(this->textCursor()); // 新建光标
    if(!cursor.hasSelection())
    {
        // 如果没有选择文字段落
        cursor.select(QTextCursor::WordUnderCursor);
    }
    cursor.mergeCharFormat(format);         // 合并光标下的 QTextCharFormat
    this->mergeCurrentCharFormat(format);   // 合并当前的 QTextCharFormat

}

/**
 * @Author Chaoqun
 * @brief  文字加粗
 * @param  void
 * @return void
 * @date   2017/05/20
 */
void DocTextBlock::textBold()
{
    QTextCharFormat fmt;
    fmt.setFontWeight(actionTextBold->isChecked() ?
                          QFont::Bold : QFont::Normal);
    mergeFormatOnWordOrSelection(fmt);
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
    menu->addAction(tr("test extend the menu"));    // 加去QAction
    menu->exec(event->globalPos());
    delete menu;        // 删除菜单
}

/**
 * @Author Chaoqun
 * @brief  初始化函数
 * @param  void
 * @return void
 * @date   2017/05/20
 */
void DocTextBlock::init()
{
    this->setMinimumSize(15,15);        // 为了正确显示缩放标识
}
