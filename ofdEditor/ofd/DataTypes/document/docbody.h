#ifndef DOCBODY_H
#define DOCBODY_H
#include "../basic_datatype.h"
#include "CT_DocInfo.h"
#include "../../ofd_parser.h"
#include "../../ofd_global.h"  // 生成库文件需要

class OFDSHARED_EXPORT DocBody {        //文档对象入口，在一个文件中可以有多个文档
    CT_DocInfo *doc_info;               //文档元数据信息描述
    ST_Loc doc_root;                    //指向文档根节点
    ST_Loc signatures;                  //数字签名 to be implemented
    DocBody() {
        doc_info = nullptr;
    }
public:
    friend class OFDParser;
    CT_DocInfo * getDocInfo() {
        return doc_info;
    }

    ST_Loc getDocRoot() {
        return doc_root;
    }

    ST_Loc getSignatures() {
        return signatures;
    }

    ~DocBody() {
        delete doc_info;
    }
};

#endif // DOCBODY_H
