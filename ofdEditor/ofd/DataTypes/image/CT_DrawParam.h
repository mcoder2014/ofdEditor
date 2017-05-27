#ifndef CT_DRAWPARAM_H
#define CT_DRAWPARAM_H

#include "../Color/CT_Color.h"
#include "../../ofd_global.h"   // 生成库文件时需要
#include "../basic_datatype.h"
#include "../../ofd_parser.h"

class OFDSHARED_EXPORT CT_Color {
    ST_Array value;             // 颜色值， “通道1 ，通道2 ，……”  -可选
    int index;                  // 调色板中颜色的编号    -可选
    bool index_used;
    ST_RefID    color_space;    // 指向所用的颜色空间
    int alpha;                  // 颜色透明度，取值0~255，   默认值255

//    CT_Pattern* pattern;         // 底纹填充，复杂颜色的一种。

//    // 二选一
//    CT_AxialShd* axial_shd;       // 轴向渐变
//    CT_RadialShd* radial_shd;     // 径向渐变
    int shd_selected;           // 0表示选择轴向，1表示选择径向，默认值-1

//    CT_Color() {
//        alpha = 255;
//        index_used = false;
//    }
public:
    friend class OFDParser;
    friend class CT_DrawParam;

    CT_Color(QString _value = "",
             int _index = 0,
             int _color_space = 0,
             ID_Table * _id_table = nullptr,
             int _alpha = 255) {
        if (!_value.isNull())
            setValue(_value);
        if (_index)
            setIndex(_index);
        else
            index_used = false;
        if (_color_space && _id_table)
            setColorSpace(_color_space, _id_table);
        setAlpha(_alpha);
    }

    ST_Array getValue() {
        return value;
    }

    void setValue(QString _value) {
        ST_Array a("Value", _value);
        value = a;
    }

    bool indexUsed() {
        return index_used;
    }

    int getIndex() {
        if (index_used)
            return index;
        else {
            throw IllegalAccessException("Try to access uninitialized value Index in CT_Color");
        }
    }

    void setIndex(int _index) {
        if (_index < 0)
            throw InvalidValueException("Try to assign a negative value to Index in CT_Color");
        index = _index;
        index_used = true;
    }

    ST_RefID getColorSpace() {
        return color_space;
    }

    void setColorSpace(int _color_space, ID_Table * id_table) {
        if (!id_table->contains(_color_space))
            throw InvalidIDException("CT_Color的ColorSpace属性引用了未被注册的ID: " + QString::number(_color_space));
        ST_RefID ri(_color_space);
        color_space = ri;
    }

    int getAlpha() {
        return alpha;
    }

    void setAlpha(int _alpha) {
        if (_alpha < 0 || _alpha > 255)
            throw InvalidValueException("CT_Color的Alpha属性使用了无效的属性值： " + QString::number(_alpha));
        alpha = _alpha;
    }
};
// 图元绘制参数属性

class OFDSHARED_EXPORT CT_DrawParam : public CT_Base {
    //属性
    ST_RefID relative;      // 指明当前绘制参数的基础绘制参数          - 可选
    QString join;            // 结合点，指定了两个线的端点结合时采用的样式。
        // 默认值为Miter  - 可选    ("Miter";"Round";"Bevel";)
    double line_width;       // 线宽当线宽为0 时，绘制一个像素的宽度。
        // 由于线宽0 的定义与设备相关，所以不推荐使用线宽0。
        // 默认值为0.353（相当于1 磅）    - 可选

    double dash_offset;      // 虚线重复样式开始的位置，默认值为0。
                            // 当DashPattern 不出现时，该参数无效  - 可选
    ST_Array dash_pattern;   //  虚线的重复样式。默认值为空。 - 可选
    QString cap;             // 线端点样式，枚举值，指定了一条线的端点样式。
        // 默认值为Butt       - 可选 ("Butt";"Round";"Square";)
    double miter_limit;      // Join 为Miter 时小角度JoinSize 的截断值，
        // 默认值为3.528。当Join 不等于Miter 时该参数无效 - 可选
    //成员
    CT_Color* fill_color;    // 填充颜色     -默认为透明值 - 可选
    bool fill_color_used;
    CT_Color* stroke_color;  // 描边颜色     -默认为黑色  - 可选
    bool stroke_color_used;

//    CT_DrawParam() {
//        line_width = 0.353;
//        join = "Miter";
//        cap = "Butt";
//        dash_offset = 0;
//        miter_limit = 3.528;
//        fill_color = new CT_Color();
//        stroke_color = new CT_Color();
//        fill_color->alpha = 0;
//        ST_Array c1("FillColor", "0 0 0");
//        ST_Array c2("StrokeColor", "0 0 0");
//        fill_color->value = c1;
//        stroke_color->value = c2;
//        fill_color_used = false;
//        stroke_color_used = true;
//    }

public:
    friend class OFDParser;

