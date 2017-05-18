#ifndef DOCTEXT_H
#define DOCTEXT_H

#include "model_global.h"       // 导出lib使用
#include<QString>

class DocTextStyle;

class MODELSHARED_EXPORT DocText
{
public:
    DocText();
    DocTextStyle* style;            // 字体样式
    QString content;                // 内容
};

#endif // DOCTEXT_H
