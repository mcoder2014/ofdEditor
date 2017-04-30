#ifndef DOCPASSAGE_H
#define DOCPASSAGE_H

#include "model_global.h"       // 导出lib使用

#include <QVector>
#include <QString>

// 类声明
class DocPage;
class CT_DocInfo;
class CT_CommonData;

/**
 * @Author Chaoqun
 * @brief  表述一篇文章
 * @date   2017/04/30
 */
class MODELSHARED_EXPORT DocPassage
{
public:
    DocPassage();
    ~DocPassage();


private:
    QString version;             // OFD 版本默认 1.0
    QString docType;             // 类型默认是 OFD
    CT_DocInfo* docInfo;        // 文档元数据结构 ofd/CT_DocInfo
    //CT_CommonData* commonData;  // 文档公用文档数据
    QVector<DocPage *> pages;

};

#endif // DOCPASSAGE_H
