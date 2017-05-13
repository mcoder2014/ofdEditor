#ifndef OFD_H
#define OFD_H
#include "../basic_datatype.h"
#include "DocBody.h"
#include "../../ofd_global.h"  // 生成库文件需要
#include "../../ofd_parser.h"
#include "Document.h"


class OFDSHARED_EXPORT OFD { //主入口文件的根元素
    //成员
    ST_Loc root_path;    //ofd.xml的绝对路径，应由应用程序提供
    QString version; //版本号
    QString doc_type;    //文档子集类型

    QVector<DocBody *> * docbodys;

    QVector<Document *> * docs;

    //私有方法
    OFD() {
        docbodys = new QVector<DocBody *>();
        docs = new QVector<Document *>();
    }
public:
    friend class OFDParser;
    //外部查询数据的接口
    QVector<DocBody *> * getDocBodies() {
        return docbodys;
    }

    ST_Loc getRootPath() {
        return root_path;
    }

    QVector<Document *> * getDocuments() {
        return docs;
    }

    QString getOfdVersion() {
        return version;
    }

    QString getDocType() {
        return doc_type;
    }

    ~OFD() {
        for (int i = 0; i < docbodys->length(); i++)
            delete docbodys->at(i);
        for (int i = 0; i < docs->length(); i++)
            delete docs->at(i);
        delete docbodys;
        delete docs;
    }
};

#endif // OFD_H
