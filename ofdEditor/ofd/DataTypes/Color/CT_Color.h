#ifndef CT_COLOR_H
#define CT_COLOR_H

#include "../../ofd_global.h"  // 生成库文件需要

#include <string>
using std::string;
#include "../basic_datatype.h"
#include "CT_Pattern.h"
#include "CT_AxialShd.h"
#include "CT_RadialShd.h"

// 颜色结构

class OFDSHARED_EXPORT CT_Color
{
public:

    ST_Array Value;             // 颜色值， “通道1 ，通道2 ，……”  -可选
    int Index;                  // 调色板中颜色的编号    -可选
    ST_RefID    Color_Space;    // 指向所用的颜色空间
    int Alpha;                  // 颜色透明度，取值0~255，   默认值255

    CT_Pattern Pattern;         // 底纹填充，复杂颜色的一种。

    // 二选一
    CT_AxialShd AxialShd;       // 轴向渐变
    CT_RadialShd RadialShd;     // 径向渐变
    int Shd_Selected;           // 0表示选择轴向，1表示选择径向，默认值-1

    CT_Color();
};

#endif // CT_COLOR_H
