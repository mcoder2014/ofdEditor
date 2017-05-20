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
//    void textUnderline();
//    void textItalic();
//    void textFamily(const QString &f);
//    void textSize(const QString &p);
//    void textStyle(int styleIndex);
//    void textColor();
//    void textAlign(QAction *a);


protected:

    void contextMenuEvent(QContextMenuEvent *event);    // 右键菜单重载

private:
    QString content;        // 文字内容

    void mergeFormatOnWordOrSelection(
            const QTextCharFormat &format);     // 合并格式
    void currentCharFormatChanged(
            const QTextCharFormat &format);     // 选中的文字格式发生了变化




    void init();            // 初始化

};

#endif // DOCTEXTBLOCK_H
