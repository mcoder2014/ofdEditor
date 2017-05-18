#ifndef CT_PATH_H
#define CT_PATH_H

#include "../../ofd_global.h"   // 生成库文件时需要
#include "CT_GraphicUnit.h"
#include "../../ofd_parser.h"


//!!!!!!!!!
class OFDSHARED_EXPORT CT_Path : public CT_GraphicUnit
{
    //属性
    bool stroke;                    // 是否被勾边，默认值为true。
    bool fill;                      // 是否被填充，默认值为false。
    QString rule;                    // 描述Path的填充规则。
        // 可选值为NonZero和Even-Odd，默认值为NonZero。
    //成员
    QString abbreviated_data;         // 定义了一种路径元素描述序列，
        // 由一系列紧缩的操作符和操作数构成。使用对象相对坐标系。
    CT_Path() {
        stroke = true;
        fill = false;
        rule = "NonZero";
    }

public:
    friend class OFDParser;
    bool getStroke() {
        return stroke;
    }
    bool getFill() {
        return fill;
    }
    QString getRule() {
        return rule;
    }
    QString getAbbreviatedData() {
        return abbreviated_data;
    }
};

#endif // CT_PATH_H
