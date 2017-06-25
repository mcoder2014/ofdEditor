#include "Doc_OFDConvertor.h"
#include "Doc/DocPassage.h"
#include "Doc/DocPage.h"
#include "Doc/DocBlock.h"
#include "Doc/DocTextBlock.h"
#include "Doc/DocLayer.h"

#include <QVector>
#include <QTextCursor>
#include <QTextCharFormat>
#include <QTextBlockFormat>
#include <QTextdocument>
#include <QTextBlock>

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
    this->passage = passage;         // 保存passage
    this->ofdFile = new OFD( );      // 建立一个新的OFD文件

    // 构建流程
    this->buildDocBody();           // 构建DocBody
    this->buildDocument();          // docment



}

/**
 * @Author Chaoqun
 * @brief  生成DocBody
 * @param  void
 * @return void
 * @date   2017/06/24
 */
void Doc_OFDConvertor::buildDocBody()
{
    qDebug() << "exec buildDocbody";
    QVector<DocBody* >  docbodys;       // 新建docbodys新建带有……的docbodys
    DocBody* docbody = new DocBody(this->passage->getDocInfo());    // 加入docbody
    docbodys.append(docbody);           // 新建DocBody
    docbody->setDocRoot(QString("Doc_0/Document.xml"));             // 设置文档根目录

    this->ofdFile->setDocBodies( &docbodys);  // 设置ofd的docbobys

}

/**
 * @Author Chaoqun
 * @brief  创建document相关的东西
 * @param  void
 * @return void
 * @date   2017/06/24
 */
void Doc_OFDConvertor::buildDocument()
{
    qDebug() << "exec buildDocument";
    QVector<Document*> documents;               // 新建Documents
    Document* document = new Document();        // 新建Document
    documents.append(document);                 // 添加到文件中

    // 设置ID_Table
    this->table = document->getIDTable();       // 获得IDTable

    // 设置publicRes
    this->public_res = new Res();
    document->getPublicRes()->append(public_res);   // 将公用资源加入到document

    // 设置common_data
    CT_CommonData * commonData = new CT_CommonData();
    document->setCommonData(commonData);
        // 设置指向资源文件的
    commonData->getPublicRes()->append(ST_Loc(QString("PublicRes.xml"),
                                                  QString("PublicRes.xml"),
                                                  QString("PublicRes.xml")));

    this->buildPages(document);     // 解析每一页
}

/**
 * @Author Chaoqun
 * @brief  转换所有页面
 * @param  void
 * @return void
 * @date   2017/06/23
 */
void Doc_OFDConvertor::buildPages(Document* document)
{
    qDebug() << "exec buildPages";
    QVector<DocPage* > docPages = this->passage->getPages(); // 获得页面
    int pagesLength = docPages.size();      // 获得页面的数量

    if(pagesLength > 1)
    {
        // 设置公共页大小
        CT_CommonData* commonData = document->getCommonData();

        CT_PageArea *area = new CT_PageArea();

        DocPage* page = docPages[0];
        area->setPhysicalBox(0,0,
                             page->getWidth(),page->getHeight());
        commonData->setPageArea(area);

    }

    CT_Pages * ctpages = new CT_Pages();  // 新建CT_Pages
    document->setPages(ctpages);          // 设置
    QVector<Page*>* pages = ctpages->getPages();     // 获得Qvector

    // 遍历每个页面
    for(int i=0; i< pagesLength;i++)
    {
        DocPage* page = docPages.operator [](i);

        Page* ctPage = new Page();
        ctPage->setID(this->table->size()+1,this->table);   // 设置ID
        // 设置页的路径
        QString num;
        num.setNum(i);
        ctPage->setBaseLoc(QString("Pages/Page_") + num);
        pages->append(ctPage);               // 加入到OFD 文件中

        this->buildPage(ctPage,page);   // 单独处理每一页

    }

}

/**
 * @Author Chaoqun
 * @brief  处理文档中某一页
 * @param  Page *ctPage
 * @param  DocPage *docPage
 * @return void
 * @date   2017/06/24
 */
void Doc_OFDConvertor::buildPage(Page *ctPage, DocPage *docPage)
{
    qDebug() << "exec buildPage";
    QVector<CT_Layer *> *layers = ctPage->getContent();

    // 分成多个层，对每个层进行处理
    DocLayer* foreground = docPage->getForegroundLayer();   // 获得前景层
    CT_Layer* foreLayer = new CT_Layer();                   // 处理前景层
    foreLayer->setType("Foreground");                       // 设置类型
    foreLayer->setID(this->table->size()+1,this->table);    // 设置ID
    layers->append(foreLayer);                               // 加入到Page中
    this->buildLayer(foreLayer,foreground);                 // 处理该层的信息


    // 处理正文层
    DocLayer* body = docPage->getBodyLayer();
    CT_Layer* bodyLayer = new CT_Layer();
    bodyLayer->setType("Body");
    bodyLayer->setID(this->table->size()+1,this->table);
    layers->append(bodyLayer);
    this->buildLayer(bodyLayer,body);

    // 背景层
    DocLayer* background = docPage->getBackgroundLayer();
    CT_Layer* backLayer = new CT_Layer();
    backLayer->setType("Background");
    backLayer->setID(this->table->size()+1,this->table);
    layers->append(backLayer);
    this->buildLayer(backLayer,background);

}

