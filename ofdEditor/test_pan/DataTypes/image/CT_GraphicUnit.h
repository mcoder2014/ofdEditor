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
protected:
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
    CT_GraphicUnit() {}
public:
    friend class OFDParser;
    ST_Box getBoundary() {
        return boundary;
    }

    CT_Color * getFillColor() {
        return fill_color;
    }

    ~CT_GraphicUnit() {
        delete fill_color;
        delete stroke_color;
    }
};

#endif // CT_GRAPHICUNIT_H
