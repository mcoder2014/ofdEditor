#ifndef CT_IMAGE_H
#define CT_IMAGE_H

#include "../../ofd_global.h"   // 生成库文件时需要
#include "../basic_datatype.h"
#include "CT_GraphicUnit.h"
#include "../../ofd_parser.h"


class OFDSHARED_EXPORT CT_Image : public CT_GraphicUnit
{
    ST_RefID resource_id;                // 指向资源文件中定义的多媒体对象的ID
    ST_RefID substitution;              // (可选)指定可替换图像，
        // 用于某些情况如高分辨率输出时进行图像替换。

    CT_Image() {}
public:
    friend class OFDParser;
    ST_RefID getResourceID() {
        return resource_id;
    }
    ST_RefID getSubstitution() {
        return substitution;
    }
};

#endif // CT_IMAGE_H
