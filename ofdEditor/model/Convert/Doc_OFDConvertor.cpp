#include "Doc_OFDConvertor.h"
#include "Doc/DocPassage.h"
#include "Doc/DocPage.h"
#include "Doc/DocBlock.h"
#include "Doc/DocTextBlock.h"
#include "Doc/DocLayer.h"
#include "Doc/DocImageBlock.h"

#include <QVector>
#include <QTextCursor>
#include <QTextCharFormat>
#include <QTextBlockFormat>
#include <QTextdocument>
#include <QTextBlock>
#include <QFontMetricsF>

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

    qDebug() << "doc to ofd finished";

    return this->ofdFile;

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

    // 新建DocBody
    DocBody* docbody = new DocBody(this->passage->getDocInfo());    // 加入docbody
    docbody->setDocRoot("Doc_0/Document.xml");              // 设置文档根目录
    this->ofdFile->getDocBodies()->append(docbody);         // 设置ofd的docbobys
    this->ofdFile->setDocType(this->passage->getDocType()); // 文档类型
    this->ofdFile->setOfdVersion(this->passage->getVersion());  // 文档类型

    qDebug() << "build docbody finished";

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
//    QVector<Document*> *documents = new QVector<Document*>();               // 新建Documents
    Document* document = new Document();        // 新建Document
