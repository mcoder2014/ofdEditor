#ifndef PARAGRAPH_H
#define PARAGRAPH_H

#include "model_global.h"
#include <QVector>

class DocText;              // 文字小段

class DocParaStyle;         // 段落的样式

class MODELSHARED_EXPORT DocParagraph
{
public:
    DocParagraph();
private:
    DocParaStyle* paraStyle;    // 段落的样式
    QVector<DocText *> texts;   // 段落中的字体一致的几个文字

};

#endif // PARAGRAPH_H
