#ifndef CT_PAGEAREA_H
#define CT_PAGEAREA_H

#include "../../ofd_global.h"   // 生成库文件时需要

#include"../basic_datatype.h"


class OFDSHARED_EXPORT CT_PageArea
{
    ST_Box physical_box;              // 页面物理区域，
        // 以其左上角为页面空间坐标系的原点。
    ST_Box application_box;           // （可选）显示区域，
        // 页面内容实际显示或打印输出的区域，
        // 位于页面物理区域内，包含页眉、页脚、版心等内容。
    ST_Box content_box;               // （可选）版心区域，
        // 即文档的正文区域，位于显示区域内。
        // 左上点的坐标决定了其在显示区域内的位置。
    ST_Box bleed_box;                 // （可选）出血区域，
        // 位于页面物理区域外。不出现时，默认值为页面物理区域。
public:
    friend class OFDParser;
    CT_PageArea(){}
    ST_Box getPhysicalBox() { return physical_box; }
    ST_Box getApplicationBox() { return application_box; }
    ST_Box getContentBox() { return content_box; }
    ST_Box getBleedBox() { return bleed_box; }
};

#endif // CT_PAGEAREA_H
