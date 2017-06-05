#ifndef CT_PAGEBLOCK_H
#define CT_PAGEBLOCK_H

#include "../basic_datatype.h"

#include "../../ofd_global.h"  // 生成库文件需要
#include "ofd_parser.h"

#include <QVector>

class CT_Text;
class CT_Path;
class CT_Image;

class OFDSHARED_EXPORT CT_PageBlock
        :public CT_Base
{

public:
    QVector<CT_Text *> * text_object;
    QVector<CT_Path *> * path_object;
    QVector<CT_Image *> * image_object;
    QVector<CT_PageBlock *> * pageblock;
public:
    friend class OFDParser;

    CT_PageBlock();
    QVector<CT_Text *> * getTextObject();
    void setTextObject(QVector<CT_Text *> * _text_object);
    QVector<CT_Path *> * getPathObject();
    void setPathObject(QVector<CT_Path *> * _path_object);
    QVector<CT_Image *> * getImageObject();
    void setImageObject(QVector<CT_Image *> * _image_object);
    QVector<CT_PageBlock *> * getPageBlock();
    void setPageBlock(QVector<CT_PageBlock *> * _pageblock);
    ~CT_PageBlock();
};

#endif // CT_PAGEBLOCK_H
