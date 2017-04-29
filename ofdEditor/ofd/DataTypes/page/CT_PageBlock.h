#ifndef CT_PAGEBLOCK_H
#define CT_PAGEBLOCK_H
#include "../basic_datatype.h"
#include "../text/CT_Text.h"

#include "../../ofd_global.h"  // 生成库文件需要

class OFDSHARED_EXPORT CT_PageBlock {
public:
    vector<CT_Text> text_objects;
};

#endif // CT_PAGEBLOCK_H
