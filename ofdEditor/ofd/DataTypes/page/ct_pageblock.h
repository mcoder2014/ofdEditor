#ifndef CT_PAGEBLOCK_H
#define CT_PAGEBLOCK_H
#include "../basic_datatype.h"
#include "../text/CT_Text.h"
#include "../image/CT_Image.h"
#include "../image/CT_Path.h"
#include "../../ofd_parser.h"
#include "../../ofd_global.h"  // 生成库文件需要

class OFDSHARED_EXPORT CT_PageBlock : public CT_Base{
protected:
    QVector<CT_Text *> * text_object;
    QVector<CT_Path *> * path_object;
    QVector<CT_Image *> * image_object;
    QVector<CT_PageBlock *> * pageblock;
    CT_PageBlock() {
        text_object = new QVector<CT_Text *>();
        path_object = new QVector<CT_Path *>();
        image_object = new QVector<CT_Image *>();
        pageblock = new QVector<CT_PageBlock *>();
    }
public:
    friend class OFDParser;
    QVector<CT_Text *> * getTextObject() {
        return text_object;
    }
    QVector<CT_Path *> * getPathObject() {
        return path_object;
    }
    QVector<CT_Image *> * getImageObject() {
        return image_object;
    }
    QVector<CT_PageBlock *> * getPageBlock() {
        return pageblock;
    }
    ~CT_PageBlock() {
        for (int i = 0; i < text_object->length(); i++)
            delete text_object->at(i);
        for (int i = 0; i < path_object->length(); i++)
            delete path_object->at(i);
        for (int i = 0; i < image_object->length(); i++)
            delete image_object->at(i);
        for (int i = 0; i < pageblock->length(); i++)
            delete pageblock->at(i);
        delete text_object;
        delete path_object;
        delete image_object;
        delete pageblock;
    }
};

#endif // CT_PAGEBLOCK_H
