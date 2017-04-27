#ifndef CT_TEXT_H
#define CT_TEXT_H
#include "../basic_datatype.h"
#include "textcode.h"

class CT_Text : CT_GraphicUnit {    //文字对象
    //属性
    ST_RefID font;  //字体
    double size;    //字号
    bool stroke;
    bool fill;
    double hscale;
    int read_direction;
    int char_direction;
    int weight;     //文字对象的粗细值
    bool italic;    //是否是斜体
    //组件
    CT_Color fillcolor;
    TextCode textcode;

};
#endif // CT_TEXT_H