    CT_DrawParam(int _relative = 0,
                 ID_Table * _id_table = nullptr,
                 double _line_width = 0.353,
                 QString _join = "Miter",
                 QString _cap = "Butt",
                 double _miter_limit = 3.528,
                 double _dash_offset = 0,
                 QString _dash_pattern = "",
                 CT_Color * _fill_color = nullptr,
                 CT_Color * _stroke_color = nullptr
                 ) {
        if (_relative && _id_table)
            setRelative(_relative, _id_table);
        setLineWidth(_line_width);
        setJoin(_join);
        setCap(_cap);
        setMiterLimit(_miter_limit);
        setDashOffset(_dash_offset);
        setDashPattern(_dash_pattern);
        if (!_fill_color) {
            fill_color_used = false;
            fill_color = new CT_Color();
            ST_Array c1("FillColor", "0 0 0");
            fill_color->value = c1;
        } else {
            fill_color = _fill_color;
            fill_color_used = true;
        }
        if (!_stroke_color) {
            stroke_color_used = false;
            stroke_color = new CT_Color();
            ST_Array c1("StrokeColor", "0 0 0");
            stroke_color->value = c1;
        } else {
            stroke_color = _stroke_color;
            stroke_color_used = true;
        }
    }

    ST_RefID getRelative() {
        return relative;
    }

    void setRelative(int _relative, ID_Table * _id_table) {
        if (!_id_table->contains(_relative))
            throw InvalidIDException("CT_DrawParam对象的Relative属性引用了尚未注册的ID: " + QString::number(_relative));
        ST_RefID ri(_relative);
        relative = ri;
    }

    double getLineWidth() {
        return line_width;
    }

    void setLineWidth(double _line_width) {
        if (_line_width < 0)
            throw InvalidValueException("CT_DrawParam对象的LineWidth属性使用了非法的值: " + QString::number(_line_width));
        line_width = _line_width;
    }

    QString getJoin() {
        return join;
    }

    void setJoin(QString _join) {
        QStringList possible_values;
        possible_values.append("Miter");
        possible_values.append("Round");
        possible_values.append("Bevel");
        if (possible_values.contains(_join))
            join = _join;
        else
            throw InvalidValueException("CT_DrawParam对象的Join属性使用了非法的值: " + _join);
    }

    QString getCap() {
        return cap;
    }

    void setCap(QString _cap) {
        QStringList possible_values;
        possible_values.append("Butt");
        possible_values.append("Round");
        possible_values.append("Square");
        if (possible_values.contains(_cap))
            cap = _cap;
        else
            throw InvalidValueException("CT_DrawParam对象的Cap属性使用了非法的值: " + _cap);
    }

    double getDashOffset() {
        return dash_offset;
    }

    void setDashOffset(double _dash_offset) {
        if (dash_pattern.isNull())
            throw InvalidValueException("CT_DrawParam对象的DashOffset属性，在DashPattern未出现时设置");
        dash_offset = _dash_offset;
    }

    ST_Array getDashPattern() {
        return dash_pattern;
    }

    void setDashPattern(QString _dash_pattern) {
        if (!_dash_pattern.isNull()) {
            ST_Array a("DashPattern", _dash_pattern);
            dash_pattern = a;
        }
    }

    double getMiterLimit() {
        return miter_limit;
    }

    void setMiterLimit(double _miter_limit) {
        if (join != "Miter")
            throw InvalidValueException("Try to set MiterLimit in CT_DrawParam, when its Join is not Miter");
        miter_limit = _miter_limit;
    }

    bool fillColorUsed() {
        return fill_color_used;
    }

    CT_Color * getFillColor() {
        return fill_color;
    }

    void setFillColor(CT_Color * _fill_color) {
        if (_fill_color) {
            fill_color_used = true;
            delete fill_color;
            fill_color = _fill_color;
        }
    }

    bool strokeColorUsed() {
        return stroke_color_used;
    }

    CT_Color * getStrokeColor() {
        return stroke_color;
    }

    void setStrokeColor(CT_Color * _stroke_color) {
        if (_stroke_color) {
            stroke_color_used = true;
            delete stroke_color;
            stroke_color = _stroke_color;
        }
    }

    ~CT_DrawParam() {
        delete fill_color;
        delete stroke_color;
    }
};

#endif // CT_DRAWPARAM_H
