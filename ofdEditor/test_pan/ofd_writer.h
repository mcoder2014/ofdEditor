#ifndef OFD_WRITER_H
#define OFD_WRITER_H
#include <QXmlStreamWriter>
#include <QFile>
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
    void writePage();

public:
    OFDWriter(OFD * _data, QString _path);
};

static QXmlStreamAttributes getAttributes(OFD * data);  //返回OFD类型标签的属性，方便书写

#endif // OFD_WRITER_H
