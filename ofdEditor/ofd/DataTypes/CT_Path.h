#ifndef CT_PATH_H
#define CT_PATH_H

#include "../ofd_global.h"   // 生成库文件时需要

#include <string>
using std::string;
#include "CT_GraphicUnit.h"



/**
 * @Author Chaoqun
 * @brief  图形对象及其定义
 * @date   2017/04/28
 */
class OFDSHARED_EXPORT CT_Path : public CT_GraphicUnit
{
public:

    string AbbreviatedData;         // 定义了一种路径元素描述序列，由一系列紧缩的操作符和操作数构成。使用对象相对坐标系。
    bool Stroke;                    // 是否被勾边，默认值为true。
    bool Fill;                      // 是否被填充，默认值为false。
    string Rule;                    // 描述Path的填充规则。可选值为NonZero和Even-Odd，默认值为NonZero。

    CT_Path();
};

#endif // CT_PATH_H
