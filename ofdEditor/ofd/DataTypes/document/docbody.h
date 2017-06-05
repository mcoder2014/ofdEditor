#ifndef DOCBODY_H
#define DOCBODY_H
#include "../basic_datatype.h"
#include "CT_DocInfo.h"
#include "../../ofd_parser.h"
#include "../../ofd_global.h"  // 生成库文件需要

class OFDSHARED_EXPORT DocBody {        //文档对象入口，在一个文件中可以有多个文档
public:
    CT_DocInfo *doc_info;               //文档元数据信息描述
    ST_Loc doc_root;                    //指向文档根节点
    ST_Loc signatures;                  //数字签名 to be implemented
public:
    friend class OFDParser;

    DocBody(CT_DocInfo * _doc_info = NULL, QString _doc_root = "", QString _signatures = "") {
        if (_doc_info)
            setDocInfo(_doc_info);
        if (!_doc_root.isNull())
            setDocRoot(doc_root);
        if (!_signatures.isNull())
            setSignatures(_signatures);
    }

    CT_DocInfo * getDocInfo() {
        return doc_info;
    }

    void setDocInfo(CT_DocInfo * _doc_info) {
        if (!_doc_info)
            throw InvalidValueException("Invalid Value in DocInfo in DocBody: null pointer");
        doc_info = _doc_info;
    }


    ST_Loc getDocRoot() {
        return doc_root;
    }

    void setDocRoot(QString _doc_root_abs) {
        ST_Loc p("DocRoot", _doc_root_abs);
        doc_root = p;
    }

    ST_Loc getSignatures() {
        return signatures;
    }

    void setSignatures(QString _signatures_abs) {
        ST_Loc p("Signatures", _signatures_abs);
        signatures = p;
    }

    ~DocBody() {
        delete doc_info;
    }
};

#endif // DOCBODY_H
