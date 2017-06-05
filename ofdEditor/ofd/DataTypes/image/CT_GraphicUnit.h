#ifndef CT_GRAPHICUNIT_H
#define CT_GRAPHICUNIT_H

#include "../../ofd_global.h"   // 生成库文件时需要
#include "../basic_datatype.h"
#include "../../ofd_parser.h"
#include "../Color/CT_Color.h"

//class CT_Clip;
//class CT_Color;
//class CT_Action;

// 基本图元对象定义
class OFDSHARED_EXPORT CT_GraphicUnit : public CT_Base
{
public:
    ST_Box boundary;            // 被引用图元对象的外接矩形，采用当前空间坐标系。
    QString name;               // 图元对象的名字。
    bool visible;               // 图元是否可见。
    ST_Array ctm;               // 对象坐标系下的变换矩阵。
    ST_RefID draw_param;        // 绘制参数的引用
    double line_width;          // 线宽
    QString cap;                // 线帽样式。默认"Butt", 可选"Round"、"Round"、"Square"
    QString join;               //绘制路径时使用的结合点方式

    double miter_limit;          // Join为Miter时JoinSize截断值。
        // 默认为3.528毫米（10  磅）
    double dash_offset;          // 虚线重复起始值。
    ST_Array dash_pattern;       // 虚线重复样式。
    int alpha;                  // 取值区间为[0,255] 表示对象的透明度，
    // 0表示全透明，255表示完全不透明。
    // 该属性不出现或超过取值范围时按照完全不透明的情况处理。

    CT_Color* fill_color;         // 填充颜色
    CT_Color* stroke_color;       // 勾边颜色
    // CT_Actions Actions;       // 图元对象附带的动作序列。

//    QVector<CT_Clip*> * clips;      // 图元对象的裁剪区

public:
    friend class OFDParser;

    CT_GraphicUnit(double _start_x = 0,
                   double _start_y = 0,
                   double _delta_x = 0,
                   double _delta_y = 0,
                   QString _name = "",
                   bool _visible = true,
                   int _draw_param = 0,
                   ID_Table * _id_table = NULL,
                   double _line_width = 0.353,
                   QString _cap = "Butt",
                   QString _join = "Miter",
                   double _miter_limit = 3.528,
                   double _dash_offset = 0.0,
                   QString _dash_pattern = "",
                   int _alpha = 255,
                   CT_Color * _fill_color = NULL,
                   CT_Color * _stroke_color = NULL
                   ) {
        if (_start_x || _start_y || _delta_x || _delta_y)
            setBoundary(_start_x, _start_y, _delta_x, _delta_y);
        setName(_name);
        setVisible(_visible);
        if (_draw_param && _id_table)
            setDrawParam(_draw_param, _id_table);
        setLineWidth(_line_width);
        setCap(_cap);
        setJoin(_join);
        setMiterLimit(_miter_limit);
        dash_offset = _dash_offset;
        setDashPattern(_dash_pattern);
        setAlpha(_alpha);
        setFillColor(_fill_color);
        setStrokeColor(_stroke_color);
    }

    ST_Box getBoundary() {
        return boundary;
    }

    void setBoundary(double _start_x, double _start_y, double _delta_x, double _delta_y) {
        ST_Box b(_start_x, _start_y, _delta_x, _delta_y);
        boundary = b;
    }

    QString getName() {
        return name;
    }

    void setName(QString _name) {
        name = _name;
    }

    bool getVisible() {
        return visible;
    }

    void setVisible(bool _visible) {
        visible = _visible;
    }

    ST_RefID getDrawParam() {
        return draw_param;
    }

    void setDrawParam(int _draw_param, ID_Table * _id_table) {
        if (!_id_table->contains(_draw_param))
            throw InvalidIDException("CT_GraphicUnit对象中的DrawParam属性引用了未注册的ID: " + QString::number(_draw_param));
        ST_RefID rd(_draw_param);
        draw_param = rd;
    }

    double getLineWidth() {
        return line_width;
    }

    void setLineWidth(double _line_width) {
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
            throw InvalidValueException("CT_GraphicUnit对象的Join属性使用了非法的值: " + _join);
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
            throw InvalidValueException("CT_GraphicUnit对象的Cap属性使用了非法的值: " + _cap);
    }

    double getDashOffset() {
        return dash_offset;
    }

    void setDashOffset(double _dash_offset) {
        if (dash_pattern.isNull())
            throw InvalidValueException("CT_GraphicUnit对象的DashOffset属性，在DashPattern未出现时设置");
        dash_offset = _dash_offset;
    }

    ST_Array getDashPattern() {
        return dash_pattern;
    }

    void setDashPattern(QString _dash_pattern) {
        ST_Array a("DashPattern", _dash_pattern);
        dash_pattern = a;
    }

    double getMiterLimit() {
        return miter_limit;
    }

    void setMiterLimit(double _miter_limit) {
        if (join != "Miter")
            throw InvalidValueException("Try to set set MiterLimit in CT_GraphicUnit, when its Join is not Miter");
        miter_limit = _miter_limit;
    }

    int getAlpha() {
        return alpha;
    }

    void setAlpha(int _alpha) {
        if (_alpha < 0 || _alpha > 255)
            throw InvalidValueException("CT_GraphicUnit的Alpha属性使用了无效的属性值： " + QString::number(_alpha));
        alpha = _alpha;
    }

    CT_Color * getFillColor() {
        return fill_color;
    }

    void setFillColor(CT_Color * _fill_color) {
        if (_fill_color) {
            delete fill_color;
            fill_color = _fill_color;
        }
    }

    CT_Color * getStrokeColor() {
        return stroke_color;
    }

    void setStrokeColor(CT_Color * _stroke_color) {
        if (_stroke_color) {
            delete stroke_color;
            stroke_color = _stroke_color;
        }
    }

    ~CT_GraphicUnit() {
        delete fill_color;
        delete stroke_color;
    }
};

#endif // CT_GRAPHICUNIT_H
