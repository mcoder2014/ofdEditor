#ifndef DOCUMENT_H
#define DOCUMENT_H

#include "../basic_datatype.h"
#include "CT_CommonData.h"
#include "../page/CT_Pages.h"
#include "ct_outlines.h"
#include "res.h"
#include "../../ofd_global.h"  // 生成库文件需要

class OFDSHARED_EXPORT Document {
    CT_CommonData * common_data;  //文档公共数据，定义了默认页面单位、页面区域定义、公共资源等数据
    CT_Outlines * outlines;   //大纲树
    ST_Loc custom_tags; //指向自定义标引文件
    ST_Loc extensions;  //指向拓展文件
    ST_Loc annotations; //指向注释文件

    QVector<Res *> * public_res;
    CT_Pages *pages;

    Document() {
        public_res = new QVector<Res *>();
        common_data = nullptr;
        outlines = nullptr;
        pages = nullptr;
    }

public:
    friend class OFDParser;
    CT_CommonData * getCommonData() { return common_data; }
    CT_Outlines * getOutLines() { return outlines; }
    ST_Loc getCustomTags() { return custom_tags; }
    ST_Loc getAnnotations() { return annotations; }
    QVector<Res *> * getPublicRes() { return public_res; }
    CT_Pages * getPages() { return pages; }
    ~Document() {
        for (int i = 0; i < public_res->length(); i++)
            delete public_res->at(i);
        delete common_data;
        delete outlines;
        delete public_res;
        delete pages;
    }
};

#endif // DOCUMENT_H
