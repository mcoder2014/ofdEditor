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
    QVector<ST_Loc> * page_res_locations;
    QVector<CT_Layer *> *content;
    //Actions to be implemented
    Page() {
        content = new QVector<CT_Layer *>;
        page_res = new QVector<Res *>;
        page_res_locations = new QVector<ST_Loc>;
        area = nullptr;
    }
public:
    friend class OFDParser;
    ST_Loc getBaseLoc() { return base_loc; }
    QVector<CT_Layer *> * getContent() {
        return content;
    }
    CT_PageArea * getArea() {
        return area;
    }

    QVector<Res *> * getPageRes() {
        return page_res;
    }

    QVector<ST_Loc> * getPageResLocations() {
        return page_res_locations;
    }

    ~Page() {
        for (int i = 0; i < page_res->length(); i++)
            delete page_res->at(i);
        for (int i = 0; i < content->length(); i++)
            delete content->at(i);
        delete area;
        delete page_res;
        delete content;
    }
};

#endif // PAGE_H
