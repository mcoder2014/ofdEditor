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
    void writePublicRes(Res * data);

    void writePageArea(CT_PageArea * cur_page_area, QXmlStreamWriter & writer);
    void writePageBlock(CT_PageBlock * cur_page_block, QXmlStreamWriter & writer, bool is_layer = false);
    void writeColor(CT_Color * cur_color, QXmlStreamWriter & writer);
    void writeTextObject(CT_Text * cur_text, QXmlStreamWriter & writer);
    void writePathObject(CT_Path * cur_path, QXmlStreamWriter & writer);
    void writeImageObject(CT_Image * cur_image, QXmlStreamWriter & writer);
    void writeGraphicUnitAttributes(CT_GraphicUnit * cur_graphic_unit, QXmlStreamWriter & writer);
    void writeGraphicUnitMemebers(CT_GraphicUnit * cur_graphic_unit, QXmlStreamWriter & writer);
    void writeBase(CT_Base * cur_base, QXmlStreamWriter & writer);
public:
    OFDWriter(OFD * _data, QString _path);
};

QXmlStreamAttributes getAttributes(OFD * data);  //返回OFD类型标签的属性，方便书写
QXmlStreamAttributes getAttributes(CT_Layer * cur_layer);
QXmlStreamAttributes getAttributes(CT_GraphicUnit * cur_graphic_unit);
QXmlStreamAttributes getAttributes(CT_Color * cur_color);
QXmlStreamAttributes getAttributes(TextCode * cur_textcode);
QXmlStreamAttributes getAttributes(CT_Path * cur_path);
#endif // OFD_WRITER_H
