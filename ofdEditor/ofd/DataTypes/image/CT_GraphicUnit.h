#ifndef CT_GRAPHICUNIT_H
#define CT_GRAPHICUNIT_H

#include "../../ofd_global.h"   // 生成库文件时需要

#include <vector>
using std::vector;
#include "../basic_datatype.h"
#include <string>
using std::string;



class CT_Clip;
class CT_Color;
class CT_Action;

// 基本图元对象定义
class OFDSHARED_EXPORT CT_GraphicUnit
{

public:
    ST_Box Boundary;            // 被引用图元对象的外接矩形，
        // 采用当前空间坐标系。
    string Name;                // 图元对象的名字。
    bool Visible;               // 图元是否可见。
    ST_Array CTM;               // 对象坐标系下的变换矩阵。
    ST_RefID DrawParam;         // 绘制参数的引用
    double LineWidth;           // 线宽

//    string Cap;                 // 线帽样式。默认"Butt"
//        // 可选"Round"、"Round"、"Square"
    enum Cap_Def{Butt,Round,Square};
    Cap_Def Cap;

//    string Join;                // 结合点样式。默认"Miter"
//        // 可选"Miter"、"Round"、"Bevel"
    enum Join_Def{Miter,Round,Bevel};
    Join_Def Join;

    double MiterLimit;          // Join为Miter时JoinSize截断值。
        // 默认为3.528毫米（10  磅）
    double DashOffset;          // 虚线重复起始值。
    ST_Array DashPattern;       // 虚线重复样式。
    int Alpha;                  // 取值区间为[0,255] 表示对象的透明度，
    // 0表示全透明，255表示完全不透明。
    // 该属性不出现或超过取值范围时按照完全不透明的情况处理。

    CT_Color* FillColor;         // 填充颜色
    CT_Color* StrokeColor;       // 勾边颜色
    // CT_Actions Actions;       // 图元对象附带的动作序列。

    vector<CT_Clip*> Clips;      // 图元对象的裁剪区

    CT_GraphicUnit();
    ~CT_GraphicUnit();
};

#endif // CT_GRAPHICUNIT_H
