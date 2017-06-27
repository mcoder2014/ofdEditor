#ifndef CT_TEXT_H
#define CT_TEXT_H

#include "../basic_datatype.h"
#include "./TextCode.h"
#include "../../ofd_parser.h"
#include "../image/CT_GraphicUnit.h"
#include "../Color/CT_Color.h"

#include "ofd_global.h"  // 生成库文件需要

class OFDSHARED_EXPORT CT_Text
        : public CT_GraphicUnit
{    //文字对象
public:
    //属性
    ST_RefID font;  //字体
    double size;    //字号
    bool size_used;

    bool stroke;
    bool fill;
    double hscale;
    int read_direction;
    int char_direction;
    int weight;     //文字对象的粗细值
    bool italic;    //是否是斜体
    //CT_CGTransform to be implemented
    QVector<TextCode *> * text_code;

    CT_Text()
    {
        stroke = false;
        fill = true;
        hscale = 1.0;
        read_direction = 0;
        char_direction = 0;
        weight = 400;
        italic = false;
        size_used = false;
        text_code = new QVector<TextCode *>;
    }

public:
    friend class OFDParser;
    ~CT_Text() {
        for (int i = 0; i < text_code->size(); i++)
            delete text_code->at(i);
        delete text_code;
    }
    ST_RefID getFont() {
        return font;
    }

    void setFont(int _font, ID_Table * _id_table) {
        if (_id_table->contains(_font)) {
            ST_RefID ri(_font);
            font = ri;
        } else
            throw InvalidIDException("CT_Text对象的Font属性引用了未注册的ID: " + QString::number(_font));
    }

    double getSize() {
        return size;
    }

    void setSize(double _size) {
        size = _size;
        size_used = true;
    }

    bool sizeUsed() {
        return size_used;
    }

    bool getStroke() {
        return stroke;
    }

    void setStroke(bool _stroke) {
        stroke = _stroke;
    }

    bool getFill() {
        return fill;
    }

    void setFill(bool _fill) {
        fill = _fill;
    }

    double getHScale() {
        return hscale;
    }

    void setHScale(double _hscale) {
        hscale = _hscale;
    }

    int getReadDirection() {
        return read_direction;
    }

    void setReadDirection(int _read_direction) {
        read_direction = _read_direction;
    }

    int getCharDirection() {
        return char_direction;
    }

    void setCharDirection(int _char_direction) {
        char_direction = _char_direction;
    }

    int getWeight() {
        return weight;
    }

    void setWeight(int _weight) {
        if (_weight == 1000)
            weight = 900;
        else if (_weight % 100 != 0 || _weight > 1000 || _weight <= 0)
            weight = 400;
    }

    bool getItalic() {
        return italic;
    }

    void setItalic(bool _italic) {
        italic = _italic;
    }

    QVector<TextCode *> * getTextCode() {
        return text_code;
    }

    void setTextCode(QVector<TextCode *> * _text_code) {
        if (_text_code) {
            for (int i = 0; i < text_code->size(); i++)
                delete text_code->at(i);
            delete text_code;
            text_code = _text_code;
        }
        else
            throw InvalidValueException("Invalid value in TextCode in Page: null pointer");
    }
};

#endif // CT_TEXT_H
