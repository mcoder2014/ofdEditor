#ifndef OFD_PARSER_H
#define OFD_PARSER_H
#include <QXmlStreamReader>
#include <QFile>
#include "DataTypes/basic_datatype.h"
#include "DataTypes/document/OFD.h"
#include "DataTypes/document/DocBody.h"
#include "DataTypes/document/CT_DocInfo.h"
#include "DataTypes/document/Document.h"
#include "DataTypes/page/Page.h"
#include "DataTypes/basic_datatype.h"
#include "document_parser.h"

class OFDParser {  //解析OFD.xml
    OFD * ofd_data;     //保存了解析出来的数据信息
    ST_Loc current_path;        //当前文档的路径
    QXmlStreamReader reader;
    void openFile();          //初始化
    void readOFD();             //读取OFD文档信息
public:
    OFDParser(QString _path); //!!!!!!!!!!!!!!!应设为private
    OFD * getData() {
        return ofd_data;
    }
};



class res_parser {  //解析PublicRes.xml以及Res.xml
    Page * page_data;
    ST_Loc current_path;
public:
    res_parser();
};

class content_parser {      //解析content.xml
    ST_Loc current_path;
public:
    content_parser();
};

#endif // OFD_PARSER_H
