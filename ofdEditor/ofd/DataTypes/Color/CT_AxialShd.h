#ifndef CT_AXIALSHD_H
#define CT_AXIALSHD_H

#include "../../ofd_global.h"  // 生成库文件需要

#include <string>
using std::string;
#include <vector>
using std::vector;
#include "../basic_datatype.h"


class CT_Color;

class CT_AxialShd_Color_Segment;

// 轴向渐变属性


class OFDSHARED_EXPORT CT_AxialShd
{
public:

    string MapType;             // 渐变绘制的方式，
        // 可选值 "Direct","Repeat","Reflect", 默认值为 "Direct"
    double MapUnit;             // 轴线一个渐变区间的长度，
        // 当MapType 的值不等于Direct 时出现，默认值为轴线长度
    int Extend;                 // 轴线延长线方向是否继续绘制渐变。
        // 可选值为0、1、2、3
                                // 0：不向两侧继续绘制渐变；
                                // 1：在终点至起点延长线方向绘制渐变；
                                // 2：在起点至终点延长线方向绘制渐变；
                                // 3：向两侧延长线方向绘制渐变。
                                // 默认值为0
    ST_Pos StartPoint;          // 轴线起始点        -必选
    ST_Pos EndPoint;            // 轴线的结束点      -必选
    // Segment                  // 颜色段，至少出现两个
    vector<CT_AxialShd_Color_Segment*> Segment;  // 颜色段，至少需要两个

    CT_AxialShd();
};

// 颜色段的表示
class OFDSHARED_EXPORT CT_AxialShd_Color_Segment{
public:
    double Position;            // 用于确定StartPoint 和 EndPoint
        // 中的各颜色的位置值，取值范围是[0, 1.0]，
        // 各段颜色的Position 值应根据颜色出现的顺序递增。
    CT_Color* Color;             // 该段的颜色，简单的颜色

    CT_AxialShd_Color_Segment();
    ~CT_AxialShd_Color_Segment();
};

#endif // CT_AXIALSHD_H
