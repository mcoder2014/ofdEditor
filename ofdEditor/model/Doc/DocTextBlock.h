#ifndef DOCTEXTBLOCK_H
#define DOCTEXTBLOCK_H

#include "model_global.h"       // 导出lib使用
#include "Doc/DocBlock.h"
#include <QVector>
#include <QTextEdit>
#include <QGraphicsProxyWidget>

class DocParagraph;

class MODELSHARED_EXPORT DocTextBlock
        :public DocBlock
{
    Q_OBJECT
public:
    DocTextBlock(QWidget *parent = Q_NULLPTR);
    ~DocTextBlock();


private:

};

#endif // DOCTEXTBLOCK_H
