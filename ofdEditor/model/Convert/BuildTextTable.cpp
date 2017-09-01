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

}

void BuildTextTable::saveAsImage()
{
    DocBlock *block = this->table_block->getBlock();
}
