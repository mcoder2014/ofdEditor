#ifndef CT_LAYER_H
#define CT_LAYER_H
#include "../basic_datatype.h"
#include "CT_PageBlock.h"

#include "../../ofd_global.h"  // 生成库文件需要

class OFDSHARED_EXPORT CT_Layer : public CT_PageBlock {
public:
    friend class OFDParser;

    CT_Layer(QString _type = "Body",
             int _draw_param = 0,
             ID_Table * _id_table = NULL) {
        setType(_type);
        if (_draw_param && _id_table)
            setDrawParam(_draw_param, _id_table);
    }

    QString getType() {
        return type;
    }

    void setType(QString _type) {
        QStringList possible_values;
        possible_values.append("Body");
        possible_values.append("Foreground");
        possible_values.append("Background");

        if (possible_values.contains(_type))
            type = _type;
        else throw InvalidValueException("CT_Layer对象中的Type属性使用了非法值: " + _type);
    }

    ST_RefID getDrawParam() {
        return draw_param;
    }

    void setDrawParam(int _draw_param, ID_Table * _id_table) {
        if (!_id_table->contains(_draw_param))
            throw InvalidIDException("CT_Layer对象中的DrawParam属性引用了未注册的ID: " + QString::number(_draw_param));
        ST_RefID rd(_draw_param);
        draw_param = rd;
    }

    //从CT_PageBlock继承了几个接口
private:
    QString type;
    ST_RefID draw_param;
};

#endif // CT_LAYER_H
