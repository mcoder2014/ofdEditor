#ifndef CT_COMMONDATA_H
#define CT_COMMONDATA_H

class CT_CommonData {
    ST_ID max_unit_id;  //当前文档中所有对象使用的最大标识
    CT_PageArea page_area;  //指定多个页面区域的大小和位置
    vector<ST_Loc> public_res;  //公共资源序列，每个资源指向OFD包内部的一个XML文件
    //vector<CT_TemplatePage> template_page; //模板页序列
    ST_RefID default_cs;    //缺省颜色空间
};

#endif // CT_COMMONDATA_H
