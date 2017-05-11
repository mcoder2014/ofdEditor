#ifndef CT_OUTLINEELEM_H
#define CT_OUTLINEELEM_H
#include "../basic_datatype.h"

#include "../../ofd_global.h"  // 生成库文件需要

class OFDSHARED_EXPORT CT_OutlineElem {
    string title;
    int count;
    bool expanded;
    //++动作
    QVector<CT_OutlineElem> outline_elem;
};

#endif // CT_OUTLINEELEM_H
