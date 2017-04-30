#ifndef DOCPAGE_H
#define DOCPAGE_H

#include "model_global.h"       // 导出lib使用
#include <vector>;
using std::vector;

// 类声明
class DocLayer;


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
    vector<DocLayer *> layers;          // 一个文档具有很多层

};

#endif // DOCPAGE_H
