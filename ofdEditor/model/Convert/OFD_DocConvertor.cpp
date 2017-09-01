#include "OFD_DocConvertor.h"

#include <QTextBlock>
#include <QVector>
#include "Doc/DocPassage.h"
#include "Doc/DocPage.h"
#include "DataTypes/document/ofd.h"
#include "DataTypes/document/document.h"
#include "DataTypes/page/ct_pages.h"
#include "DataTypes/page/page.h"
#include "Doc/DocTextBlock.h"
#include "Doc/DocBlock.h"
#include "Doc/DocImageBlock.h"
#include "Tool/UnitTool.h"
#include "DataTypes/text/TextCode.h"
#include "DataTypes/text/CT_Text.h"
#include "DataTypes/image/CT_DrawParam.h"
#include "DataTypes/Color/CT_ColorSpace.h"
#include "Convert/Objects/MinTextUnit.h"
#include "DataTypes/page/CT_PageArea.h"
#include "DataTypes/document/ct_commondata.h"
#include "DataTypes/image/CT_Image.h"
#include "DataTypes/image/CT_MultiMedia.h"

OFD_DocConvertor::OFD_DocConvertor()
{

}

/**
 * @Author Chaoqun
 * @brief  将ofd格式转换成doc格式
 * @param  OFD * ofd
 * @return DocPassage *
 * @date   2017/05/23
 */
DocPassage *OFD_DocConvertor::ofd_to_doc(OFD *ofd)
{
    DocPassage * passage = NULL;
    this->ofdFile = ofd;

    try{

        passage = new DocPassage();
        passage->setVisible(false);
        // version
        passage->setVersion(ofd->getOfdVersion());
        passage->setDocType(ofd->getDocType());

        QVector<DocBody* > * bodys = ofd->getDocBodies();
        DocBody * docBody = (*bodys)[0];        // DocBody

        passage->setDocInfo(*(docBody->getDocInfo()));      // 设置文档元信息

        Document * document = (*(ofd->getDocuments()))[0];          // Document

        CT_CommonData * commonData = document->getCommonData();     // 获取common_data- >内含公用页大小
        this->public_pageArea = commonData->getPageArea();          // 从common_data中获取公用边大小

        //    CT_Pages pages = document->pages;               // 获得文档中的页
        QVector<Page * > * pages = document->getPages()->getPages(); // 获得页属性

        for(int i = 0; i <pages->size(); i++)
        {

            //  生成每一页
            qDebug() << "build Page" << i;
            DocPage * newPage = this->buildDocPage(passage, (*pages)[i]);
            newPage->setVisible(true);
        }
    }
    catch(...)
    {
        qDebug() << "ofd_to_doc:exception.";
    }

    passage->setVisible(true);
    return passage;
}

DocPage *OFD_DocConvertor::buildDocPage(DocPassage *passage, Page *ct_page)
{
    DocPage * page;
    try
    {
        if(ct_page->area != NULL)
        {
            // 如果定义了纸张尺寸

            CT_PageArea* area = ct_page->getArea();      // 获得空间
            if(area == NULL)
            {
                // 如果没有定义页面大小
                qDebug() <<"buildPage -> there is no pageArea in page";
                area = this->public_pageArea;       // 使用公用的页面空间
            }

            // 使用物理空间新建页面
            page = new DocPage(area->getPhysicalBox().getDeltaX(),
                               area->getPhysicalBox().getDeltaY(),
                               1.0);       // 设置纸张大小
        }
        else
        {
           page = new DocPage();
        }

        page->setVisible(false);        // 先隐藏显示
        passage->addPage(page);         // 加入到文章中来


        // 将每一层加入到页中
        QVector<CT_Layer *>* layers = ct_page->getContent();     // 获得文章中的层信息
        for(int i = 0; i < layers->size(); i++)
        {
            CT_Layer* layer = (*layers)[i];
            qDebug() << "excute insertLayer: " << i;
            this->insertLayer(page,layer);          // 将该层的内容加入到页面中
        }

    }
    catch(...)
    {
        qDebug() << "OFD_DocConvertor::buildDocPage exception.";
    }

    return page;
}

/**
 * @Author Chaoqun
 * @brief  处理CT_Layer
 * @param  DocPage *page
 * @param  CT_Layer *layer
 * @return void
 * @date   2017/05/23
 */
void OFD_DocConvertor::insertLayer(DocPage *page, CT_Layer *layer)
{
    try
    {

        this->insertPageBlock(page,layer,layer);        // 先将CT_Layer当作PageBlock处理

    }
    catch(...)
    {
        qDebug() << "OFD_DocConvertor::insertLayer has exception";
    }
}

/**
 * @Author Chaoqun
 * @brief  处理插入pageblock
 * @param  DocPage *page
 * @param  CT_Layer *layer
 * @param  CT_PageBlock*
 * @return 返回值
 * @date   2017/05/23
 */
