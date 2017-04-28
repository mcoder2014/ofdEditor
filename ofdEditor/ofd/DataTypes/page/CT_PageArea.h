#ifndef CT_PAGEAREA_H
#define CT_PAGEAREA_H

#include "../../ofd_global.h"   // 生成库文件时需要

#include"../basic_datatype.h"

/**
 * @Author Chaoqun
 * @brief  页面设置定义类型。
 * @date   2017/04/28
 */
class OFDSHARED_EXPORT CT_PageArea
{
public:

    ST_Box PhysicalBox;              // 页面物理区域，
        // 以其左上角为页面空间坐标系的原点。
    ST_Box ApplicationBox;           // 显示区域，
        // 页面内容实际显示或打印输出的区域，
        // 位于页面物理区域内，包含页眉、页脚、版心等内容。
    ST_Box ContentBox;               // （可选）版心区域，
        // 即文档的正文区域，位于显示区域内。
        // 左上点的坐标决定了其在显示区域内的位置。
    ST_Box BleedBox;                 // （可选）出血区域，
        // 位于页面物理区域外。不出现时，默认值为页面物理区域。

    CT_PageArea();
};

#endif // CT_PAGEAREA_H
