#ifndef CT_COLOR_H
#define CT_COLOR_H

#include "../../ofd_global.h"  // 生成库文件需要
#include "../basic_datatype.h"
#include "../image/CT_DrawParam.h"

//class CT_AxialShd;
//class CT_Pattern;
//class CT_RadialShd;

// 颜色结构

//class OFDSHARED_EXPORT CT_Color {
//    ST_Array value;             // 颜色值， “通道1 ，通道2 ，……”  -可选
//    int index;                  // 调色板中颜色的编号    -可选
//    bool index_used;
//    ST_RefID    color_space;    // 指向所用的颜色空间
//    int alpha;                  // 颜色透明度，取值0~255，   默认值255

////    CT_Pattern* pattern;         // 底纹填充，复杂颜色的一种。

////    // 二选一
////    CT_AxialShd* axial_shd;       // 轴向渐变
////    CT_RadialShd* radial_shd;     // 径向渐变
//    int shd_selected;           // 0表示选择轴向，1表示选择径向，默认值-1

//    CT_Color() {
//        alpha = 255;
//        index_used = false;
//    }
//public:
//    friend class OFDParser;
//    friend class CT_DrawParam;
//    ST_Array getValue() {
//        return value;
//    }

//    bool indexUsed() {
//        return index_used;
//    }

//    int getIndex() {
//        if (index_used)
//            return index;
//        else {
//            //Error! Try to access unused data
//            abort();
//        }
//    }

//    ST_RefID getColorSpace() {
//        return color_space;
//    }

//    int getAlpha() {
//        if (alpha >= 0 && alpha < 256)
//            return alpha;
//        else {
//            //Error!    Illegal value
//            abort();
//        }
//    }
//};

#endif // CT_COLOR_H
