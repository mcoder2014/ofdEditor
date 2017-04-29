#ifndef CT_OUTLINES_H
#define CT_OUTLINES_H
#include "../basic_datatype.h"
#include "CT_OutlineElem.h"

#include "../../ofd_global.h"  // 生成库文件需要

class OFDSHARED_EXPORT CT_Outlines { //大纲树
    vector<CT_OutlineElem> outline_elem;
};

#endif // CT_OUTLINES_H