//    documents->append(document);                 // 添加到文件中

    // 设置ID_Table
    this->table = document->getIDTable();       // 获得IDTable

    // 设置publicRes\documentRes
    this->public_res = new Res();
    this->public_res->setBaseLoc("Res");
    this->document_res = new Res();
    this->document_res->setBaseLoc("Res");
    document->getPublicRes()->append(this->public_res);   // 将公用资源加入到document
    document->document_res->append(this->document_res);
    // 设置common_data
    CT_CommonData * commonData = new CT_CommonData();
    document->setCommonData(commonData);

    // 设置指向资源文件的
    commonData->getPublicRes()->append(ST_Loc("PublicRes.xml",
                                               "PublicRes.xml",
                                               "PublicRes.xml"));
    commonData->document_res->append(ST_Loc("DocumentRes.xml",
                                            "DocumentRes.xml",
                                            "DocumentRes.xml"));

    this->buildPages(document);     // 解析每一页

    //    this->ofdFile->setDocument(documents);
    commonData->setMaxUnitID(this->table->size());      // 设置最大单元
    this->ofdFile->getDocuments()->append(document);

    qDebug() << "build document finished";
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
    QVector<DocPage* >* docPages = this->passage->getPages(); // 获得页面
    int pagesLength = docPages->size();      // 获得页面的数量

    // 公共页大小，之后改成从DocPassage读取
    if(pagesLength >= 1)
    {
        // 设置公共页大小
        CT_CommonData* commonData = document->getCommonData();

        CT_PageArea *area = new CT_PageArea();

        DocPage* page = docPages->operator [](0);
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
        DocPage* page = docPages->operator [](i);

        Page* ctPage = new Page();
        ctPage->setID(this->table->size()+1,this->table);   // 设置ID
        // 设置页的路径

        ctPage->setBaseLoc(QString("Pages/Page_") + QString::number(i)
                           + "/Content.xml");
        pages->append(ctPage);               // 加入到OFD 文件中

        this->buildPage(ctPage,page);   // 单独处理每一页

    }

    qDebug() << "build Pages finished";

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
    qDebug() << "exec buildPage:" << ctPage->getBaseLoc();
    QVector<CT_Layer *> *layers = ctPage->getContent();

    // pageArea
    CT_PageArea *area = new CT_PageArea();
    area->setPhysicalBox(0,0,
                         docPage->getWidth(),docPage->getHeight());
    ctPage->setArea(area);

    // 分成多个层，对每个层进行处理
    DocLayer* foreground = docPage->getForegroundLayer();   // 获得前景层
    if(foreground->size() > 0)
    {
        CT_Layer* foreLayer = new CT_Layer();                   // 处理前景层
        foreLayer->setType("Foreground");                       // 设置类型
        foreLayer->setID(this->table->size()+1,this->table);    // 设置ID
        layers->append(foreLayer);                               // 加入到Page中
        this->buildLayer(foreLayer,foreground);                 // 处理该层的信息
    }

    // 处理正文层
    DocLayer* body = docPage->getBodyLayer();
    if(body->size() > 0)
    {
        CT_Layer* bodyLayer = new CT_Layer();
        bodyLayer->setType("Body");
        bodyLayer->setID(this->table->size()+1,this->table);
        layers->append(bodyLayer);
        this->buildLayer(bodyLayer,body);
    }


    // 背景层
    DocLayer* background = docPage->getBackgroundLayer();
    if(background->size() > 0)
    {
        CT_Layer* backLayer = new CT_Layer();
        backLayer->setType("Background");
        backLayer->setID(this->table->size()+1,this->table);
        layers->append(backLayer);
        this->buildLayer(backLayer,background);
    }

    qDebug() << "build page Finished";

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
    qDebug() << "Exec buildLayer" << ctLayer->getType();
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
            this->buildText(ctLayer, block->getTextBlock());
            this->buildTextBlock(ctLayer, block->getTextBlock());
        }

        // 如果是图片对象
        if(block->isImageBlock())
        {
            // 将图片移动到资源文件夹，存下路径再结束
            this->buildImage(ctLayer, block->getImageBlock());
        }

        // 如果是表格文件
        if(block->isTableBlock())
        {
            // 除了保存文字，还要画线
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

    QTextDocument* document = cursor.document();    // 获得文档

    cursor.movePosition(QTextCursor::Start);    // 移动到文章开头

    QString lineContent;                // 行内容
    QString lineContent_backup;         // 当前行内容备份
    int pixelContent = 0;               // 当前行已经使用的像素长度
    int pixelContHeight = 0;            // 当前行的最大高度
    int pixelLine = 0;                  // 当前纵向已经进入的深度
    QString tempFragment;               // 块
    QTextFragment fragment;     // 短句

    // lineCount记录当前处理到的行数，第一行从0开始算起
    for(int i = 0, lineCount = -1 ; i < document->blockCount(); i++)
    {
        QTextBlock block = cursor.block();  // 获得当前块
        QTextCursor tempCursor = cursor;    // 复制cursor

        QTextBlock::Iterator iter = block.begin();      // 块的开始

        while(!iter.atEnd()
              || lineContent.length() != 0
              || tempFragment.length() !=0)        // 用来判断是否处理完了
        {
            if(tempFragment.size() == 0 && !iter.atEnd())
            {
                // 如果处理字段为空
                fragment = iter.fragment();   // 获得当前的
                tempFragment = fragment.text();
                iter++;
            }

            if(lineContent == "")
            {
                tempCursor.select(QTextCursor::LineUnderCursor);    // 选择当前行
                QString currentLine = tempCursor.selectedText();    // 选中的文字

                lineContent = currentLine;          // 追加一行
                lineContent_backup = currentLine;   // 当前行内容
                lineCount ++;
                //                qDebug() << "Read Next Line： " << currentLine
                //                         << "LineContent: " << lineContent;
                pixelContent = 0;       // 使用新行，已用长度置空
                pixelContHeight = 0;    // 使用新行，最大高度置零
            }

            QString nextEditContent;    // 即将处理的字段

            // 处理当前Fragment
            if(tempFragment.length() < lineContent.length())
            {
                // 当前小块的长度未达一页

                // 删除重复的部分，用来循环
                lineContent.remove(0,tempFragment.length());     // 把重复的部分先从lineContent中删除
                qDebug() << "use TextFragement: " << tempFragment;
                nextEditContent = tempFragment;                 // 设置待处理字段
                tempFragment.remove(0,tempFragment.length());   // 清空

            }
            else if(tempFragment.length() >= lineContent.length())
            {
                // 当前行已完，但小块未完

                // 删除重复的用来循环
                tempFragment.remove(0,lineContent.length());
                qDebug() << "use remaind lineContent: " << lineContent;
                nextEditContent = lineContent;                      // 设置待处理字段
                lineContent.remove(0,lineContent.length());             // 清空
            }

            // 样式
            QTextCharFormat charFormat = fragment.charFormat();     // 字符格式
            QTextBlockFormat blockFormat = tempCursor.blockFormat();// 块格式
            QFont font = charFormat.font();     // 字体


            // 分析块的大小
            QFontMetricsF metrics(font);

            int pixelWidth = metrics.boundingRect(nextEditContent).width();    // 获得宽度
            int pixelHeight = metrics.boundingRect(nextEditContent).height();     // 获得高度

            if(pixelContHeight < pixelHeight)
            {

                // 如果小于最大行间距
                pixelLine = pixelLine + pixelHeight;               // 纵向深度增加
                qDebug() << "pixelLine: "<< pixelLine
                         << " pixelHeight:" << pixelHeight;
            }

            CT_Text* ct_text = new CT_Text();                       // 新建文本块
            ct_text->setID(this->table->size() +1 ,this->table);    // 设置ID
            ctLayer->getTextObject()->append(ct_text);              // 加入到layer

            // 设置颜色
            QBrush brush = charFormat.foreground();     // 获得画笔
            QColor color = brush.color();               // 获得颜色
            CT_ColorSpace *space = new CT_ColorSpace(); // 颜色空间
            space->setBitsPerComponent(8);
            space->setType("RGB");
            int colorSpaceId = this->addColorSpace(space);
            CT_Color* ct_color = new CT_Color();

            ct_color->setValue(QString::number(color.red()) + " "
                               + QString::number(color.green()) + " "
                               + QString::number(color.blue()));
            ct_color->setColorSpace(colorSpaceId,this->table);

            ct_text->setFillColor(ct_color);


            // 加入字体
            CT_Font* ctfont = new CT_Font();
            ctfont->setFamilyName(font.family());
            ctfont->setFontName(font.family());
            int font_id = this->addFont(ctfont);    // 添加字体ID
            ct_text->setFont(font_id, this->table); // 设置字体引用

            // 设置字体
            if(font.italic())
            {
                // 如果斜体
                ct_text->setItalic( true);
            }
            if(font.weight() != 50)
            {
                // 字重
                ct_text->setWeight(font.weight() * 8);  // 默认值400
            }

            // 设置字体大小
            // 这个只针对/通过 setPixelSize的才有效
//            double font_size = font.pixelSize();    // 获得字大小
//            qDebug() << "Font Size:" <<font.pixelSize();
//            ct_text->setSize(UnitTool::pixelToMM(font_size));
//            int fontPixelSize = metrics.height();       // 获得像素高度
            ct_text->setSize(UnitTool::pixelToMM(pixelHeight));

            qDebug() << "before setBoundary pixelLine: "<< pixelLine;

            // 边框
            ct_text->setBoundary(UnitTool::pixelToMM(textBlock->pos().rx()
                                                     + pixelContent),      // 最左侧，向右偏移一定距离
                                 UnitTool::pixelToMM(textBlock->pos().ry()
                                                     + pixelLine),
                                 UnitTool::pixelToMM(pixelWidth),
                                 UnitTool::pixelToMM(pixelLine) + 5
                                 );

            qDebug() << "After setBoundary pixelLine: "<< pixelLine;

            // 设置delta_x
            TextCode* textCode = new TextCode();        // 文字内容
            textCode->setText(nextEditContent);            // 设置文字内容
            textCode->setX(0);  // 设置起始位置
            textCode->setY(UnitTool::pixelToMM(metrics.height()));

            if(nextEditContent.length()>1)
            {
                qDebug() << "font's wordSpacing:" << font.wordSpacing();
                QString delta_x_str;
                // n个字符 n-1 个 delta_x
                for(int delta_x = 0; delta_x < nextEditContent.length()-1; delta_x++)
                {

                    delta_x_str = delta_x_str + " "
                            + QString::number(
                                UnitTool::pixelToMM(
                                    metrics.boundingRect(
                                        nextEditContent.mid(delta_x,1)).width()));
                }
                textCode->setDeltaX(delta_x_str);
            }

            ct_text->getTextCode()->append(textCode);   // 加入到textCode中

            pixelContent += pixelWidth;         // 将当前使用长度加入，进入下一次循环
        }

        cursor.movePosition(QTextCursor::NextBlock);        //
    }
    //// 将文本框按照 格式和行进行拆分

}

