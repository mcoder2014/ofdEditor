#ifndef PAGE_H
#define PAGE_H
#include "../basic_datatype.h"
#include "CT_Layer.h"
#include "CT_PageArea.h"
#include "../../ofd_global.h"  // 生成库文件需要
#include "../document/res.h"

class OFDSHARED_EXPORT Page : public CT_Base{
    ST_Loc base_loc;
    CT_PageArea * area;
    QVector<Res *> * page_res;
    QVector<CT_Layer *> *content;
    //Actions to be implemented
    Page() {
        content = new QVector<CT_Layer *>;
        page_res = new QVector<Res *>;
    }
public:
    friend class OFDParser;
    ST_Loc getBaseLoc() { return base_loc; }
    QVector<CT_Layer *> * getContent() {
        return content;
    }
    QVector<Res *> * getPageRes() {
        return page_res;
    }
    ~Page() {
        for (int i = 0; i < page_res->length(); i++)
            delete page_res[i];
        for (int i = 0; i < content->length(); i++)
            delete content[i];
        delete area;
        delete page_res;
        delete content;
    }
};

#endif // PAGE_H
