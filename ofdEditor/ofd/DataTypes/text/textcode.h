#ifndef TEXTCODE_H
#define TEXTCODE_H
#include "../basic_datatype.h"
#include "../../ofd_global.h"  // 生成库文件需要

class OFDSHARED_EXPORT TextCode {    //文字内容，也就是一段字符编码串。需要进行XML转义
public:
    double x, y;    //首字形的绘制点的横纵坐标
    ST_Array delta_x, delta_y;  //字形绘制点横纵方向偏移
    QString text; //！！！！！文本内容

public:
    friend class OFDParser;
    TextCode() {}
    double getX() {
        return x;
    }

    void setX(double _x) {
        x = _x;
    }

    double getY() {
        return y;
    }

    void setY(double _y) {
        y = _y;
    }

    ST_Array getDeltaX() {
        return delta_x;
    }

    void setDeltaX(QString _delta_x) {
        ST_Array a("DeltaX", _delta_x);
        delta_x = a;
    }

    ST_Array getDeltaY() {
        return delta_y;
    }

    void setDeltaY(QString _delta_y) {
        ST_Array a("DeltaY", _delta_y);
        delta_y = a;
    }

    QString getText() {             //获取文本内容
        return text;
    }

    void setText(QString _text) {
        text = _text;
    }
};
#endif // TEXTCODE_H
