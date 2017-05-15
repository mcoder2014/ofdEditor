#ifndef CT_COLORSPACE_H
#define CT_COLORSPACE_H

#include "../../ofd_global.h"  // 生成库文件需要
#include "../basic_datatype.h"

//class CT_ColorSpace_Palette;

// 颜色空间描述


class OFDSHARED_EXPORT CT_ColorSpace : public CT_Base
{
public :
    friend class OFDParser;
    QString getType() {
        return type;
    }
    int getBitsPerComponent() {
        return bits_per_component;
    }
    ST_Loc getProfile() {
        return profile;
    }

    //Other methods to be implemented

private:
    //属性
    QString type;
    int bits_per_component;   // 颜色通道数，默认为 8
    ST_Loc profile;         // 指向包内的文件描述文件

    // Palette // 调色版描述
    //vector<CT_ColorSpace_Palette> Palette;      // 调色板描述

    CT_ColorSpace() {
        bits_per_component = 8;
    }
};

//class OFDSHARED_EXPORT CT_ColorSpace_Palette{
//    ST_Array CV;            // 调色板中预定义颜色
//};




#endif // CT_COLORSPACE_H
