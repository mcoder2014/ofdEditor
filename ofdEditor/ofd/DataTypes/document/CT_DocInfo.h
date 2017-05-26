#ifndef CT_DOCINFO_H
#define CT_DOCINFO_H
#include "../basic_datatype.h"
#include "../../ofd_parser.h"
#include "../../ofd_global.h"  // 生成库文件需要

class OFDSHARED_EXPORT CT_DocInfo {  //文档员数据信息
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
    friend class OFDParser;

    CT_DocInfo(QString _doc_id = "",
               QString _title = "",
               QString _author = "",
               QString _subject = "",
               QString _abstract = "",
               QString _creation_date = "",
               QString _mod_date = "",
               QString _doc_usage = "",
               QString _cover = "",
               //QString _keywords = "",
               QString _creator = "",
               QString _creator_version = "",
               QVector<QStringList> * _custom_datas = nullptr) {
        if (!_doc_id.isNull())
            setDocID(_doc_id);
        setTitle(_title);
        setAuthor(_author);
        setSubject(_subject);
        setAbstract(_abstract);
        setCreationDate(_creation_date);
        setModDate(_mod_date);
        setDocUsage(_doc_usage);
        setCover(_cover);
        setCreator(_creator);
        setCreatorVersion(_creator_version);
        if (_custom_datas)
            setCustomDatas(_custom_datas);
        else
            custom_datas = new QVector<QStringList>();
    }

    ~CT_DocInfo() {
        delete custom_datas;
    }

    QString getDocID() {
        return doc_id;
    }

    void setDocID(QString _doc_id) {
        if (_doc_id.isNull())
            throw InvalidValueException("Invalid Value in DocID in CT_DocInfo: null pointer");
        doc_id = _doc_id;
    }

    QString getCreationDate() {
        return creation_date;
    }

    void setCreationDate(QString _creation_date) {
        creation_date = _creation_date;
    }

    QString getModDate() {
        return mod_date;
    }

    void setModDate(QString _mod_date) {
        mod_date = _mod_date;
    }

    QString getTitle() {
        return title;
    }

    void setTitle(QString _title) {
        title = _title;
    }

    QString getAuthor() {
        return author;
    }

    void setAuthor(QString _author) {
        author = _author;
    }

    QString getSubject() {
        return subject;
    }

    void setSubject(QString _subject) {
        subject = _subject;
    }

    QString getAbstract() {
        return abstract;
    }

    void setAbstract(QString _abstract) {
        abstract = _abstract;
    }

    QString getCreator() {
        return creator;
    }

    void setCreator(QString _creator) {
        creator = _creator;
    }

    QString getCreatorVersion() {
        return creator_version;
    }

    void setCreatorVersion(QString _creator_version) {
        creator_version = _creator_version;
    }

    QString getDocUsage() {
        return doc_usage;
    }

    void setDocUsage(QString _doc_usage) {
        doc_usage = _doc_usage;
    }

    ST_Loc getCover() {
        return cover;
    }

    void setCover(QString _cover_abs) {
        ST_Loc i("Cover", _cover_abs);
        cover = i;
    }

    QVector<QStringList> * getCustomDatas() {
        return custom_datas;
    }

    void setCustomDatas(QVector<QStringList> * _custom_datas) {
        if (!_custom_datas)
            throw InvalidValueException("Invalid Value in CustomDatas in CT_DocInfo: null pointer");
        delete custom_datas;
        custom_datas = _custom_datas;
    }
};

#endif // CT_DOCINFO_H
