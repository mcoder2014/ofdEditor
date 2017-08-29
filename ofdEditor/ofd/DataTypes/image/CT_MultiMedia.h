#ifndef CT_MULTIMEDIA_H
#define CT_MULTIMEDIA_H

#include "ofd_global.h"
#include "DataTypes/basic_datatype.h"
#include <QString>


class OFDSHARED_EXPORT CT_MultiMedia
        :public CT_Base
{
public:
    friend class OFDParser;
    CT_MultiMedia();

    // 属性
    QString Type;       // 多媒体资源主类型
    QString Format;     // 多媒体资源格式
    QString MediaFile;  // 多媒体文件路径
};

#endif // CT_MULTIMEDIA_H
