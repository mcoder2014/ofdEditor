#ifndef CT_RADIALSHD_H
#define CT_RADIALSHD_H

#include "../../ofd_global.h"  // 生成库文件需要

#include <string>
using std::string;
#include <vector>
using std::vector;
// 径向渐变类型
#include "../basic_datatype.h"
#include "CT_AxialShd.h"

class OFDSHARED_EXPORT CT_RadialShd
{
public:

    string MapType;             // 渐变绘制的方式，可选值 "Direct","Repeat","Reflect", 默认值为 "Direct"
    double MapUnit;             // 当MapType的值不为Direct时出现。表示中心点连线上一个渐变区间所绘制的长度，默认值为中心点连线长度。
    double Eccentricity;        // 两个椭圆的离心率，取值范围是[0, 1]。默认值为0
    double Angle;               // 倾斜角度，椭圆长轴与x轴正向的夹角，默认值为0，单位为角度。

    ST_Pos StartPoint;          // 起始椭圆的中心点坐标。
    double StartRadius;         // 起始椭圆的半长轴长度。
    ST_Pos EndPoint;            // 结束椭圆的中心点坐标。
    double EndRadius;           // 结束椭圆的半长轴长度。
    int Extend;                 // 当轴线至少有一侧位于Clip裁剪区域对象内部时，轴线延长线方向是否继续绘制渐变。可选值为0、1、2、3，默认值为0。
                                // 0：不向圆心联机两侧继续绘制渐变；
                                // 1：在终点圆至起点圆延长线方向绘制渐变；
                                // 2：在起点圆至终点圆延长线方向绘制渐变；
                                // 3：向两侧延长线方向绘制渐变。

    vector<CT_AxialShd_Color_Segment*> Segment;  // 颜色段，至少需要两个


    CT_RadialShd();
};

#endif // CT_RADIALSHD_H
