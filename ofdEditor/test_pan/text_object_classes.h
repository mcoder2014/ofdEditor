#ifndef DATACLASSES_H
#define DATACLASSES_H
#include "basic_datatype.h"
#include <string>
using std::string;
//*****************************************
class CT_Object { //OFD文档中所有部件的基类
    ST_ID id;
};


class CT_GraphicUnit : CT_Object{ //图元对象
    //属性
    ST_Box boundary;
};

class CT_Base { //？？？？？？？？？？？？？

};

//******************************************
//class CT_Font : public CT_Base {    //字体声明类型
//    //属性
//    string font_name;   //字体名
//    string family_name; //字体族名
//    string charset;     //字符集
//    bool italic;        //是否斜体
//    bool bold;          //是否粗体
//};

//class CT_CGTransform {  //字形变换类型
//    //属性
//    int CodePosition;   //TextCode中字符编码的起始位置，从0开始
//    int CodeCount;  //变换关系中字符的数量，该数值必须大于等于1.
//    int GlyphCount; //变换关系中字形索引的个数，该数值必须大于等于1，否则属于错误描述
//};

//class TextCode {    //文字内容，也就是一段字符编码串。需要进行XML转义
//    //属性
//    double x, y;    //首字形的绘制点的横纵坐标
//    ST_Array delta_x, delta_y;  //字形绘制点横纵方向偏移
//    //内容
//    string content; //！！！！！文本内容
//};

//class CT_Text : CT_GraphicUnit {    //文字对象
//    //属性
//    ST_RefID font;  //字体
//    double size;    //字号
//    bool stroke;
//    bool fill;
//    double hscale;
//    int read_direction;
//    int char_direction;
//    int weight;     //文字对象的粗细值
//    bool italic;    //是否是斜体
//    //组件
//    CT_Color fillcolor;
//    TextCode textcode;



//};

#endif // PAGEDATACLASSES_H
