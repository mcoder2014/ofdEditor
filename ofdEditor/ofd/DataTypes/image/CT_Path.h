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

public:
    friend class OFDParser;

    CT_Path(QString _abbreviated_data = "",
            bool _stroke = true,
            bool _fill = true,
            QString _rule = "NonZero") {
        if (!_abbreviated_data.isNull())
            setAbbreviatedData(_abbreviated_data);
        setFill(_fill);
        setStroke(_stroke);
        setRule(_rule);
    }

    bool getStroke() {
        return stroke;
    }

    void setStroke(bool _stroke) {
        stroke = _stroke;
    }

    bool getFill() {
        return fill;
    }

    void setFill(bool _fill) {
        fill = _fill;
    }

    QString getRule() {
        return rule;
    }

    void setRule(QString _rule) {
        QStringList possible_values;
        possible_values.append("NonZero");
        possible_values.append("Even-Odd");

        if (!possible_values.contains(_rule))
            throw InvalidValueException("CT_Path对象的Rule属性使用了非法值: " + _rule);
        rule = _rule;
    }

    QString getAbbreviatedData() {
        return abbreviated_data;
    }

    void setAbbreviatedData(QString _abbreviated_data) {
        if (_abbreviated_data.isNull())
            throw InvalidValueException("Invalid value in AbbreviatedData in CT_Path: null pointer");
        abbreviated_data = _abbreviated_data;
    }
};

#endif // CT_PATH_H
