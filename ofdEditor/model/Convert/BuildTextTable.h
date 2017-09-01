#ifndef BUILDTEXTTABLE_H
#define BUILDTEXTTABLE_H

#include <QObject>
#include "model_global.h"
#include <QTextBlock>
#include "Doc_OFDConvertor.h"
class DocPassage;       // 文章
class DocPage;          // 页
class DocLayer;         // 层
class DocBlock;         // 块
class DocTextBlock;     // 文本框

class OFD;              // OFD文件模型
class Res;              // 资源描述
class ID_Table;         // id table表
class CT_Font;          // ct_font
class CT_ColorSpace;    // 颜色空间
class Page;
class Document;
class CT_Layer;
class DocImageBlock;
class DocTable;


///
/// \brief The BuildTextTable class
///     暂行方式，保存成图片
///

class MODELSHARED_EXPORT BuildTextTable
{
public:
    BuildTextTable();

    DocTable *table_block;
    CT_Layer *layer;
    ID_Table *id_table;
    Doc_OFDConvertor *convertor;

    void buildTable(
            DocTable *table_block,
            CT_Layer *layer,
            ID_Table *id_table,
            Doc_OFDConvertor *convertor);
    void saveAsImage();

};

#endif // BUILDTEXTTABLE_H