///
/// \brief Doc_OFDConvertor::buildTextBlock
/// \param CT_Layer
/// \param textBlock
///     处理 DocTextBlock 实际相当于计算 QTextEdit中的文本的位置大小
///
void Doc_OFDConvertor::buildTextBlock(CT_Layer *CT_Layer, DocTextBlock *textBlock)
{
    DocBlock *block = textBlock->getBlock();        // 获得Block可以获得文本块的总大小
    double block_pos_x = block->x();
    double block_pos_y = block->y();
    double block_width = block->size().width();
    double block_height = block->size().height();

    int lineCount = 0;                              // 用来计算共有多少行
    textBlock->moveCursor(QTextCursor::Start);      // 将鼠标移动到文档开始
    QTextCursor cursor = textBlock->textCursor();   // 光标

    // 计算每行的文字的最大高度
    QVector<int> lineHeight_Font;                 // 每个字体的实际高度
    QVector<int> lineHeight_Real;                 // 经过段落格式后的高度

    QVector<int> end_of_line_pos;                   // 纪录每一行结尾的地方
    QVector<int> start_of_fragment;                 // 每一个QTextFragment开始的位置

    QTextBlock::iterator it_fragment = cursor.block().begin();

    QString tempFragment;
    // 一行一行的进行遍历
    while(!cursor.atEnd())
    {
        lineCount ++;
        cursor.movePosition(QTextCursor::EndOfLine);    // 移动到当前行尾，纪录位置
        end_of_line_pos.push_back(cursor.position());   // 将该行的结尾位置发出来
        cursor.movePosition(QTextCursor::Right);

        // 先用每块的字体初始化一下 lineHeight_Font
        QTextCharFormat charFormat = cursor.charFormat();
        QFontMetrics fontMetrics(charFormat.font());

        lineHeight_Font.push_back(
                    fontMetrics.ascent() + fontMetrics.descent() + fontMetrics.leading() + 1);
    }

    QTextFrame::iterator  it = textBlock->document()->rootFrame()->begin();     // 遍历rootFrame
    while (!it.atEnd())
    {
        // QTextBlock
        QTextBlock  block = it.currentBlock();  // 当前block
        qDebug() << "block "<< block.text();

        QTextBlock::iterator block_it = block.begin();
        while(!block_it.atEnd())
        {
            // 遍历QTextFragment
            QTextFragment fragment = block_it.fragment();
            start_of_fragment.push_back(fragment.position());

            // 判断 fragment 影响哪几行的高度
            int lineHeight_fragment = 0;
            QFontMetricsF fontMetrics(fragment.charFormat().font());
            lineHeight_fragment = fontMetrics.ascent() + fontMetrics.descent() + fontMetrics.leading() + 1;

            // 利用快的起点和重点，判断影响了哪几行
            int start_pos = fragment.position();
            int end_pos = fragment.position() + fragment.length();
            int start_line = 0;
            int end_line = 0;

            for(int line = 0; line < end_of_line_pos.size(); line ++)
            {
                if(start_pos > end_of_line_pos[line])
                {
                    start_line = line + 1;
                }

                if(end_pos < end_of_line_pos[line])
                {
                    end_line = line;
                    break;
                }
            }
            qDebug() << "fragment pos"<<fragment.position()
                     << start_line << end_line << lineHeight_fragment << fragment.text();

            for(int line = start_line; line <= end_line; line ++)
            {
                if(lineHeight_fragment > lineHeight_Font[line])
                {
                    lineHeight_Font[line] = lineHeight_fragment;
                }
            }

            block_it ++;
        }           // 遍历fragment
        it ++;
    }               // 遍历block

    lineHeight_Real = lineHeight_Font;      // 复制内容

    // 遍历每行，根据行高规则计算行高
    for(int line = 0 ; line < end_of_line_pos.size(); line ++)
    {
        cursor.setPosition(end_of_line_pos[line]);
        QTextBlockFormat blockFormat = cursor.blockFormat();

        int lineHeightType = blockFormat.lineHeightType();
        int lineHeightValue = blockFormat.lineHeight();

        switch (lineHeightType)
        {
        case QTextBlockFormat::SingleHeight:
            // 单倍行距不用改
            break;
        case QTextBlockFormat::MinimumHeight:
            // 最小值
            if(lineHeight_Font[line] < lineHeightValue)
            {
                lineHeight_Real[line] = lineHeightValue;
            }
            else
            {
                lineHeight_Real[line] = lineHeight_Font[line];
            }
            break;
        case QTextBlockFormat::FixedHeight:
            // 固定值
            lineHeight_Real[line] = lineHeightValue;
            break;
        case QTextBlockFormat::ProportionalHeight:
            // 多倍行距
            lineHeight_Real[line] = (int)(1.0 * lineHeightValue / 100 * lineHeight_Font[line]);
            break;
        default:
            break;
        }
    }

    qDebug() << "lineCount" << lineCount << endl
             << "end of line" << end_of_line_pos    << endl
             << " start of fragements" << start_of_fragment << endl
             << "line height font" << lineHeight_Font << endl
             << "line height real" << lineHeight_Real << endl;

    // ---------------- 行高计算完毕，下面应该做啥子


}

