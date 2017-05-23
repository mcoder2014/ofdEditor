#ifndef CT_PAGES_H
#define CT_PAGES_H
#include "../basic_datatype.h"
#include "Page.h"

#include "../../ofd_global.h"  // 生成库文件需要

class OFDSHARED_EXPORT CT_Pages {
public:
    QVector<Page *> * pages;
    CT_Pages() {
        pages = new QVector<Page*>();
    }
public:
    friend class OFDParser;
    QVector<Page*> *getPages() {
        return pages;
    }
    ~CT_Pages() {
        for (int i = 0; i < pages->size(); i++)
            delete pages->at(i);
        delete pages;
    }
};

#endif // CT_PAGES_H
