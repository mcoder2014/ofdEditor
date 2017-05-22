#ifndef CT_FONT_H
#define CT_FONT_H
#include "../basic_datatype.h"

#include "../../ofd_global.h"  // 生成库文件需要


class OFDSHARED_EXPORT CT_Font : public CT_Base {    //字体声明类型
    //属性
    QString font_name;      //字体名
    QString family_name;    //字体族名
    QString charset;        //字符集
    bool italic;            //是否斜体
    bool bold;              //是否粗体
    bool serif;
    bool fixed_width;
    //FontFile
    CT_Font(){
        charset = "GB18030";
        italic = false;
        serif = false;
        bold = false;
        fixed_width = false;
    }
public:
    friend class OFDParser;
    QString getFontName() {
        return font_name;
    }
    QString getFamilyName() {
        return family_name;
    }
    QString getCharset() {
        return charset;
    }
    bool getItalic() {
        return italic;
    }
    bool getBold() {
        return bold;
    }
    bool getSerif() {
        return serif;
    }
    bool getFixedWidth() {
        return fixed_width;
    }
};

#endif // CT_FONT_H