///
/// \brief Doc_OFDConvertor::buildQTextBlock
/// \param ctlayer
/// \param textBlock
/// \param pos_x
/// \param pos_y
/// \param width
/// \param height
///     使用提供的大小做为QTextBlock的大小进行处理文本
///
void Doc_OFDConvertor::buildQTextBlock(
        CT_Layer *ctlayer,
        QTextBlock *textBlock,
        double pos_x,
        double pos_y,
        double width,
        double height)
{

}

///
/// \brief Doc_OFDConvertor::buildImage
/// \param ctLayer
/// \param imageBlock
///
void Doc_OFDConvertor::buildImage(CT_Layer *ctLayer, DocImageBlock *imageBlock)
{
    // 新建所需的对象，设置ID
    CT_Image *ctimage = new CT_Image();                 // iamgeobject
    CT_MultiMedia *multiMedia = new CT_MultiMedia();    // 多媒体
    ctimage->setID(this->table->size() + 1,
                   this->table);
    multiMedia->setID(this->table->size() + 1,
                      this->table);
    ctimage->setResourceID(multiMedia->getID().getID(), this->table);

    // 设置多媒体引用的标签
    multiMedia->Format = "";
    multiMedia->Type = "Image";
    multiMedia->MediaFile = imageBlock->getFileName();

    // 将图片存储到临时文件夹去
    QString fileName = this->passage->getTempSavePath()
            + "/" + multiMedia->MediaFile;
    imageBlock->saveImage(fileName);

    DocBlock* block = imageBlock->getBlock();

    // 设置边界
    ctimage->setBoundary(
                UnitTool::pixelToMM(block->x()),
                UnitTool::pixelToMM(block->y()),
                UnitTool::pixelToMM(block->size().width()),
                UnitTool::pixelToMM(block->size().height()));


    this->document_res->getMultiMedia()->push_back(multiMedia);     // 追加到资源文件内
    ctLayer->image_object->push_back(ctimage);
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

