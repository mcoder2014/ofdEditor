#include "DocTextBlock.h"
#include "Widget/ParagraphFormatDialog.h"
#include "Widget/FontSettingDialog.h"
#include "Doc/DocPage.h"
#include "Doc/DocLayer.h"
#include "Doc/DocPassage.h"
#include "Command/SetTextBlodCmd.h"

#include <QTextCursor>
#include <QPalette>
#include <QMenu>
#include <QAction>
#include <QString>
#include <QDebug>
#include <QFontDialog>
#include <QColorDialog>
#include <QTextBlock>
#include <QClipboard>
#include <QApplication>

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
 * @brief  获得TextBlock中的所有文本
 * @param  void
 * @return QString &
 * @date   2017/06/25
 */
QString DocTextBlock::getContent()
{
    QString content = this->document()->toPlainText();  // 获得纯文本
    return content;
}

/**
 * @Author Chaoqun
 * @brief  蝴蝶文字框内容长度
 * @param  void
 * @return int
 * @date   2017/06/25
 */
int DocTextBlock::getContentLength()
{
    QString content = this->document()->toPlainText();
    return content.length();
}

///
/// \brief DocTextBlock::getType
/// \return
///
QString DocTextBlock::getType()
{
    QString summary = this->document()->toPlainText().left(5);
    return tr("DocTextBlock") + summary;
}

