#ifndef DOCUMENT_H
#define DOCUMENT_H
#include "../basic_datatype.h"
#include "ct_commondata.h"
#include "../page/ct_pages."

class Document {
    CT_CommonData common_data;  //文档公共数据，定义了默认页面单位、页面区域定义、公共资源等数据
    CT_Pages pages; //页树
    CT_Outlines outlines;   //大纲树
    ST_Loc custom_tags; //指向自定义标引文件
    ST_Loc extensions;  //指向拓展文件
    ST_Loc annotations; //指向注释文件
};

#endif // DOCUMENT_H
