#ifndef OFD_WRITER_H
#define OFD_WRITER_H
#include <QXmlStreamWriter>
#include <QFile>
#include <QDir>
#include "DataTypes/basic_datatype.h"
#include "ofdexceptions.h"

class OFD;
class DocBody;
class CT_DocInfo;
class Document;
class Page;
class CT_Color;
class CT_PageBlock;
class CT_Text;
class CT_Path;
class CT_Image;
class CT_Layer;
class CT_GraphicUnit;
class TextCode;
class CT_ColorSpace;
class CT_DrawParam;
class CT_Font;
class Res;
class CT_PageArea;

class OFDSHARED_EXPORT OFDWriter
{
private:
    OFD * data;                 //OFD文档的数据
    ST_Loc current_path;        //当前文档节点的路径
    QFile * current_file;        //当前输出的文件
    QXmlStreamWriter writer;
    void makePath(ST_Loc path);
    void createFile();
    void writeOFD();
    void writeDocument(Document * data);
    void writePage(Page * data);
    void writeRes(Res * data);

    void writePageArea(CT_PageArea * cur_page_area);
    void writePageBlock(CT_PageBlock * cur_page_block, bool is_layer = false);
    void writeColor(CT_Color * cur_color);
    void writeColorSpace(CT_ColorSpace * cur_colorspace);
    void writeFont(CT_Font * cur_font);
    void writeDrawParam(CT_DrawParam * cur_draw_param);
    void writeTextObject(CT_Text * cur_text);
    void writePathObject(CT_Path * cur_path);
    void writeImageObject(CT_Image * cur_image);
    void writeGraphicUnitAttributes(CT_GraphicUnit * cur_graphic_unit);
    void writeGraphicUnitMemebers(CT_GraphicUnit * cur_graphic_unit);
    void writeBase(CT_Base * cur_base);
public:
    OFDWriter(OFD * _data, QString _path);
};

QXmlStreamAttributes getAttributes(OFD * data);  //返回OFD类型标签的属性，方便书写
QXmlStreamAttributes getAttributes(CT_Layer * cur_layer);
QXmlStreamAttributes getAttributes(CT_GraphicUnit * cur_graphic_unit);
QXmlStreamAttributes getAttributes(CT_Color * cur_color);
QXmlStreamAttributes getAttributes(TextCode * cur_textcode);
QXmlStreamAttributes getAttributes(CT_Path * cur_path);
QXmlStreamAttributes getAttributes(CT_ColorSpace * cur_colorspace);
QXmlStreamAttributes getAttributes(CT_DrawParam * cur_draw_param);
QXmlStreamAttributes getAttributes(CT_Font * cur_font);
QXmlStreamAttributes getAttributes(CT_Text * cur_text);
#endif // OFD_WRITER_H
