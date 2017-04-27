#ifndef PAGE_H
#define PAGE_H
#include "../basic_datatype.h"
#include "ct_layer.h"

class Page {
public:
    CT_PageArea area;   //该页页面区域大小
    ST_Loc page_res;    //指向该页使用的资源文件
    vector<CT_Layer> content;
    //++动作
};

#endif // PAGE_H
