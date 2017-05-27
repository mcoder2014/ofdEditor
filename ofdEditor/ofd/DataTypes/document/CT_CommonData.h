#ifndef CT_COMMONDATA_H
#define CT_COMMONDATA_H
#include "../basic_datatype.h"
#include "../page/CT_PageArea.h"
#include "../../ofd_global.h"  // 生成库文件需要

class OFDSHARED_EXPORT CT_CommonData {
public:
    ST_ID max_unit_id;                      //当前文档中所有对象使用的最大标识
    CT_PageArea *page_area;                 //指定多个页面区域的大小和位置
    QVector<ST_Loc> *public_res;            //公共资源序列，每个资源指向OFD包内部的一个XML文件
    //vector<CT_TemplatePage> template_page; //模板页序列
    ST_RefID default_cs;                    //缺省颜色空间
public:
    friend class OFDParser;

    CT_CommonData() {
        public_res = new QVector<ST_Loc>();
        page_area = nullptr;
    }

    ST_ID getMaxUnitID() {
        return max_unit_id;
    }

    void setMaxUnitID(int _max_unit_id) {
        if (_max_unit_id < 0)
            throw InvalidValueException("Invalid Value in MaxUnitID in CT_CommonData: " + QString::number(_max_unit_id));
        ST_ID i(_max_unit_id);
        max_unit_id = i;
    }

    CT_PageArea *getPageArea() {
        return page_area;
    }

    void setPageArea(CT_PageArea * _page_area) {
        if (!_page_area)
            throw InvalidValueException("Invalid Value in PageArea in CT_CommonData: null pointer");
        delete page_area;
        page_area = _page_area;
    }

    QVector<ST_Loc> *getPublicRes() {
        return public_res;
    }

    void setPublicRes(QVector<ST_Loc> * _public_res) {
        if (!_public_res)
           throw InvalidValueException("Invalid Value in PublicRes in CT_CommonData: null pointer");
        delete public_res;
        public_res = _public_res;
    }

    ST_RefID getDefaultCS() {
        return default_cs;
    }

    void setDefaultCS(int _default_cs, ID_Table * id_table) {
        if (!id_table->contains(_default_cs))
            throw InvalidIDException("Unregistered ID in DefaultCS in CT_CommonData: " + QString::number(_default_cs));
        ST_RefID ri(_default_cs);
        default_cs = ri;
    }

    ~CT_CommonData() {
        delete public_res;
        delete page_area;
    }

    /**
 * @Author Chaoqun
 * @brief  CT_CommonData 深拷贝函数
 * @param  CT_CommonData * commonData
 * @return void
 * @date   2017/05/23
 */
    void copy(CT_CommonData * commonData)
    {
        // 预计5月30日前实现
    }
};

#endif // CT_COMMONDATA_H