/**
 * @Author Chaoqun
 * @brief  建造层
 * @param  Page* ctPage
 * @param  DocLayer* layer
 * @return 返回值
 * @date   2017/06/24
 */
void Doc_OFDConvertor::buildLayer(CT_Layer* ctLayer,DocLayer *layer)
{
    qDebug() << "Exec buildLayer";
    QVector<DocBlock* > *blocks = layer->getBlocks();
    int blocks_length = blocks->size();

    // 遍历所有块
    for(int i =0; i< blocks_length; i++)
    {
        DocBlock* block = blocks->operator [](i);       // 获得一个临时的框

        // 如果是文字框
        if(block->isTextBlock())
        {
            // 进入文字框处理模式
            this->buildText(ctLayer,block->getTextBlock());
        }
    }

}

/**
 * @Author Chaoqun
 * @brief  处理文字框
 * @param  CT_Layer* ctLayer
 * @param  DocTextBlock*textBlock
 * @return void
 * @date   2017/06/24
 */
void Doc_OFDConvertor::buildText(CT_Layer* ctLayer,DocTextBlock *textBlock)
{
    if(textBlock->getContentLength() == 0)
    {
        // 如果文本框中没有内容
        qDebug() << "The textBlock has no content in it.";
        return;
    }

    // 分析TextBlock
    textBlock->moveCursor(QTextCursor::Start);      // 移动到块的开始
    QTextCursor cursor = textBlock->textCursor();
    QTextDocument* document = cursor.document();
    int blockCount = document->blockCount();        // 获得Block的数量

    qDebug() << "This textBlock has " << blockCount << " QTextBlocks";

    for(int i =0; i<blockCount; i++)
    {
        // 遍历每一个QTextBlock
       QTextBlock block = cursor.block();
       cursor.movePosition(QTextCursor::NextBlock,
                           QTextCursor::MoveAnchor);    // 将鼠标移动到下一个块
       QTextCursor tempCursor = cursor;             // 复制一个临时光标处理
       int lineCount = block.lineCount();           // 计算该块有多少行

       qDebug() << "block " << i
                << " has " << lineCount
                << "lines";

//       for(int lineValue = 0; lineValue <lineCount; lineValue ++)
//       {
//           tempCursor.select(QTextCursor::LineUnderCursor); // 光标选中当前行
//           QString lineStr = tempCursor.selectedText();     // 当前选中的内容
//           qDebug() << "block " << i
//                    << " line " << lineValue
//                    << " content:" << lineStr;

//           tempCursor.movePosition(QTextCursor::Down);     // 移动到下一行
//       }

    }




}

/**
 * @Author Chaoqun
 * @brief  加入字体
 * @param  CT_Font *font
 * @return int 插入的Font 的RefId
 * @date   2017/06/24
 */
int Doc_OFDConvertor::addFont(CT_Font *font)
{
    int checkId = this->checkFont(font);    // 获得fontID

    if(checkId != -1)
    {
        // 如果检查后，存在了这个font
        return checkId;
    }

    // 为字体分配ID
    font->setID(this->table->size()+1, this->table);
    this->public_res->getFonts()->append(font);     // 加入
    qDebug() << "Add Font:" << font->getFontName();
    return font->getID().getID();
}

/**
 * @Author Chaoqun
 * @brief  为公用资源添加颜色空间
 * @param  CT_ColorSpace *colorSpace
 * @return int
 * @date   2017/06/24
 */
int Doc_OFDConvertor::addColorSpace(CT_ColorSpace *colorSpace)
{
    int checkId = this->checkColorSpace(colorSpace);    // 检查是否已有该颜色空间
    if(checkId != -1)
    {
        return checkId;
    }

    // 为颜色空间分派id
    colorSpace->setID(this->table->size()+1, this->table);
    this->public_res->getColorSpaces()->append(colorSpace);
    qDebug() << "Add ColorSpace: " << colorSpace->getType();
    return colorSpace->getID().getID();
}

/**
 * @Author Chaoqun
 * @brief  检查是否已经存在这个字体
 * @param  CT_Font* font
 * @return int
 * @date   2017/06/24
 */
int Doc_OFDConvertor::checkFont(CT_Font *font)
{
    QVector<CT_Font* >* fonts = this->public_res->getFonts();   // 获得字体
    int length = fonts->size();

    // 遍历一遍，检查是否有相同的
    for(int i= 0; i< length; i++)
    {
        if((*fonts)[i]->getFamilyName() == font->getFamilyName())
        {
            return (*fonts)[i]->getID().getID();        // 返回ID
        }
    }

    qDebug() << "this font is not stored";
    return -1;
}

/**
 * @Author Chaoqun
 * @brief  检查是否已经存在此ColorSpace
 * @param  CT_ColorSpace
 * @return int 如果存在，返回ID信息，不存在，则返回-1
 * @date   2017/06/24
 */
int Doc_OFDConvertor::checkColorSpace(CT_ColorSpace *colorSpace)
{
    QVector<CT_ColorSpace*>* colorSpaces
            = this->public_res->getColorSpaces();   // 获得颜色空间

    int length = colorSpaces->size();
    // 遍历colorSpace检查是否有相同的
    for(int i = 0; i < length; i++)
    {
        if((*colorSpaces)[i]->getType() == colorSpace->getType())
        {
            return (*colorSpaces)[i]->getID().getID();      // 返回ID
        }
    }

    return -1;  // 不存在已有的
}
