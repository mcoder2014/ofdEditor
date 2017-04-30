#ifndef DOCPAGE_H
#define DOCPAGE_H

#include "model_global.h"       // 导出lib使用

#include <QVector>

// 类声明
class DocLayer;
class CT_PageArea;


/**
 * @Author Chaoqun
 * @brief  用来表示文章中的某一页
 * @date   2017/04/30
 */
class MODELSHARED_EXPORT DocPage
{
public:
    DocPage();
    ~DocPage();

private:
    QVector<DocLayer *> layers;          // 一个文档具有很多层
    // 还应该有模板页
    CT_PageArea* area;                  // 页面大小描述

};

#endif // DOCPAGE_H
