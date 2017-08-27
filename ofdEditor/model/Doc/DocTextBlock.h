#ifndef DOCTEXTBLOCK_H
#define DOCTEXTBLOCK_H

#include "model_global.h"       // 导出lib使用
#include "Doc/DocBlock.h"
#include <QVector>
#include <QTextEdit>
#include <QGraphicsProxyWidget>
#include <QColorDialog>

//class DocParagraph;
class QAction;
class QTextBlockFormat;     // 块格式
class QTextFrameFormat;     // 框架格式
class QTextCharFormat;      // 字节格式
class QFont;                // 字体格式
class ParagraphFormatDialog;// 段落样式
class DocPassage;           // 文章
class DocPage;              // 获得所在页
class DocLayer;             // 层
class commands;

class MODELSHARED_EXPORT DocTextBlock
        :public QTextEdit
{
    Q_OBJECT
public:
    DocTextBlock(QWidget *parent = NULL);
    ~DocTextBlock();
    void setContent(QString str);       // 设置内容

    DocPassage* getPassage();           // 获得文章
    DocPage* getPage();                 // 获得页
    DocLayer* getLayer();               // 获得层
    DocBlock* getBlock();               // 获得Block
    QString getContent();               // 获得TextBlock中的所有文本
    int getContentLength();             // 获得内部文字长度
    QString getType();                  // 获得标识，来区分不同的块
    QMenu* getMenu();                   // 获得该块的菜单成分

public slots:

    void textBold();            // 将光标选择的文字粗体
    void textBold(QTextCursor& cursor, int mode = 0);     // 将给定光标选择的文字设置为粗体
    void textUnderline();       // 下划线
    void textUnderline(QTextCursor& cursor, int mode = 0);// 设置下划线
    void textItalic();          // 斜体
    void textItalic(QTextCursor& cursor, int mode = 0);    // 设置斜体
    void setTextColor();        // 设置字体颜色
    void setTextColor(QTextCursor& cursor,QColor color);    // 设置颜色

    void textParagraph();       // 设置段落
    void customFontDialog();    // 自定义的字体窗口设置

    void setTextBlockFormat(
            QTextBlockFormat& blockFormat);   // 设置块格式
    void setTextBlockFormat(
            QTextCursor& cursor,
            QTextBlockFormat& blockFormat);   // 设置给定光标下块格式

    void setCharFormatOnWordOrSelection(
            QTextCharFormat &format);     // 设置选中字段的QTextCharFormat
            // 如果没有选中，则会选择光标所在行的字体
    void setCharFormatOnWordOrSelection(
            QTextCursor& cursor,
            QTextCharFormat &format);     // 设置给定光标的QTextCharFormat

    void setCharFormatOnSelection(
            QTextCharFormat &format);     // 设置选中字段的QTextCharFormat
    void serCharFormatOnSelection(
            QTextCursor& cursor,
            QTextCharFormat &format);     // 设置给定光标下，文字的格式

    void mergeFormatOnWordOrSelection(
            QTextCharFormat &format);     // 合并格式
    void mergeFormatOnWordOrSelection(
            QTextCursor& cursor,
            QTextCharFormat &format);     // 合并指定光标处的文字格式

    void mergeBlockFormatOnBlock(
            QTextBlockFormat& blockFormat);     // 合并blockFormat格式
    void mergeBlockFormatOnBlock(
            QTextCursor cursor,
            QTextBlockFormat &blockFormat);     // 合并给定光标下块的格式

    void setFont(const QFont &font);            // 设置格式
    void setFont(
            QTextCursor& cursor,
            const QFont &font);                 // 设置给定光标下的字体格式

    // 关于框的一些其他部分
    void setBlock(DocBlock* block);             // 设置Block
    void remove();                              // 移除本文本框
    void showBoundaryFrame(bool show);          // 是否显示边界

protected:
    void focusInEvent(QFocusEvent *e);
    void focusOutEvent(QFocusEvent *e);

private slots:
    void checkCurrentFormat();                  // 检查当前的格式是否发生改变
    void emitFormatSignals();                   // 发射格式的信号
    void textBlockSizeChanged();                // 当文字内容改变时自动调整文字大小

private:
    QString content;        // 文字内容
    void init();            // 初始化
    void initAcitons();     // 初始化事件
    void initFormat();      // 初始化文字样式
    void initMenu();        // 初始化右键菜单

    // QActions
    QAction * actionCut;        // 剪贴
    QAction * actionCopy;       // 复制
    QAction * actionPaste;      // 粘贴
    QAction * actionSelectAll;  // 全选

    QAction * actionBold;       // 加粗
    QAction * actionUnderline;  // 下划线
    QAction * actionItalic;     // 斜体
    QAction * actionColor;      // 设置颜色
    QAction * actionParagraph;  // 设置段落
    QAction * actionFontSetTest;// 新字体窗口测试

    QMenu * ContextMenu;        // 右键菜单
    DocBlock* block;            // 本类型所在的block

    QTextBlockFormat _currentBlockFormat;       // 当前BlockFormat
    QTextCharFormat _currentCharFormat;         // 当前

signals:
    void signals_remove(DocTextBlock* textBlock);    // 移除文本框的信号。
    void signals_setZValue(qreal z);                 // 设置Z值的信号

    void signals_currentCharFormatChanged(
            QTextCharFormat fmt);    // 当前选择的charFormat发生了变化
    void signals_currentBlockFormatChanged(
            QTextBlockFormat fmt);  // 当前选择的block格式发生了变化、
    void signals_currentTextBlock(DocTextBlock* textBlock); // 当前操作的textBlock

};

#endif // DOCTEXTBLOCK_H
