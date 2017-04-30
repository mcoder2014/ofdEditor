#ifndef DOCTEMPLATE_H
#define DOCTEMPLATE_H

#include "model_global.h"       // 导出lib使用

// 类声明
class DocLayer;
class CT_PageArea;

class MODELSHARED_EXPORT DocTemplate
{
public:
    DocTemplate();
    ~DocTemplate();
private:
    QVector<DocLayer *> layers;     // 模板的层

    CT_PageArea* area;              // 模板的页面大小


};

#endif // DOCTEMPLATE_H
