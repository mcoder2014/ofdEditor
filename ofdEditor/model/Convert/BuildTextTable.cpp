#include "BuildTextTable.h"

#include "Doc_OFDConvertor.h"
#include "Doc/DocPassage.h"
#include "Doc/DocPage.h"
#include "Doc/DocBlock.h"
#include "Doc/DocTextBlock.h"
#include "Doc/DocLayer.h"
#include "Doc/DocImageBlock.h"
#include "Doc/DocTable.h"
#include "Convert/BuildTextBlock.h"

#include <QVector>
#include <QTextCursor>
#include <QTextCharFormat>
#include <QTextBlockFormat>
#include <QTextdocument>
#include <QTextBlock>
#include <QFontMetricsF>
#include <QPixmap>
#include <QUuid>

#include "DataTypes/basic_datatype.h"
#include "DataTypes/document/ofd.h"
#include "DataTypes/document/Res.h"
#include "DataTypes/document/docbody.h"
#include "DataTypes/document/ct_docinfo.h"
#include "DataTypes/document/document.h"
#include "DataTypes/page/ct_pages.h"
#include "DataTypes/page/page.h"
#include "DataTypes/page/ct_layer.h"
#include "DataTypes/page/CT_PageArea.h"
#include "DataTypes/Color/CT_Color.h"
#include "DataTypes/Color/CT_ColorSpace.h"
#include "DataTypes/text/CT_Font.h"
#include "DataTypes/text/ct_text.h"
#include "DataTypes/text/textcode.h"
#include "DataTypes/basic_datatype.h"
#include "Tool/UnitTool.h"
#include "DataTypes/image/CT_Image.h"
#include "DataTypes/image/CT_MultiMedia.h"

BuildTextTable::BuildTextTable()
{

}

///
/// \brief BuildTextTable::buildTable
/// \param table_block
/// \param layer
/// \param id_table
/// \param convertor
///
void BuildTextTable::buildTable(
        DocTable *table_block, CT_Layer *layer, ID_Table *id_table, Doc_OFDConvertor *convertor)
{
    this->table_block =table_block;
    this->layer = layer;
    this->id_table = id_table;
    this->convertor = convertor;

    this->saveAsImage();
}

void BuildTextTable::saveAsImage()
{
    DocBlock *block = this->table_block->getBlock();

    QString fileName;                   // 将要保存的图片的文件名
    QUuid uuid = QUuid::createUuid();   // 创建uuid
    QString imageName = uuid.toString();    // 转换为字符串

    // 去掉字符串的链接符号  {0142d46f-60b5-47cf-8310-50008cc7cb3a}
    // 0142d46f60b547cf831050008cc7cb3a
    imageName.remove(imageName.length()-1, 1);
    imageName.remove(imageName.length() -13, 1);
    imageName.remove(imageName.length() -17,1);
    imageName.remove(imageName.length() -21, 1);
    imageName.remove(imageName.length() - 25,1);
    imageName.remove(0,1);

    fileName = imageName + ".jpg";

    CT_Image *ctimage = new CT_Image();
    CT_MultiMedia *multiMedia = new CT_MultiMedia();

    ctimage->setID(this->id_table->size() + 1 , this->id_table );
    multiMedia->setID(this->id_table->size() + 1, this->id_table );
    ctimage->setResourceID(multiMedia->getID().getID(), this->id_table);

    // 设置多媒体引用的标签
    multiMedia->Format = "";
    multiMedia->Type = "Image";
    multiMedia->MediaFile = fileName;

    QString filePath = convertor->passage->getTempSavePath()
            + "/" + fileName;

    // 设置为背景透明
//    this->table_block->viewport()->setAttribute(Qt::WA_TranslucentBackground, false);
//    QPixmap pixmap = QPixmap::grabWidget(this->table_block,0,0,block->size().width(),block->size().height());
//    pixmap.save(filePath,"JPEG",100);
//    this->table_block->viewport()->setAttribute(Qt::WA_TranslucentBackground, true);

    QPixmap pixmap = QPixmap::grabWidget(
                this->table_block->getPage(),
                block->x(),
                block->y(),
                block->size().width()+4 ,
                block->size().height()+4);

    pixmap.save(filePath,"JPEG",100);

    ctimage->setBoundary(
                UnitTool::pixelToMM(block->x()),
                UnitTool::pixelToMM(block->y()),
                UnitTool::pixelToMM(block->size().width()),
                UnitTool::pixelToMM(block->size().height()));

    convertor->document_res->getMultiMedia()->push_back(multiMedia);
    this->layer->image_object->push_back(ctimage);
}
