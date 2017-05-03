#ifndef CT_TEXT_H
#define CT_TEXT_H
#include "../basic_datatype.h"
#include "TextCode.h"
#include "../../ofd_parser.h"
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
    //CT_CGTransform to be implemented
    TextCode * text_code;
    CT_Text(){
        stroke = false;
        fill = true;
        hscale = 1.0;
        read_direction = 0;
        char_direction = 0;
        weight = 400;
        italic = false;
    }
public:
    friend class OFDParser;
    ~CT_Text() {
        delete fillcolor;
        delete textcode;
    }
    ST_RefID getFont() { return font; }
    double getSize() { return size; }
    bool getStroke() { return stroke; }
    bool getFill() { return fill; }
    double getHScale() { return hscale; }
    int getReadDirection() { return read_direction; }
    int getCharDirection() { return char_direction; }
    int getWeight() { return weight; }
    bool getItalic() { return italic; }
    TextCode * getTextCode() { return text_code; }


};
#endif // CT_TEXT_H
