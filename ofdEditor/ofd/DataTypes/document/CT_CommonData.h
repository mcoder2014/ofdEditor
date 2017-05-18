#ifndef CT_COMMONDATA_H
#define CT_COMMONDATA_H
#include "../basic_datatype.h"
#include "../page/CT_PageArea.h"
#include "../../ofd_global.h"  // 生成库文件需要

class OFDSHARED_EXPORT CT_CommonData {
    ST_ID max_unit_id;  //当前文档中所有对象使用的最大标识
    CT_PageArea *page_area;  //指定多个页面区域的大小和位置
    QVector<ST_Loc> *public_res;  //公共资源序列，每个资源指向OFD包内部的一个XML文件
    //vector<CT_TemplatePage> template_page; //模板页序列
    ST_RefID default_cs;    //缺省颜色空间
    CT_CommonData() {
        public_res = new QVector<ST_Loc>();
        page_area = nullptr;
    }
public:
    friend class OFDParser;
    ST_ID getMaxUnitID() { return max_unit_id; }
    CT_PageArea *getPageArea() { return page_area; }
    QVector<ST_Loc> *getPublicRes() { return public_res; }
    ST_RefID getDefaultCS() { return default_cs; }
    ~CT_CommonData() {
        delete public_res;
        delete page_area;
    }
};

#endif // CT_COMMONDATA_H
