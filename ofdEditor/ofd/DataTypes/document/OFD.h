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
public:
    friend class OFDParser;

    OFD(QString _root_path = "",
        QString _version = "",
        QString _doc_type = "",
        QVector<DocBody *> * _docbodys = nullptr,
        QVector<Document *> * _docs = nullptr
        ) {
        if (!_root_path.isNull())
            setRootPath(_root_path);
        if (!_version.isNull())
            setOfdVersion(_version);
        if (!_doc_type.isNull())
            setDocType(_doc_type);
        if (!_docbodys)
            docbodys = new QVector<DocBody *>();
        else
            docbodys = _docbodys;
        if (!_docs)
            docs = new QVector<Document *>();
        else
            docs = _docs;
    }

    //外部查询数据的接口
    QVector<DocBody *> * getDocBodies() {
        return docbodys;
    }

    void setDocBodies(QVector<DocBody *> * _docbodys) {
        if (!_docbodys)
            throw InvalidValueException("Invalid Value in DocBodys in OFD: null pointer");
        for (int i = 0; i < docbodys->size(); i++)
            delete docbodys->at(i);
        delete docbodys;
        docbodys = _docbodys;
    }

    ST_Loc getRootPath() {
        return root_path;
    }

    void setRootPath(QString _root_path_abs) {
        ST_Loc p("RootPath", _root_path_abs);
        root_path = p;
    }

    QVector<Document *> * getDocuments() {
        return docs;
    }

    void setDocument(QVector<Document *> * _docs) {
        if (!docs)
            throw InvalidValueException("Invalid Value in Documents in OFD: null pointer");
        for (int i = 0; i < docs->size(); i++)
            delete docs->at(i);
        delete docs;
        docs = _docs;
    }

    QString getOfdVersion() {
        return version;
    }

    void setOfdVersion(QString _version) {
        if (_version.isNull())
            throw InvalidValueException("Invalid Value in Version in OFD: null");
        version = _version;
    }

    QString getDocType() {
        return doc_type;
    }

    void setDocType(QString _doc_type) {
        if (_doc_type.isNull())
            throw InvalidValueException("Invalid Value in DocType in OFD: null");
        doc_type = _doc_type;
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
