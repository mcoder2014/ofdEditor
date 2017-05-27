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

    void setType(QString _type) {
        QStringList possible_value;
        possible_value.append("Gray");
        possible_value.append("RGB");
        possible_value.append("CMYK");

        if (possible_value.contains(_type) || _type.isNull()) {
            type = _type;
        }
        else throw InvalidValueException("新建CT_ColorSpace对象时Type属性使用了无效的值： " + _type);
    }

    int getBitsPerComponent() {
        return bits_per_component;
    }

    void setBitsPerComponent(int _bits_per_component) {
        if (_bits_per_component <= 0)
            throw InvalidValueException("新建CT_ColorSpace对象时BitsPerComponent属性使用了无效的值： " + QString::number(_bits_per_component));
        bits_per_component = _bits_per_component;
    }

    ST_Loc getProfile() {
        return profile;
    }

    void setProfile(QString _profile) {
        ST_Loc p("Profile", _profile);
        profile = p;
    }

    //Other methods to be implemented

    CT_ColorSpace(int _id = 0,
                  ID_Table * id_table = nullptr,
                  QString _type = "RGB",
                  int _bits_per_component = 8,
                  QString _profile = "") {
        if (_id && id_table) {
            if (id_table->contains(_id))
                throw InvalidIDException("新建CT_ColorSpace对象时申请了已被使用的ID: " + QString::number(_id));
            ST_ID i(_id);
            CT_Base::setID(i, id_table);
        }
        setType(_type);
        setBitsPerComponent(_bits_per_component);
        if (!_profile.isNull()) {
            setProfile(_profile);
        }
    }

private:
    //属性
    QString type;
    int bits_per_component;   // 颜色通道数，默认为 8
    ST_Loc profile;         // 指向包内的文件描述文件

    // Palette // 调色版描述
    //vector<CT_ColorSpace_Palette> Palette;      // 调色板描述

};

//class OFDSHARED_EXPORT CT_ColorSpace_Palette{
//    ST_Array CV;            // 调色板中预定义颜色
//};




#endif // CT_COLORSPACE_H