///
/// \brief DocTextBlock::getMenu
///     将文本块中间所需的菜单返回出来
/// \return
///
QMenu *DocTextBlock::getMenu()
{

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

    return this->ContextMenu;       // 返回ContextMenu
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
    emit signals_remove(this);      // 发送信号，remove
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
//    QTextCharFormat fmt;
//    QTextCharFormat currentFormat = this->currentCharFormat();      // 当前选择文字的样式
//    fmt.setFontWeight(currentFormat.fontWeight() != QFont::Bold ?   // 设置粗细
//                          QFont::Bold : QFont::Normal);

//    mergeFormatOnWordOrSelection(fmt);      // 合并格式
    QTextCursor cursor=this->textCursor();
    textBold(cursor,0);
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

    if(currentCharFormat.fontWeight()==QFont::Normal)
    {
        DocPassage *parentPassage=this->getPassage();
        QUndoCommand *setTextBlodCmd=new SetTextBlodCmd(this,cursor);
        parentPassage->undoStack->push(setTextBlodCmd);
        qDebug()<<"stack size:"<<parentPassage->undoStack->count();
    }
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

//    ParagraphFormatDialog * para = new ParagraphFormatDialog(
//                blockFormat,this);

//    para->exec();
    ParagraphFormatDialog* para =
            ParagraphFormatDialog::getInstance();  // 获得实例
    para->init(blockFormat, this);
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

///
/// \brief DocTextBlock::customFontDialog
///     自定义的字体设置窗口
///
void DocTextBlock::customFontDialog()
{

    FontSettingDialog* font = FontSettingDialog::getInstance(); // 获取实例
    font->init(this);
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
//        qDebug() << "cursor has no selection!";
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
//        qDebug() << "cursor has no selection!";
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
//        qDebug() << "cursor has no selection!";
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
 * @brief  设置选择文字的格式
 * @param  QTextCharFormat &format
 * @return void
 * @date   2017/06/23
 */
void DocTextBlock::setCharFormatOnSelection(
        QTextCharFormat &format)
{
    QTextCursor cursor = this->textCursor();    // 获得光标
    cursor.setCharFormat(format);               // 设置字符格式
    this->setTextCursor(cursor);                    // 并应用光标
}

/**
 * @Author Chaoqun
 * @brief  设置给定光标的，选择的文字的格式
 * @param  QTextCursor &cursor
 * @param  QTextCharFormat &format
 * @return void
 * @date   2017/06/23
 */
void DocTextBlock::serCharFormatOnSelection(
        QTextCursor &cursor, QTextCharFormat &format)
{
    cursor.setCharFormat(format);               // 设置字符格式
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

    emitFormatSignals();            // 当鼠标移进时，必须发出信号

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

    QTextEdit::focusOutEvent(e);
}

/**
 * @Author Chaoqun
 * @brief  用来检查当前的格式是否发生改变，并向外界发出信号
 * @param  参数
 * @return 返回值
 * @date   2017/06/22
 */
void DocTextBlock::checkCurrentFormat()
{
    QTextCursor cursor = this->textCursor();        //获得当前的光标
    // 比较当前光标位置的类型和已有的是否一致，不一致的话，当前的类型，并发射信号
    QTextBlockFormat blockFormat = cursor.blockFormat();    // 块格式
    QTextCharFormat charFormat = cursor.charFormat();       // 字符格式

    // 处理字符格式
    if(this->_currentCharFormat == charFormat)
    {
        // 如果字符格式相等
//        qDebug() << "charFormat does not change!";
    }
    else
    {
//        qDebug() << "charFormat changes! "
//                 << "And emit signal signals_currentCharFormatChanged!";
        // 如果字符格式变化
        this->_currentCharFormat = charFormat;          // 更新char格式
        emit this->signals_currentCharFormatChanged(charFormat);    // 发射char格式变更信号
    }

    // 处理块格式
    if(this->_currentBlockFormat == blockFormat)
    {
        // 格式相等
//        qDebug() << "blockFormat does not change!";
    }
    else
    {
//        qDebug() << "blockFormat changes! "
//                 << "And emit signal signals_currentBlockFormatChanged!";
        // 格式不相等
        this->_currentBlockFormat = blockFormat;    // 存储新格式
        emit this->signals_currentBlockFormatChanged(blockFormat);      // 发射blockFormat变更信号
    }
    emit this->signals_currentTextBlock(this);      // 发射Text Block信号
//    qDebug() << "check format";
}

/**
 * @Author Chaoqun
 * @brief  将所有的信号发出
 * @param  void
 * @return void
 * @date   2017/06/23
 */
void DocTextBlock::emitFormatSignals()
{
    QTextCursor cursor = this->textCursor();    // 光标
    this->_currentBlockFormat = cursor.blockFormat();   // 获得块格式
    this->_currentCharFormat = cursor.charFormat();     // 获得字符格式

    // 发射三个信号
    emit this->signals_currentBlockFormatChanged(this->_currentBlockFormat);
    emit this->signals_currentCharFormatChanged(this->_currentCharFormat);
    emit this->signals_currentTextBlock(this);
}

void DocTextBlock::printTestMessage()
{
    qDebug() << "Click Right Button";
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

    this->initFormat();                                 // 初始化格式

    QTextCursor cursor = this->textCursor();            // 获得光标
    this->_currentBlockFormat = cursor.blockFormat();   // 块格式
    this->_currentCharFormat = cursor.charFormat();     // 字符格式

    // 连接当前光标位置改变事件
    connect(this, SIGNAL(cursorPositionChanged()),
            this, SLOT(checkCurrentFormat()));

    this->initAcitons();    // 初始化QAction相关
    this->initMenu();       // 初始化右键菜单
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

    // 段落
    this->actionParagraph = new QAction(tr("Paragraph"),NULL);
    this->connect(this->actionParagraph,SIGNAL(triggered()),
                  this,SLOT(textParagraph()));

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

///
/// \brief DocTextBlock::initMenu
///         初始化右键菜单
///
void DocTextBlock::initMenu()
{
    this->ContextMenu = new QMenu(tr("DocTextBlock"));

    // 基本功能
    this->actionCut = this->ContextMenu->addAction(tr("Cut"));              // 剪切
    this->actionCopy = this->ContextMenu->addAction(tr("Copy"));            // 复制
    this->actionPaste = this->ContextMenu->addAction(tr("Paste"));          // 粘贴
    this->actionSelectAll = this->ContextMenu->addAction(tr("SelectAll"));  // 全选

    connect(this->actionCut, SIGNAL(triggered(bool)),
            this, SLOT(cut()));
    connect(this->actionCopy, SIGNAL(triggered(bool)),
            this, SLOT(copy()));
    connect(this->actionPaste, SIGNAL(triggered(bool)),
            this, SLOT(paste()));
    connect(this->actionSelectAll, SIGNAL(triggered(bool)),
            this, SLOT(selectAll()));

    this->ContextMenu->addSeparator();                      // 增加分界线
    this->ContextMenu->addAction(this->actionBold);         // 加粗
    this->ContextMenu->addAction(this->actionItalic);       // 斜体
//    this->ContextMenu->addAction(this->actionUnderline);    // 下划线
    this->ContextMenu->addAction(this->actionColor);        // 颜色

    this->ContextMenu->addSeparator();                      // 分界线
    this->ContextMenu->addAction(this->actionFontSetTest);  // 字体
    this->ContextMenu->addAction(this->actionParagraph);    // 段落设置


}
