#ifndef OFD_PARSER_H
#define OFD_PARSER_H
#include <QDomDocument>
#include <QFile>
#include "DataTypes/basic_datatype.h"
#include "DataTypes/document/OFD.h"
#include "DataTypes/document/DocBody.h"
#include "DataTypes/document/CT_DocInfo.h"
#include "DataTypes/document/Document.h"
#include "DataTypes/page/Page.h"
#include "DataTypes/basic_datatype.h"
#include "DataTypes/Color/CT_Color.h"
#include "ofd_global.h"

class OFD;

class OFDSHARED_EXPORT OFDParser {               //解析OFD.xml
    OFD * data;                 //保存了解析出来的数据信息
    ST_Loc current_path;        //当前文档的路径
    QDomDocument *document;     //保存了ofd文档信息的树形结构
    QString error_msg;          //当xml文档格式不当、解析错误时的错误信息
    int error_line;
    int error_column;

    void readPageArea(CT_PageArea * data,
                      QDomElement & root_node); //读取PageArea部分的子模块

    void readGraphicUnit(CT_GraphicUnit * data,
                         QDomElement & root_node); //读取某个图元对象的GraphicUnit部分

    void readColor(CT_Color * data,
                   QDomElement & root_node);   //读取某个CT_Color对象的信息

    void readFont(CT_Font * data,
                  QDomElement & root_node);     //读取某个CT_Font对象的信息

    void readColorSpace(CT_ColorSpace * data,
                        QDomElement & root_node);//读取某个CT_ColorSpace对象的信息

    void readDrawParam(CT_DrawParam * data,
                       QDomElement & root_node);

    void openFile();            //打开文件
    OFD *readOFD();             //读取OFD文档信息
    Document *readDocument();   //读取Document文档信息
    void readPage(Page *);      //读取Content页面信息
    void readResource(Res *);   //读取Res文档信息

public:
    OFDParser(QString _path); //!!!!!!!!!!!!!!!应设为private
    OFD * getData() {
        return data;
    }
};


#endif // OFD_PARSER_H
