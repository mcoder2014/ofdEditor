#ifndef CT_PAGEBLOCK_H
#define CT_PAGEBLOCK_H
#include "../basic_datatype.h"
#include "../text/CT_Text.h"
#include "../image/CT_Image.h"
#include "../image/CT_Path.h"
#include "../../ofd_parser.h"
#include "../../ofd_global.h"  // 生成库文件需要

class OFDSHARED_EXPORT CT_PageBlock : public CT_Base{
public:
    QVector<CT_Text *> * text_object;
    QVector<CT_Path *> * path_object;
    QVector<CT_Image *> * image_object;
    QVector<CT_PageBlock *> * pageblock;
public:
    friend class OFDParser;

    CT_PageBlock() {
        text_object = new QVector<CT_Text *>();
        path_object = new QVector<CT_Path *>();
        image_object = new QVector<CT_Image *>();
        pageblock = new QVector<CT_PageBlock *>();
    }

    QVector<CT_Text *> * getTextObject() {
        return text_object;
    }

    void setTextObject(QVector<CT_Text *> * _text_object) {
        if (_text_object) {
            for (int i = 0; i < text_object->size(); i++)
                delete text_object->at(i);
            delete text_object;
            text_object = _text_object;
        }
        else
            throw InvalidValueException("Invalid value in TextObject in CT_PageBlock: null pointer");
    }

    QVector<CT_Path *> * getPathObject() {
        return path_object;
    }

    void setPathObject(QVector<CT_Path *> * _path_object) {
        if (_path_object) {
            for (int i = 0; i < path_object->size(); i++)
                delete path_object->at(i);
            delete path_object;
            path_object = _path_object;
        }
        else
            throw InvalidValueException("Invalid value in PathObject in CT_PageBlock: null pointer");
    }

    QVector<CT_Image *> * getImageObject() {
        return image_object;
    }

    void setImageObject(QVector<CT_Image *> * _image_object) {
        if (_image_object) {
            for (int i = 0; i < image_object->size(); i++)
                delete image_object->at(i);
            delete image_object;
            image_object = _image_object;
        }
        else
            throw InvalidValueException("Invalid value in ImageObject in CT_PageBlock: null pointer");
    }

    QVector<CT_PageBlock *> * getPageBlock() {
        return pageblock;
    }

    void setPageBlock(QVector<CT_PageBlock *> * _pageblock) {
        if (_pageblock) {
            for (int i = 0; i < _pageblock->size(); i++)
                delete _pageblock->at(i);
            delete _pageblock;
            pageblock = _pageblock;
        }
        else
            throw InvalidValueException("Invalid value in PageBlock in CT_PageBlock: null pointer");
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
