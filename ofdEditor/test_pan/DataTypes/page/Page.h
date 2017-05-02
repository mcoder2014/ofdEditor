#ifndef PAGE_H
#define PAGE_H
#include "../basic_datatype.h"
#include "CT_Layer.h"
#include "CT_PageArea.h"
#include "../../ofd_global.h"  // 生成库文件需要

class OFDSHARED_EXPORT Page {
    ST_ID id;
    ST_Loc base_loc;
    QVector<CT_Layer *> *content;
    Page() {
        content = new QVector<CT_Layer *>;
    }
public:
    friend class OFDParser;
    ST_ID getID() { return id; }
};

#endif // PAGE_H
