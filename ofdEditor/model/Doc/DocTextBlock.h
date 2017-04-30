#ifndef DOCTEXTBLOCK_H
#define DOCTEXTBLOCK_H

#include "model_global.h"       // 导出lib使用
#include "Doc/DocBlock.h"
#include <QVector>

class DocParagraph;

class MODELSHARED_EXPORT DocTextBlock
        :public DocBlock
{
public:
    DocTextBlock();
    ~DocTextBlock();
private:
    QVector<DocParagraph *> paragraphs;      // 段落
};

#endif // DOCTEXTBLOCK_H
