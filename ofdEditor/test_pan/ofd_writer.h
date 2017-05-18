#ifndef OFD_WRITER_H
#define OFD_WRITER_H
#include <QXmlStreamWriter>
#include <QFile>
#include <QDir>
#include "DataTypes/basic_datatype.h"
#include "DataTypes/document/OFD.h"
#include "DataTypes/document/DocBody.h"
#include "DataTypes/document/CT_DocInfo.h"
#include "DataTypes/document/Document.h"
#include "DataTypes/page/Page.h"
#include "DataTypes/basic_datatype.h"
#include "DataTypes/Color/CT_Color.h"

class OFDWriter
{
private:
    OFD * data;                 //OFD文档的数据
    ST_Loc current_path;        //当前文档节点的路径
    QFile * current_file;        //当前输出的文件
    QXmlStreamWriter writer;

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
#endif // OFD_WRITER_H
