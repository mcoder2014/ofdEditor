#ifndef CT_TEXT_H
#define CT_TEXT_H
#include "../basic_datatype.h"
#include "TextCode.h"

#include "../image/CT_GraphicUnit.h"
#include "../Color/CT_Color.h"

#include "../../ofd_global.h"  // 生成库文件需要

class OFDSHARED_EXPORT CT_Text : public CT_GraphicUnit
{    //文字对象
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