void OFD_DocConvertor::insertPageBlock(DocPage *page,
                                       CT_Layer *layer,
                                       CT_PageBlock *pageBlock)
{

    QString cttype = layer->getType();        // 首先获得层

    DocPage::Layer doctype;

    // 获得关于层的概念
    if(cttype == "Foreground")
    {
        doctype = DocPage::Foreground;
    }
    else if(cttype == "Body")
    {
        doctype = DocPage::Body;
    }
    else if(cttype == "Background")
    {
        doctype = DocPage::Background;
    }


    // 处理 CT_Text
    QVector <CT_Text *> * texts = pageBlock->getTextObject();
    for(int i = 0; i<texts->size(); i++)
    {
        this->insertCT_Text(page,doctype,(*texts)[i]);  // 插入文字
    }

    // 处理 CT_Path

    // 处理 CT_Image
    QVector <CT_Image *> *images = pageBlock->getImageObject();
    for(int i = 0; i < images->size(); i++)
    {
        this->insertCT_Image(
                    page,
                    doctype,
                    images->operator [](i));
    }


}

/**
 * @Author Chaoqun
 * @brief  将文字块插入到页面中
 * @param  DocPage *page
 * @param  DocPage::Layer *layer
 * @param  CT_Text *text
 * @return 返回值
 * @date   2017/05/23
 */
void OFD_DocConvertor::insertCT_Text(DocPage *page, DocPage::Layer layer, CT_Text *text)
{
//    qDebug() << "execute insert CT_Text" << text->getID();
    CT_Color * fillColor = text->getFillColor();            // 获得填充颜色
    QColor color = this->ctColorToQColor(fillColor);        // 获得填充颜色

    CT_Base* base_font =
            (*this->ofdFile->getDocuments())[0]
                ->getIDTable()->getItem(
                    text->getFont().getRefID());
    QFont textFont;
    if(base_font != NULL)
    {
        // 确保存在该数据
        CT_Font* ctFont = (CT_Font*) base_font;
        QString font_name = ctFont->getFontName();      // 字体名
        QString family_name = ctFont->getFamilyName();  // 字体族名
//        qDebug() << font_name << "  "<< family_name;
        textFont.setFamily(family_name);
//        qDebug() << "After setting font family :"<<textFont.family();
    }

    // 处理每一个textCode
    QVector<TextCode *>* textCodes = text->getTextCode();   // 获得文字内容
    int length = textCodes->size();             // 数量
    qDebug() << "textCodes' length:" << length;
    for(int i = 0 ; i< length; i++)
    {
        TextCode* textCode = (*textCodes)[i];   // 当前处理的块
//        qDebug() << "deal with current textcode:" << i;

        // 求出单个块的保卫面积
        double height = text->getSize() + 5;           // 高等于字体的高
        double width = 0;                       // 所有deltax相加再加上一个字宽
        double x = text->boundary.getX() + textCode->getX();    // 左上角 x坐标
        double y = text->boundary.getY() + textCode->getY() - height;   // 左上角y值

        // 计算宽度
        ST_Array arrayDeltaX = textCode->getDeltaX();
//        qDebug() << "deltax length" <<arrayDeltaX.length();
        int deltax_size = arrayDeltaX.length();
        for(int j = 0; j < deltax_size; j++)
        {
//            qDebug() << j << arrayDeltaX[j] ;
            width += arrayDeltaX[j].toDouble();   // 计算宽度
        }

        width += height + 15;        // 假设为方块字吧

//        qDebug() << "has compute the boundary";

        // 获得文本内容
        QString content = textCode->getText();     // 文本内容

        qDebug() << "insert Content:" << content;

        DocTextBlock *textBlock = new DocTextBlock();       // 新建文本块
        DocBlock *block = new DocBlock();                   // 新建块
        block->setWidget(textBlock);                        // 将块放入文本块中

        block->resize(UnitTool::mmToPixel(width),
                      UnitTool::mmToPixel(height));          // 调整块大小
        block->setPos(UnitTool::mmToPixel(x),
                      UnitTool::mmToPixel(y));                   // 设置位置

        QTextCursor cursor = textBlock->textCursor();       // 获得光标

        // 去掉QTextEdit的无用的边框
        textBlock->document()->setDocumentMargin(0);            // document
        QTextFrame * frame = textBlock->document()->rootFrame();
        QTextFrameFormat frameFormat = frame->frameFormat();     // frameFormat

        // 清除没用的边缘距离
        frameFormat.setTopMargin(0);
        frameFormat.setBottomMargin(0);
        frameFormat.setLeftMargin(0);
        frameFormat.setRightMargin(0);
        frameFormat.setPadding(0);

        frame->setFrameFormat(frameFormat);         // 应用Frame格式

        QTextBlockFormat blockFormat = cursor.blockFormat();      // 块格式
        QTextCharFormat charFormat = cursor.charFormat();         // 字符格式
        QFont font = charFormat.font();         // 字体

        font.setPixelSize(UnitTool::mmToPixel(text->getSize()));    // 字号
        font.setFamily(textFont.family());                      // 设置字体

        qDebug() << "italic " << text->getItalic();
        // 倾斜
        if(text->getItalic())
        {
            font.setItalic(true);
        }

        // 加粗
        int weight = text->getWeight();  // 获得粗细
        font.setWeight(weight / 8);

        // 上下间距
        blockFormat.setTopMargin(0);
        blockFormat.setBottomMargin(0);
        blockFormat.setLeftMargin(0);
        blockFormat.setRightMargin(0);
        blockFormat.setLineHeight(0,QTextBlockFormat::MinimumHeight);

        // 应用格式
        charFormat.setFont(font);               // 应用字体格式
        charFormat.setForeground(color);        // 设置文字颜色
        cursor.setCharFormat(charFormat);       // 应用字符格式
        cursor.setBlockFormat(blockFormat);     // 应用文字块样式

        cursor.insertText(content);             // 插入文本

//        qDebug()<<"page->addBlock";
        page->addBlock(block,layer);                    // 插入到场景中

    }

}

