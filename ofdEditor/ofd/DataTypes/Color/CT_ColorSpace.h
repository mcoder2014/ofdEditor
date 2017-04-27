#ifndef CT_COLORSPACE_H
#define CT_COLORSPACE_H

#include <string>
using std::string;
#include "../basic_datatype.h"

// 颜色空间描述


class CT_ColorSpace
{
public:

    ST_ID ID;               // 声明该颜色空间的标识，不能与已有标识重复
    string Type;            // 颜色空间类型，支持 "Gray", "RGB", "CMYK"
    int BitsPerComponent;   // 颜色通道数，默认为 8
    ST_Loc Profile;         // 指向包内的文件描述文件
    // Palette // 调色版描述
    ST_Array CV;            // 调色板中预定义颜色

    CT_ColorSpace();
};

#endif // CT_COLORSPACE_H
