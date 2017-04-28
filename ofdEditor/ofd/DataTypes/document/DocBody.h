#ifndef DOCBODY_H
#define DOCBODY_H
#include "../basic_datatype.h"
#include "CT_DocInfo.h"

#include "../../ofd_global.h"  // 生成库文件需要

class OFDSHARED_EXPORT DocBody { //文档对象入口，在一个文件中可以有多个文档
    CT_DocInfo doc_info;    //文档元数据信息描述
    ST_Loc doc_root;    //指向文档根节点
};

#endif // DOCBODY_H
