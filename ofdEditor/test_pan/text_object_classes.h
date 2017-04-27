#ifndef DATACLASSES_H
#define DATACLASSES_H
#include "basic_datatype.h"


//*****************************************
class CT_Object { //OFD文档中所有部件的基类
    ST_ID id;
};


class CT_GraphicUnit : CT_Object{ //图元对象
    //属性
    ST_Box boundary;
};

class CT_CGTransform {
    //属性
};

class TextCode {
    //属性
    double x, y;
    ST_Array delta_x, delta_y;
};

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

#endif // PAGEDATACLASSES_H
