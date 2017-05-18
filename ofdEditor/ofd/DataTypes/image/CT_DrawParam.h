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

    CT_Color() {
        alpha = 255;
        index_used = false;
    }
public:
    friend class OFDParser;
    friend class CT_DrawParam;
    ST_Array getValue() {
        return value;
    }

    bool indexUsed() {
        return index_used;
    }

    int getIndex() {
        if (index_used)
            return index;
        else {
            //Error! Try to access unused data
            abort();
        }
    }

    ST_RefID getColorSpace() {
        return color_space;
    }

    int getAlpha() {
        if (alpha >= 0 && alpha < 256)
            return alpha;
        else {
            //Error!    Illegal value
            abort();
        }
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

    CT_DrawParam() {
        fill_color = nullptr;
        stroke_color = nullptr;
        line_width = 0.353;
        join = "Miter";
        cap = "Butt";
        dash_offset = 0;
        miter_limit = 3.528;
        fill_color = new CT_Color();
        stroke_color = new CT_Color();
        fill_color->alpha = 0;
        ST_Array c1("FillColor", "0 0 0");
        ST_Array c2("StrokeColor", "0 0 0");
        fill_color->value = c1;
        stroke_color->value = c2;
        fill_color_used = false;
        stroke_color_used = true;
    }

public:
    friend class OFDParser;
    ST_RefID getRelative() {
        return relative;
    }

    double getLineWidth() {
        return line_width;
    }

    QString getJoin() {
        return join;
    }

    QString getCap() {
        return cap;
    }

    double getDashOffset() {
        return dash_offset;
    }

    ST_Array getDashPattern() {
        return dash_pattern;
    }

    double getMiterLimit() {
        return miter_limit;
    }

    bool fillColorUsed() {
        return fill_color_used;
    }

    CT_Color * getFillColor() {
        return fill_color;
    }

    bool strokeColorUsed() {
        return stroke_color_used;
    }

    CT_Color * getStrokeColor() {
        return stroke_color;
    }

    ~CT_DrawParam() {
        delete fill_color;
        delete stroke_color;
    }
};

#endif // CT_DRAWPARAM_H
