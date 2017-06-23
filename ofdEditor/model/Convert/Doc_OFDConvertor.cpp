#include "Doc_OFDConvertor.h"
#include "Doc/DocPassage.h"
#include "Doc/DocPage.h"
#include "Doc/DocBlock.h"
#include "Doc/DocTextBlock.h"
#include "Doc/DocLayer.h"

#include "DataTypes/basic_datatype.h"
#include "DataTypes/document/ofd.h"



Doc_OFDConvertor::Doc_OFDConvertor(QObject *parent)
    : QObject(parent)
{

}

/**
 * @Author Chaoqun
 * @brief  将Doc文件转换为ofd文件类型,启动程序
 * @param  DocPassage *passage
 * @return OFD*
 * @date   2017/06/22
 */
OFD *Doc_OFDConvertor::doc_to_ofd(DocPassage *passage)
{

}

void Doc_OFDConvertor::buildDocInfo()
{

}

void Doc_OFDConvertor::buildPage()
{

}

void Doc_OFDConvertor::buildLayer(DocLayer *layer)
{

}

void Doc_OFDConvertor::buildText(DocTextBlock *textBlock)
{

}
