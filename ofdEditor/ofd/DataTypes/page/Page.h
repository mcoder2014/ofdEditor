#ifndef PAGE_H
#define PAGE_H
#include "../basic_datatype.h"
#include "CT_Layer.h"
#include "CT_PageArea.h"
#include "../../ofd_global.h"  // 生成库文件需要
#include "../document/Res.h"

class OFDSHARED_EXPORT Page : public CT_Base{
public:
    ST_Loc base_loc;
    CT_PageArea * area;
    QVector<Res *> * page_res;
    QVector<ST_Loc> * page_res_locations;
    QVector<CT_Layer *> *content;
    //Actions to be implemented
public:
    friend class OFDParser;
    Page() {
        content = new QVector<CT_Layer *>;
        page_res = new QVector<Res *>;
        page_res_locations = new QVector<ST_Loc>;
        area = NULL;
    }

    ST_Loc getBaseLoc() {
        return base_loc;
    }

    void setBaseLoc(QString _base_loc_abs) {
        ST_Loc p("BaseLoc", _base_loc_abs, _base_loc_abs);
        base_loc = p;
    }

    QVector<CT_Layer *> * getContent() {
        return content;
    }

    void setContent(QVector<CT_Layer *> * _content) {
        if (_content) {
            for (int i = 0; i < content->size(); i++)
                delete content->at(i);
            delete content;
            content = _content;
        }
        else
            throw InvalidValueException("Invalid Value in Content in Page: null pointer");
    }

    CT_PageArea * getArea() {
        return area;
    }

    void setArea(CT_PageArea * _area) {
        delete area;
        area = _area;
    }

    QVector<Res *> * getPageRes() {
        return page_res;
    }

    void setPageRes(QVector<Res *> * _page_res) {
        if (_page_res) {
            for (int i = 0; i < page_res->size(); i++)
                delete page_res->at(i);
            delete page_res;
            page_res = _page_res;
        }
        else
            throw InvalidValueException("Invalid Value in PageRes in Page: null pointer");
    }

    QVector<ST_Loc> * getPageResLocations() {
        return page_res_locations;
    }

    void setPageResLocations(QVector<ST_Loc> * _page_res_locations) {
        if (_page_res_locations) {
            delete page_res_locations;
            page_res_locations = _page_res_locations;
        } else
            throw InvalidValueException("Invalid Value in PageResLocations in Page: null pointer");
    }

    ~Page() {
        for (int i = 0; i < page_res->size(); i++)
            delete page_res->at(i);
        for (int i = 0; i < content->size(); i++)
            delete content->at(i);
        delete area;
        delete page_res;
        delete content;
    }
};

#endif // PAGE_H
