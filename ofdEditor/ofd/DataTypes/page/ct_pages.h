#ifndef CT_PAGES_H
#define CT_PAGES_H
#include "../basic_datatype.h"
#include "Page.h"

#include "../../ofd_global.h"  // 生成库文件需要

class OFDSHARED_EXPORT CT_Pages {
public:
    QVector<Page *> * pages;

public:
    friend class OFDParser;
    CT_Pages() {
        pages = new QVector<Page*>();
    }

    QVector<Page*> *getPages() {
        return pages;
    }

    void setPages(QVector<Page *> * _pages) {
        if (_pages) {
            for (int i = 0; i < pages->size(); i++)
                delete pages->at(i);
            delete pages;
            pages = _pages;
        }
        else
            throw InvalidValueException("Invalid Value in Pages in CT_Pages: null pointer");
    }

    ~CT_Pages() {
        for (int i = 0; i < pages->size(); i++)
            delete pages->at(i);
        delete pages;
    }
};

#endif // CT_PAGES_H
