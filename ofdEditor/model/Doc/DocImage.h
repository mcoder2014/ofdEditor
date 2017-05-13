#ifndef DOCIMAGE_H
#define DOCIMAGE_H

#include "model_global.h"
#include "Doc/DocBlock.h"

#include <QTextDocument>      // 文档

// 本类型用来表述文章中的图形、图片之类的

class MODELSHARED_EXPORT DocImage
        :public QTextDocument
{
    Q_OBJECT
public:
    DocImage(QObject *parent = Q_NULLPTR);
private:

};

#endif // DOCIMAGE_H

