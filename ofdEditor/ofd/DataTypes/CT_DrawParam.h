#ifndef CT_DRAWPARAM_H
#define CT_DRAWPARAM_H

#include "../ofd_global.h"   // 生成库文件时需要

#include "basic_datatype.h"

#include <string>
using std::string;

class CT_Color;

// 图元绘制参数属性

class OFDSHARED_EXPORT CT_DrawParam
{
public:
    ST_ID ID;               // 声明该绘制参数的标识，不能与已有标识重复 - 必选
    ST_RefID Relative;      // 指明当前绘制参数的基础绘制参数          - 可选
    string Join;            // 结合点，指定了两个线的端点结合时采用的样式。
        // 默认值为Miter  - 可选    ("Miter";"Round";"Bevel";)
    double LineWidth;       // 线宽当线宽为0 时，绘制一个像素的宽度。
        // 由于线宽0 的定义与设备相关，所以不推荐使用线宽0。
        // 默认值为0.353（相当于1 磅）    - 可选

    double DashOffset;      // 虚线重复样式开始的位置，默认值为0。
                            // 当DashPattern 不出现时，该参数无效  - 可选
    ST_Array DashPattern;   //  虚线的重复样式。默认值为空。 - 可选
    string Cap;             // 线端点样式，枚举值，指定了一条线的端点样式。
        // 默认值为Butt       - 可选 ("Butt";"Round";"Square";)
    double MiterLimit;      // Join 为Miter 时小角度JoinSize 的截断值，
        // 默认值为3.528。当Join 不等于Miter 时该参数无效 - 可选
    CT_Color* FillColor;    // 填充颜色     -默认为透明值 - 可选
    CT_Color* StrokeColor;  // 描边颜色     -默认为黑色  - 可选

    CT_DrawParam();
    ~CT_DrawParam();
};

#endif // CT_DRAWPARAM_H
