#ifndef PAGE_H
#define PAGE_H
#include "../basic_datatype.h"
#include "CT_Layer.h"
#include "CT_PageArea.h"

#include "../../ofd_global.h"  // 生成库文件需要

class OFDSHARED_EXPORT Page {
public:
    CT_PageArea area;   //该页页面区域大小
    ST_Loc page_res;    //指向该页使用的资源文件
    vector<CT_Layer> content;
    //++动作
};

#endif // PAGE_H
