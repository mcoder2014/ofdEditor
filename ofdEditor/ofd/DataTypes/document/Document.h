#ifndef DOCUMENT_H
#define DOCUMENT_H

#include "../basic_datatype.h"
#include "CT_CommonData.h"
#include "../page/CT_Pages.h"
#include "CT_Outlines.h"
#include "Res.h"
#include "../../ofd_global.h"  // 生成库文件需要

class OFDSHARED_EXPORT Document {
public:
    CT_CommonData * common_data;    //文档公共数据，定义了默认页面单位、页面区域定义、公共资源等数据
    CT_Outlines * outlines;         //大纲树
    ST_Loc custom_tags;             //指向自定义标引文件
    ST_Loc extensions;              //指向拓展文件
    ST_Loc annotations;             //指向注释文件
    ID_Table * id_table;            //记录所有CT_Base的ID信息
    QVector<Res *> * public_res;
    QVector<Res *> * document_res;

    CT_Pages *pages;


public:
    friend class OFDParser;

    Document( CT_CommonData * _common_data = NULL,
              CT_Pages * _pages = NULL,
              CT_Outlines * _outlines = NULL,
              QString _custom_tags = "",
              QString _extensions = "",
              QString _annotations = "",
              QVector<Res *> * _public_res = NULL,
              ID_Table * _id_table = NULL
              ) {
        if (_common_data)
            setCommonData(_common_data);
        if (_pages)
            setPages(_pages);
        setOutlines(_outlines);
        if (!_custom_tags.isNull())
            setCustomTags(_custom_tags);
        if (!_extensions.isNull())
            setExtensions(_extensions);
        if (!_annotations.isNull())
            setAnnotations(_annotations);
        if (!_public_res)
            public_res = new QVector<Res *>;
        else public_res = _public_res;
        if (!_id_table)
            id_table = new ID_Table;
        else id_table = _id_table;

        document_res = new QVector<Res *>();
    }

    CT_CommonData * getCommonData() {
        return common_data;
    }

    void setCommonData(CT_CommonData * _common_data) {
        if (!_common_data)
            throw InvalidValueException("Invalid Value in CommonData in Document: null pointer");
        common_data = _common_data;
    }

    CT_Outlines * getOutlines() {
        return outlines;
    }

    void setOutlines(CT_Outlines * _outlines) {
        outlines = _outlines;
    }

    ST_Loc getCustomTags() {
        return custom_tags;
    }

    void setCustomTags(QString _custom_tags_abs) {
        ST_Loc p("CustomTags", _custom_tags_abs);
        custom_tags = p;
    }

    ST_Loc getAnnotations() {
        return annotations;
    }

    void setAnnotations(QString _annotations_abs) {
        ST_Loc p("Annotations", _annotations_abs);
        annotations = p;
    }

    ST_Loc getExtensions() {
        return extensions;
    }

    void setExtensions(QString _extensions_abs) {
        ST_Loc p("Extensions", _extensions_abs);
        extensions = p;
    }

    QVector<Res *> * getPublicRes() {
        return public_res;
    }

    void setPublicRes(QVector<Res *> * _public_res) {
        if (!_public_res)
            throw InvalidValueException("Invalid Value in PublicRes in Document: null pointer");
        for (int i = 0; i < public_res->size(); i++)
            delete public_res->at(i);
        delete public_res;
        public_res = _public_res;
    }

    CT_Pages * getPages() {
        return pages;
    }

    void setPages(CT_Pages * _pages) {
        if (!_pages)
            throw InvalidValueException("Invalid Value in Pages in Document: null pointer");
        pages = _pages;
    }

    ID_Table * getIDTable() {
        return id_table;
    }

    void setIDTable(ID_Table * _id_table) {
        if (!_id_table)
            throw InvalidValueException("新建Document对象ID_Table不可为空！！");
        delete id_table;
        id_table = _id_table;
    }

    ~Document() {
        for (int i = 0; i < public_res->size(); i++)
            delete public_res->at(i);
        delete common_data;
        delete outlines;
        delete public_res;
        delete pages;
    }
};

#endif // DOCUMENT_H
