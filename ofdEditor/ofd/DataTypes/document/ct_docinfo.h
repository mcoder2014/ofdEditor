#ifndef CT_DOCINFO_H
#define CT_DOCINFO_H
#include "../basic_datatype.h"
#include "../../ofd_parser.h"
#include "../../ofd_global.h"  // 生成库文件需要

class OFDSHARED_EXPORT CT_DocInfo {  //文档员数据信息

public:
    QString doc_id;         //由32位字符组成的文件标识
    QString creation_date;  //创建日期
    QString mod_date;       //修改日期
    QString title;          //标题
    QString author;         //作者
    QString subject;        //主题
    QString abstract;       //文档的注释和摘要
    QString creator;        //生成文档的软件
    QString creator_version;//生成文档的软件的版本
    QString doc_usage;      //文档的分类
    ST_Loc cover;           //文档的封面，此路径指向一个图片文件
    //ST_Array keywords;  //关键词集合
    QVector<QStringList> *custom_datas;  //每个CustomData元素为2元QStringList，其中[0]为"Name"属性，[1]为值

public:
    CT_DocInfo() {
        custom_datas = new QVector<QStringList>();
    }

public:
    friend class OFDParser;

    ~CT_DocInfo() {
        delete custom_datas;
    }

    QString getDocID() {
        return doc_id;
    }

    QString getCreationDate() {
        return creation_date;
    }

    QString getModDate() {
        return mod_date;
    }

    QString getTitle() {
        return title;
    }

    QString getAuthor() {
        return author;
    }

    QString getSubject() {
        return subject;
    }

    QString getAbstract() {
        return abstract;
    }

    QString getCreator() {
        return creator;
    }

    QString getCreatorVersion() {
        return creator_version;
    }

    QString getDocUsage() {
        return doc_usage;
    }

    ST_Loc getCover() {
        return cover;
    }

    QVector<QStringList> * getCustomDatas() {
        return custom_datas;
    }

    void copy(const CT_DocInfo & docInfo)
    {
        this->abstract = docInfo.abstract;
        this->author = docInfo.author;
        this->cover = docInfo.cover;
        this->creation_date = docInfo.creation_date;
        this->creator = docInfo.creator;
        this->creator_version = docInfo.creator_version;
        this->custom_datas = docInfo.custom_datas;
        this->doc_id = docInfo.doc_id;
        this->doc_usage = docInfo.doc_usage;
        this->mod_date = docInfo.mod_date;
        this->subject = docInfo.subject;
        this->title = docInfo.title;
    }

};

#endif // CT_DOCINFO_H