void OFD_DocConvertor::insertCT_Path(DocPage *page, CT_Layer *layer, CT_Path *path)
{

}

///
/// \brief OFD_DocConvertor::insertCT_Image
/// \param page
/// \param layer
/// \param image
///
void OFD_DocConvertor::insertCT_Image(
        DocPage *page,
        DocPage::Layer layer,
        CT_Image *image)
{
    DocImageBlock *imageBlock = new DocImageBlock();

    // 获得多媒体资源
    CT_Base* base_media =
            this->ofdFile->getDocuments()->operator [](0)
                ->getIDTable()->getItem(
                    image->getResourceID());
    CT_MultiMedia* multMedia = (CT_MultiMedia *)base_media;

    qDebug() << multMedia->MediaFile;
    imageBlock->setImage(multMedia->MediaFile);             // 设置图片
    imageBlock->setWidthHeightRatioLocked(false);           // 先不要锁定纵横比
    DocBlock * block = new DocBlock();

    block->setWidget(imageBlock);
    ST_Box box = image->boundary;                           // 获得位置大小
    block->resize(
                UnitTool::mmToPixel(box.getDeltaX()),
                UnitTool::mmToPixel(box.getDeltaY()));
    block->setPos(
                UnitTool::mmToPixel(box.getX()),
                UnitTool::mmToPixel(box.getY()));

    page->addBlock(block, layer);                      // 加入到页面中
}

/**
 * @Author Chaoqun
 * @brief  将CT_Color转换为QColor
 * @param  CT_Color *ct_color
 * @return QColor
 * @date   2017/06/22
 */
QColor OFD_DocConvertor::ctColorToQColor(CT_Color *ct_color)
{

    QColor defaultColor(Qt::black);         // 设置默认颜色

    Document* document = (*this->ofdFile->getDocuments())[0];
    ID_Table* table = document->getIDTable();

    CT_Base* base = table->getItem(
                ct_color->getColorSpace().getRefID());

    // 返回NULL 就判断为默认颜色
    if(base == NULL)
    {
        return defaultColor;
    }
    CT_ColorSpace* colorSpace = (CT_ColorSpace*)base;       // 获得颜色空间

    QString colorType = colorSpace->getType();               // 获得颜色类型
    colorType.toUpper();        // 转换为大写字母

    ST_Array value = ct_color->getValue();      // 颜色的值

    if(colorType == "RGB")
    {
        // 如果是RGB颜色空间
        if(value.length() < 3)
        {
            return defaultColor;
        }

        // rgb
        int red = value[0].toInt();
        int green = value[1].toInt();
        int blue = value[2].toInt();

        QColor color;
        color.setRgb(red,green,blue);
        return color;
    }
    else if(colorType == "GRAY")
    {
        // 如果是GRAY颜色空间
        if(value.length() < 1)
        {
            return defaultColor;
        }
        int gray = value[0].toInt();
        QColor color;
        color.setRgb(gray,gray,gray);
    }
    else if(colorType == "CMYK")
    {
        // 如果是CMYK颜色空间
        if(value.length() < 4)
        {
            return defaultColor;
        }
//        qDebug() << value[0] << value[1] << value[2] << value[3];

        // cyan (C), magenta (M), yellow (Y) and black (K)
        int cyan = value[0].toInt();
        int magenta = value[1].toInt();
        int yellow = value[2].toInt();
        int black = value[3].toInt();
        QColor color;
        color.setCmyk(cyan,magenta,yellow,black);
        return color;
    }
    else
    {
        // 如果都不是
        qDebug() << "Can't search the accurate color";
        return defaultColor;
    }

}
