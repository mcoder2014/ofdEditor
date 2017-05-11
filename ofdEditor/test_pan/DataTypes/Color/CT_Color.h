#ifndef CT_COLOR_H
#define CT_COLOR_H

#include "../../ofd_global.h"  // 生成库文件需要
#include "../basic_datatype.h"

class CT_AxialShd;
class CT_Pattern;
class CT_RadialShd;

// 颜色结构

class OFDSHARED_EXPORT CT_Color
{
    ST_Array value;             // 颜色值， “通道1 ，通道2 ，……”  -可选
    int index;                  // 调色板中颜色的编号    -可选
    ST_RefID    color_space;    // 指向所用的颜色空间
    int alpha;                  // 颜色透明度，取值0~255，   默认值255

    CT_Pattern* pattern;         // 底纹填充，复杂颜色的一种。

    // 二选一
    CT_AxialShd* axial_shd;       // 轴向渐变
    CT_RadialShd* radial_shd;     // 径向渐变
    int shd_selected;           // 0表示选择轴向，1表示选择径向，默认值-1

    CT_Color();
public:
    friend class OFDParser;
    ST_Array getValue() {
        return value;
    }
    ST_RefID getColorSpace() {
        return color_space;
    }
};

#endif // CT_COLOR_H
