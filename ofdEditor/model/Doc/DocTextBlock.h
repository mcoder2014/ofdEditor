#ifndef DOCTEXTBLOCK_H
#define DOCTEXTBLOCK_H

#include "model_global.h"       // 导出lib使用
#include "Doc/DocBlock.h"
#include <QVector>
#include <QTextEdit>
#include <QGraphicsProxyWidget>

class DocParagraph;
class QAction;
class QTextBlockFormat;     // 块格式
class QTextFrameFormat;     // 框架格式
class QTextCharFormat;      // 字节格式
class QFont;                // 字体格式
class ParagraphFormatWidget;

class MODELSHARED_EXPORT DocTextBlock
        :public QTextEdit
{
    Q_OBJECT
public:
    DocTextBlock(QWidget *parent = Q_NULLPTR);
    ~DocTextBlock();
    void setContent(QString str);               // 设置内容



public slots:
    void textBold();            // 粗体
    void textUnderline();
    void textItalic();
    void textParagraphWidget();     // 设置段落
//    void textFamily(const QString &f);
//    void textSize(const QString &p);
//    void textStyle(int styleIndex);
//    void textColor();
//    void textAlign(QAction *a);
    void textFontDialog();      // 通过字体小窗口设置字体


protected:

    void contextMenuEvent(QContextMenuEvent *event);    // 右键菜单重载

private slots:


    void mergeFormatOnWordOrSelection(
            const QTextCharFormat &format);     // 合并格式
    void currentCharFormatChangedEvent(
            const QTextCharFormat &format);     // 选中的文字格式发生了变化
    void cursorPositionChangedEvent( );         // 光标位置发生改变
    void setFont(const QFont &font);            // 设置格式


private:
    QString content;        // 文字内容
    QTextCharFormat* currentTextCharFormat;     // 当前 QTextCharFormat样式
    void init();            // 初始化
    void initAcitons();     // 初始化事件

    // QActions
    QAction * actionBold;       // 加粗
    QAction * actionUnderline;  // 下划线
    QAction * actionItalic;     // 斜体
    QAction * actionFontSet;    // 设置字体
    QAction * actionParagraph;  // 设置段落

    QMenu * ContextMenu;        // 右键菜单



};

#endif // DOCTEXTBLOCK_H
