#include "DocTextBlock.h"
#include "Doc/DocParagraph.h"
#include "Widget/ParagraphFormatDialog.h"
#include "Widget/FontSettingDialog.h"
#include "Doc/DocPage.h"
#include "Doc/DocLayer.h"
#include "Doc/DocPassage.h"

#include <QTextCursor>
#include <QPalette>
#include <QMenu>
#include <QAction>
#include <QString>
#include <QDebug>
#include <QFontDialog>
#include <QColorDialog>


DocTextBlock::DocTextBlock(QWidget *parent)
    :QTextEdit(parent)
{
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
 * @brief  获得文章
 * @param  void
 * @return 返回值
 * @date   2017/06/21
 */
DocPassage *DocTextBlock::getPassage()
{
    DocPage* page = this->getPage();        // 查找所在的页
//    qDebug() << "getPassage success";
    if(page == NULL)
        return NULL;
    return page->getPassage();              // 返回所在页所在的文章
}

/**
 * @Author Chaoqun
 * @brief  获得所在的页
 * @param  void
 * @return DocPage*
 * @date   2017/06/21
 */
DocPage *DocTextBlock::getPage()
{
    DocLayer* layer = this->getLayer();     // 获得层
    if(layer == NULL)
        return NULL;
//    qDebug() << "GetPage success";
    return layer->getPage();                // 由层去获得passage
}

/**
 * @Author Chaoqun
 * @brief  获得所在的层
 * @param  void
 * @return DocLayer*
 * @date   2017/06/21
 */
DocLayer *DocTextBlock::getLayer()
{
    DocBlock* block = this->block;
    if(block == NULL)
        return NULL;
//    qDebug()<<"get layer success";
    return block->getLayer();
}

/**
 * @Author Chaoqun
 * @brief  获得Block的指针
 * @param  void
 * @return DocBlock*
 * @date   2017/06/21
 */
DocBlock *DocTextBlock::getBlock()
{
//    qDebug()<<"getBlock test success";
    return this->block;
}

/**
 * @Author Chaoqun
 * @brief  用来合并格式
 * @param  const QTextCharFormat &format
 * @return void
 * @date   2017/05/20
 */
void DocTextBlock::mergeFormatOnWordOrSelection(
        QTextCharFormat &format)
{
    QTextCursor cursor = this->textCursor(); // 新建光标
    if(!cursor.hasSelection())
    {
        // 如果没有选择文字段落
        cursor.select(QTextCursor::WordUnderCursor);
        qDebug() << "cursor has no selection!";
    }
    cursor.mergeCharFormat(format);         // 合并光标下的 QTextCharFormat
    this->mergeCurrentCharFormat(format);   // 合并当前的 QTextCharFormat

}

/**
 * @Author Chaoqun
 * @brief  合并指定光标下的char和文字格式
 * @param  QTextCursor &cursor
 * @param  QTextCharFormat &format
 * @return void
 * @date   2017/06/22
 */
void DocTextBlock::mergeFormatOnWordOrSelection(
        QTextCursor &cursor, QTextCharFormat &format)
{
    if(!cursor.hasSelection())
    {
        // 如果没有选择文字段落
        cursor.select(QTextCursor::WordUnderCursor);
        qDebug() << "cursor has no selection!";
    }
    cursor.mergeCharFormat(format);         // 合并光标下的 QTextCharFormat
//    this->mergeCurrentCharFormat(format);   // 合并当前的 QTextCharFormat
}

/**
 * @Author Chaoqun
 * @brief  合并当前光标下块的格式
 * @param  QTextBlockFormat &blockFormat
 * @return void
 * @date   2017/06/22
 */
void DocTextBlock::mergeBlockFormatOnBlock(
        QTextBlockFormat &blockFormat)
{
    QTextCursor cursor = this->textCursor();        // 光标
    if(!cursor.hasSelection())
    {
        cursor.select(QTextCursor::BlockUnderCursor);   // 选择光标下的块
    }

    cursor.mergeBlockFormat(blockFormat);
}

/**
 * @Author Chaoqun
 * @brief  合并给定光标下的块格式
 * @param  QTextCursor &cursor
 * @param  QTextBlockFormat &blockFormat
 * @return void
 * @date   2017/06/22
 */
void DocTextBlock::mergeBlockFormatOnBlock(
        QTextCursor cursor, QTextBlockFormat &blockFormat)
{
    if(!cursor.hasSelection())
    {
        cursor.select(QTextCursor::BlockUnderCursor);   // 选择光标下的块
    }

    cursor.mergeBlockFormat(blockFormat);
}


void DocTextBlock::setFont(const QFont &font)
{

    QTextCursor cursor = this->textCursor();
    QTextCharFormat currentFormat = cursor.charFormat();

    currentFormat.setFont(font);            // 设置字体

    mergeFormatOnWordOrSelection(currentFormat);    // 合并字体样式
}

/**
 * @Author Chaoqun
 * @brief  设置给定光标下的字体格式
 * @param  QTextCursor &cursor
 * @param  const QFont &font
 * @return void
 * @date   2017/06/22
 */
void DocTextBlock::setFont(
        QTextCursor &cursor, const QFont &font)
{
    QTextCharFormat currentFormat = cursor.charFormat();

    currentFormat.setFont(font);            // 设置字体

    mergeFormatOnWordOrSelection(cursor, currentFormat);    // 合并给定光标下的字体样式
}

/**
 * @Author Chaoqun
 * @brief  设置block
 * @param  DocBlock *block
 * @return 返回值
 * @date   2017/06/21
 */
void DocTextBlock::setBlock(DocBlock *block)
{
    this->block = block;        // 设置Block
}

/**
 * @Author Chaoqun
 * @brief  通知DocBlock，从DocPage中移除本文本框
 * @param  void
 * @return void
 * @date   2017/06/20
 */
void DocTextBlock::remove()
{
    emit signals_remove();      // 发送信号，remove
}

/**
 * @Author Chaoqun
 * @brief  控制是否显示文本框的边界
 * @param  bool show
 * @return void
 * @date   2017/06/22
 */
void DocTextBlock::showBoundaryFrame(bool show)
{
    if(show)
    {
        this->setFrameStyle(QFrame::Box);           // 显示边框
    }
    else
    {
        this->setFrameStyle(QFrame::NoFrame);       // 隐藏边框
    }
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
    QTextCharFormat currentFormat = this->currentCharFormat();      // 当前选择文字的样式
    fmt.setFontWeight(currentFormat.fontWeight() != QFont::Bold ?   // 设置粗细
                          QFont::Bold : QFont::Normal);

    mergeFormatOnWordOrSelection(fmt);      // 合并格式

}

/**
 * @Author Chaoqun
 * @brief  使用QTextCursor作为参数，并且通过mode参数可以控制是否设置加粗，
 *          方便Command操作进行使用
 * mode = -1 普通，
 * mode = 0 一般模式同无参的这个函数
 * mode = 1 加粗
 * @param  QTextCursor &
 * @return void
 * @date   2017/06/20
 */
void DocTextBlock::textBold(QTextCursor &cursor, int mode)
{
    QTextCharFormat fmt;
    QTextCharFormat currentCharFormat = cursor.charFormat();
    switch (mode) {
    case -1:
        fmt.setFontWeight(QFont::Normal);           // 设置普通
        break;
    case 0:
        fmt.setFontWeight(currentCharFormat.fontWeight() != QFont::Bold ?   // 设置粗细
                             QFont::Bold : QFont::Normal);
        break;
    case 1:
        fmt.setFontWeight(QFont::Bold);             // 设置加粗
        break;
    }

    mergeFormatOnWordOrSelection(cursor,fmt);       // 合并给定光标下的字体
}

/**
 * @Author Chaoqun
 * @brief  设置下划线
 * @param  void
 * @return void
 * @date   2017/05/21
 */
void DocTextBlock::textUnderline()
{
    QTextCharFormat fmt;
    QTextCharFormat currentFormat = this->currentCharFormat();      // 当前选择文字的样式

    fmt.setFontUnderline(currentFormat.fontUnderline()?
                             false:true);

    mergeFormatOnWordOrSelection(fmt);      // 合并格式
}

/**
 * @Author Chaoqun
 * @brief  设置下划线
 * @param  QTextCursor& cursor
 * @param mode -1 取消下划线
 * @param mode 0 正常机制
 * @param mode 1 设置下划线
 * @return 返回值
 * @date   2017/06/22
 */
void DocTextBlock::textUnderline(QTextCursor &cursor, int mode)
{
    QTextCharFormat fmt;
    QTextCharFormat currentFormat = cursor.charFormat();      // 当前选择文字的样式

    switch (mode)
    {
    case -1:
        fmt.setFontUnderline(false);
        break;
    case 0:
        fmt.setFontUnderline(currentFormat.fontUnderline()?
                                 false:true);
    case 1:
        fmt.setFontUnderline(true);
    default:
        break;
    }

    mergeFormatOnWordOrSelection(cursor,fmt);      // 合并格式
}

/**
 * @Author Chaoqun
 * @brief  设置斜体
 * @param  void
 * @return void
 * @date   2017/05/21
 */
void DocTextBlock::textItalic()
{
    QTextCharFormat fmt;
    QTextCharFormat currentFormat = this->currentCharFormat();      // 当前选择文字的样式
    fmt.setFontItalic(currentFormat.fontItalic() == true?                  // 设置斜体
                          false:true);

    mergeFormatOnWordOrSelection(fmt);      // 合并格式
}

/**
 * @Author Chaoqun
 * @brief  设置斜体
 * @param  QTextCursor &cursor 选中文字的光标
 * @param mode -1 取消斜体
 * @param mode 0 正常模式
 * @param mode 1 加入斜体
 * @return void
 * @date   2017/06/22
 */
void DocTextBlock::textItalic(QTextCursor &cursor, int mode)
{
    QTextCharFormat fmt;
    QTextCharFormat currentFormat = cursor.charFormat();      // 当前选择文字的样式

    switch (mode) {
    case -1:
        fmt.setFontItalic(false);           // 取消斜体
        break;
    case 0:
        fmt.setFontItalic(currentFormat.fontItalic() == true?                  // 设置斜体
                              false:true);
        break;
    case 1:
        fmt.setFontItalic(true);            // 设置斜体
        break;
    default:
        break;
    }

    fmt.setFontItalic(currentFormat.fontItalic() == true?                  // 设置斜体
                          false:true);

    mergeFormatOnWordOrSelection(cursor,fmt);      // 合并格式
}

/**
 * @Author Chaoqun
 * @brief  弹出一个段落属性设置的QDialog，设置好了后，
 *          将该属性应用在选中的段落上
 * @param  void
 * @return void
 * @date   2017/05/21
 */
void DocTextBlock::textParagraph()
{
    QTextCursor cursor = this->textCursor();
    QTextBlockFormat blockFormat = cursor.blockFormat();

    ParagraphFormatDialog * para = new ParagraphFormatDialog(
                blockFormat,this);

    para->exec();
}
/**
 * @Author Chaoqun
 * @brief  弹出一个框，设置字体颜色，设置好了以后，将该属性应用到选中的文字中
 * @param  参数
 * @return 返回值
 * @date   2017/05/27
 */
void DocTextBlock::setTextColor()
{

    QColor color = QColorDialog::getColor(this->textColor(),
                                          NULL,
                                          tr("Choose a Color"),
                                          QColorDialog::ShowAlphaChannel);

    if(!color.isValid())
        return;
    QTextCharFormat charFormat;
    charFormat.setForeground(color);        // 设置颜色
    mergeCurrentCharFormat(charFormat);

}

/**
 * @Author Chaoqun
 * @brief  摘要
 * @param  参数
 * @return 返回值
 * @date   2017/06/22
 */
void DocTextBlock::setTextColor(QTextCursor& cursor,QColor color)
{
    QTextCharFormat fmt;
    fmt.setForeground(color);       // 设置前景-文字的颜色

    mergeFormatOnWordOrSelection(cursor,fmt);
}

/**
 * @Author Chaoqun
 * @brief  通过字体小窗口设置字体
 * @param  void
 * @return void
 * @date   2017/05/21
 */
void DocTextBlock::textFontDialog()
{
    bool btn_ok;    // 确认按键
    QTextCursor cursor = this->textCursor();
    QTextCharFormat currentFormat = cursor.charFormat();
//    QTextCharFormat currentFormat =
//            this->currentCharFormat();      // 当前选择文字的样式
    QFont oldFont = currentFormat.font();   // 获取之前的字体样式

    QFont newFont = QFontDialog::getFont(
                &btn_ok, oldFont,NULL,tr("Set the font")
                );    // 选择字体框

    if(btn_ok)
    {
        this->setFont(newFont);     // 设置字体
    }
    else
    {
        // 用户取消了操作，不做处理
        qDebug() << "Cancel select Font!";
    }


}

void DocTextBlock::customFontDialog()
{

    FontSettingDialog * font = new FontSettingDialog(this,0);
    font->exec();
}

/**
 * @Author Chaoqun
 * @brief  设置当前选中的段落的格式
 * @param  参数
 * @return 返回值
 * @date   2017/05/22
 */
void DocTextBlock::setTextBlockFormat(QTextBlockFormat &blockFormat)
{
    QTextCursor cursor = this->textCursor(); // 新建光标
    if(!cursor.hasSelection())
    {
        // 如果没有选择文字段落
        cursor.select(QTextCursor::WordUnderCursor);
        qDebug() << "cursor has no selection!";
    }

    cursor.setBlockFormat(blockFormat);


    // 发出信号
    emit this->signals_currentBlockFormatChanged(blockFormat);


}

/**
 * @Author Chaoqun
 * @brief  设置给定光标下的块的格式
 * @param  QTextCursor &cursor
 * @param  QTextBlockFormat &blockFormat
 * @return void
 * @date   2017/06/22
 */
void DocTextBlock::setTextBlockFormat(
        QTextCursor &cursor, QTextBlockFormat &blockFormat)
{
    if(!cursor.hasSelection())
    {
        // 如果没有选择文字段落
        cursor.select(QTextCursor::WordUnderCursor);
        qDebug() << "cursor has no selection!";
    }

    cursor.setBlockFormat(blockFormat);

}

/**
 * @Author Chaoqun
 * @brief  直接修改选中的文字的CharFormat
 * @param  const QTextCharFormat &format
 * @return void
 * @date   2017/05/25
 */
void DocTextBlock::setCharFormatOnWordOrSelection(
         QTextCharFormat &format)
{
    QTextCursor cursor = this->textCursor(); // 新建光标
    if(!cursor.hasSelection())
    {
        // 如果没有选择文字段落
        cursor.select(QTextCursor::WordUnderCursor);
        qDebug() << "cursor has no selection!";
    }
    cursor.setCharFormat(format);         // 设置光标下的 QTextCharFormat
    this->setCurrentCharFormat(format);   // 合并当前的 QTextCharFormat

    // 发射信号
    emit this->signals_currentCharFormatChanged(format);
}

/**
 * @Author Chaoqun
 * @brief  设置给定光标的char格式
 * @param  QTextCursor &cursor
 * @param  QTextCharFormat &format
 * @return void
 * @date   2017/06/22
 */
void DocTextBlock::setCharFormatOnWordOrSelection(
        QTextCursor &cursor, QTextCharFormat &format)
{
    if(!cursor.hasSelection())
    {
        // 如果没有选择文字段落
        cursor.select(QTextCursor::WordUnderCursor);
        qDebug() << "cursor has no selection!";
    }
    cursor.setCharFormat(format);         // 设置光标下的 QTextCharFormat
    this->setCurrentCharFormat(format);   // 合并当前的 QTextCharFormat
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

    this->ContextMenu = createStandardContextMenu();        // 拓展标准菜单
    this->ContextMenu->addAction(this->actionBold);         // 加粗
    this->ContextMenu->addAction(this->actionItalic);       // 斜体
    this->ContextMenu->addAction(this->actionUnderline);    // 下划线
    this->ContextMenu->addAction(this->actionColor);        // 颜色
    this->ContextMenu->addSeparator();                      // 分界线
    this->ContextMenu->addAction(this->actionFontSet);      // 字体设置
    this->ContextMenu->addAction(this->actionParagraph);    // 段落设置
    this->ContextMenu->addAction(this->actionFontSetTest);  // 字体
    this->ContextMenu->addAction(this->actionRemove);       // 移除操作

    connect(this->ContextMenu, SIGNAL(aboutToHide()),
            this,SLOT(contextMenuAboutToHideEvent()));      // 测试

    this->tempZValue = this->getBlock()->getZValue();
    emit this->signals_setZValue(2000);

    // 展示菜单
    this->ContextMenu->exec(event->globalPos());

}

/**
 * @Author Chaoqun
 * @brief  焦点聚焦，显示边框
 * @param  QFocusEvent *e
 * @return void
 * @date   2017/05/25
 */
void DocTextBlock::focusInEvent(QFocusEvent *e)
{
    this->showBoundaryFrame(true);
    emit this->signals_focusIn();

    QTextEdit::focusInEvent(e);
}

/**
 * @Author Chaoqun
 * @brief  焦点移出，隐藏边框
 * @param  QFocusEvent *e
 * @return void
 * @date   2017/05/25
 */
void DocTextBlock::focusOutEvent(QFocusEvent *e)
{
    this->showBoundaryFrame(false);
    emit this->signals_focusOut();

    QTextEdit::focusOutEvent(e);
}

/**
 * @Author Chaoqun
 * @brief  用来当右键菜单消失时，调整块的深度
 * @param  参数
 * @return 返回值
 * @date   2017/06/22
 */
void DocTextBlock::contextMenuAboutToHideEvent()
{
    emit this->signals_setZValue(this->tempZValue);         // 还原Z值
    this->focusInEvent(new QFocusEvent(QEvent::FocusIn));   // 关注它
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
    this->setMinimumSize(5,5);        // 为了正确显示缩放标识

    // 关闭滑动条
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    // 设置为背景透明
    this->viewport()->setAttribute(Qt::WA_TranslucentBackground, true);
    // 无边框
    this->setFrameStyle(QFrame::NoFrame);

    this->initFormat();         // 初始化格式



    // 连接当前charFormat改变函数
    connect(this, SIGNAL(currentCharFormatChanged(QTextCharFormat)),
            this, SLOT(currentCharFormatChangedEvent(QTextCharFormat)));

    // 连接当前光标位置改变事件
    connect(this, SIGNAL(cursorPositionChanged()),
            this, SLOT(cursorPositionChangedEvent()));

    this->initAcitons();    // 初始化QAction相关
}

/**
 * @Author Chaoqun
 * @brief  初始化按钮 ， QAction
 * @param  void
 * @return 返回值
 * @date   2017/05/21
 */
void DocTextBlock::initAcitons()
{
    // 粗体
    this->actionBold = new QAction(tr("Bold"),NULL);
    this->actionBold->setPriority(QAction::LowPriority);
    QFont bold;
    bold.setBold(true);
    this->actionBold->setFont(bold);

    this->connect(this->actionBold,SIGNAL(triggered()),
                  this,SLOT(textBold()));

    // 斜体
    this->actionItalic = new QAction(tr("Italic"),NULL);
    this->actionItalic->setPriority(QAction::LowPriority);
    QFont italic;
    italic.setItalic(true);
    this->actionItalic->setFont(italic);

    this->connect(this->actionItalic,SIGNAL(triggered()),
                  this,SLOT(textItalic()));

    // 下划线
    this->actionUnderline = new QAction(tr("Underline"),NULL);
    this->actionUnderline->setPriority(QAction::LowPriority);
    QFont underline;
    underline.setUnderline(true);
    this->actionUnderline->setFont(underline);

    this->connect(this->actionUnderline, SIGNAL(triggered()),
                  this,SLOT(textUnderline()));

    // 设置字体颜色
    this->actionColor = new QAction(tr("Color"),NULL);
    this->actionColor->setPriority(QAction::LowPriority);

    this->connect(this->actionColor,SIGNAL(triggered()),
                  this,SLOT(setTextColor()));

    // 字体
    this->actionFontSet = new QAction(tr("Font"),NULL);
    this->actionFontSet->setPriority(QAction::LowPriority);

    this->connect(this->actionFontSet,SIGNAL(triggered()),
                  this,SLOT(textFontDialog()));

    // 段落
    this->actionParagraph = new QAction(tr("Paragraph"),NULL);
    this->connect(this->actionParagraph,SIGNAL(triggered()),
                  this,SLOT(textParagraph()));

    // 移除文本框
    this->actionRemove = new QAction(tr("Remove"),NULL);

    this->connect(this->actionRemove,SIGNAL(triggered(bool)),
                  this,SLOT(remove()));         // 链接信号，可以移除文本框

    // 字体窗口测试
    this->actionFontSetTest = new QAction(tr("FontDialogTest"),NULL);
    this->connect(this->actionFontSetTest, SIGNAL(triggered()),
                  this, SLOT(customFontDialog()));

}


/**
 * @Author Chaoqun
 * @brief  初始化文字的样式
 * @param  void
 * @return void
 * @date   2017/05/25
 */
void DocTextBlock::initFormat()
{
    QTextCursor cursor = this->textCursor();            // 获得当前光标

    QTextCharFormat charFormat = cursor.charFormat();   // 字符格式
    charFormat.setVerticalAlignment(QTextCharFormat::AlignMiddle);

    this->mergeCurrentCharFormat(charFormat);

}
