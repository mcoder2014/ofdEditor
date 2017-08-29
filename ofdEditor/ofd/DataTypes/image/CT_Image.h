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
public:
    friend class OFDParser;

    CT_Image(int _resource_id = 0,
             ID_Table * _id_table = NULL,
             int _substitution = 0) {
        if (_resource_id && _id_table)
            setResourceID(_resource_id, _id_table);
        if (_substitution)
            setSubstitution(_substitution, _id_table);
    }

    ST_RefID getResourceID() {
        return resource_id;
    }

    void setResourceID(int _resource_id, ID_Table * _id_table) {
        if (!_id_table->contains(_resource_id))
            throw InvalidIDException("CT_Image对象的ResourceID属性引用了未注册的ID: " + QString::number(_resource_id));
        ST_RefID ri(_resource_id);
        resource_id = ri;
    }

    ST_RefID getSubstitution() {
        return substitution;
    }

    void setSubstitution(int _substitution, ID_Table * _id_table) {
        if (!_id_table->contains(_substitution))
            throw InvalidIDException("CT_Image对象的Substitution属性引用了未注册的ID: " + QString::number(_substitution));
        ST_RefID ri(_substitution);
        substitution = ri;
    }
};

#endif // CT_IMAGE_H
