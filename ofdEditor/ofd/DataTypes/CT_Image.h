#ifndef CT_IMAGE_H
#define CT_IMAGE_H

#include "../ofd_global.h"   // 生成库文件时需要

#include <string>
using std::string;
#include "basic_datatype.h"
#include "CT_GraphicUnit.h"

/**
 * @Author Chaoqun
 * @brief  图像对象类型
 * @date   2017/04/28
 */
class OFDSHARED_EXPORT CT_Image:public CT_GraphicUnit
{
public:

    ST_RefID ResourceID;                // 指向资源文件中定义的多媒体对象的ID
    ST_RefID Substitution;              // (可选)指定可替换图像，用于某些情况如高分辨率输出时进行图像替换。

    CT_Image();
};

#endif // CT_IMAGE_H
