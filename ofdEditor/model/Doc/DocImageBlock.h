#ifndef DOCIMAGEBLOCK_H
#define DOCIMAGEBLOCK_H

#include "model_global.h"
#include "Doc/DocBlock.h"
#include <QLabel>           //存放图片的QWidget

// 本类型用来表述文章中的图形、图片之类的

class MODELSHARED_EXPORT DocImageBlock : public QLabel
{
    Q_OBJECT
public:
    DocImageBlock(QWidget *parent = NULL);
    void setImage(QPixmap & pixmap);
    void focusInEvent(QFocusEvent *ev);
    void focusOutEvent(QFocusEvent *ev);
private:

};

#endif // DOCIMAGEBLOCK_H

