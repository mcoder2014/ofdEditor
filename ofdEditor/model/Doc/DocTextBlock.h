#ifndef DOCTEXTBLOCK_H
#define DOCTEXTBLOCK_H

#include "model_global.h"       // 导出lib使用
#include "Doc/DocBlock.h"
#include <QVector>
#include <QTextEdit>
#include <QGraphicsProxyWidget>

class DocParagraph;

class MODELSHARED_EXPORT DocTextBlock
        :public QTextEdit
{
    Q_OBJECT
public:
    DocTextBlock(QWidget *parent = Q_NULLPTR);
    ~DocTextBlock();

protected:

    void contextMenuEvent(QContextMenuEvent *event);    // 右键菜单重载

private:

};

#endif // DOCTEXTBLOCK_H
