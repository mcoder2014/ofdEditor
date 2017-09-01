#include "ofd_writer.h"
#include <QDebug>

#include "DataTypes/document/OFD.h"
#include "DataTypes/document/DocBody.h"
#include "DataTypes/document/CT_DocInfo.h"
#include "DataTypes/document/Document.h"
#include "DataTypes/page/Page.h"
#include "DataTypes/Color/CT_Color.h"
#include "DataTypes/image/CT_GraphicUnit.h"
#include "DataTypes/text/TextCode.h"
#include "DataTypes/text/CT_Text.h"
#include "DataTypes/image/CT_Path.h"
#include "DataTypes/image/CT_Image.h"

OFDWriter::OFDWriter(OFD * _data, QString _path)
    : data(_data), current_path("OFD", _path + "OFD.xml")
{
//    writeOFD();
}

void OFDWriter::createFile()
{
    current_file = new QFile(current_path.getPath());
    qDebug() << current_path.getPath() << endl;
    if (!current_file->open(QFile::WriteOnly | QFile::Text))
    {
        qDebug() << "can't open or write file";
        throw WritingFileException("无法打开或创建XML文件: " + current_path.getPath());
    }
    writer.setDevice(current_file);
}

void OFDWriter::writeOFD()
{
    createFile();
    //    qDebug() << "Checkpoint 1";
    writer.writeStartDocument();
    writer.setAutoFormatting(true);

    //进入正文
    writer.writeStartElement("ofd:OFD");
    writer.writeAttribute("xmlns:ofd", "http://www.ofdspec.org");
    QXmlStreamAttributes a = getAttributes(data);
    writer.writeAttributes(a);

    //写DocBody
    qDebug() << "DocBodys size: "<< data->getDocBodies()->size();
    for (int i = 0; i < data->getDocBodies()->size(); i++)
    {
        DocBody * cur_docbody = data->getDocBodies()->operator [](i);
        writer.writeStartElement("ofd:DocBody");
        CT_DocInfo * cur_docinfo = cur_docbody->getDocInfo();
        writer.writeStartElement("ofd:DocInfo");

        //写DocInfo
        //            qDebug() << "Checkpoint 1.1";
        if (!cur_docinfo->getDocID().isNull())
        {
            writer.writeTextElement("ofd:DocID", cur_docinfo->getDocID());
        }
        else
        {
            qDebug("DocInfo类型数据缺少必要的成员DocID");
        }

        //        qDebug() << "Checkpoint 1.2";
        if (!cur_docinfo->getTitle().isNull())
        {
            //            qDebug() << "Checkpoint 1.2.1";
            cur_docinfo->getTitle();
            //            qDebug() << "Checkpoint 1.2.1.5";
            writer.writeTextElement("ofd:Title", cur_docinfo->getTitle());
            //            qDebug() << "Checkpoint 1.2.2";
        }
        //        qDebug() << "Checkpoint 1.3";
        if (!cur_docinfo->getAuthor().isNull())
        {
            //            qDebug() << "Checkpoint 1.3.1";
            writer.writeTextElement("ofd:Author", cur_docinfo->getAuthor());
            //            qDebug() << "Checkpoint 1.3.2";
        }
        //        qDebug() << "Checkpoint 1.4";
        if (!cur_docinfo->getSubject().isNull())
        {
            writer.writeTextElement("ofd:Subject", cur_docinfo->getSubject());
        }
        //        qDebug() << "Checkpoint 1.5";
        if (!cur_docinfo->getAbstract().isNull())
        {
            writer.writeTextElement("ofd:Abstract", cur_docinfo->getAbstract());
        }
        if (!cur_docinfo->getCreationDate().isNull())
        {
            writer.writeTextElement("ofd:CreationDate", cur_docinfo->getCreationDate());
        }
        if (!cur_docinfo->getModDate().isNull())
        {
            writer.writeTextElement("ofd:ModDate", cur_docinfo->getModDate());
        }
        if (!cur_docinfo->getDocUsage().isNull())
        {
            writer.writeTextElement("ofd:DocUsage", cur_docinfo->getDocUsage());
        }
        //Cover, keywords to be implmented
        if (!cur_docinfo->getCreator().isNull())
        {
            writer.writeTextElement("ofd:Creator", cur_docinfo->getCreator());
        }
        if (!cur_docinfo->getCreatorVersion().isNull())
        {
            writer.writeTextElement("ofd:CreatorVersion", cur_docinfo->getCreatorVersion());
        }
        //        qDebug() << "Checkpoint 1.6";
        //写CustomDatas
        QVector<QStringList> * cur_custom_datas = cur_docinfo->getCustomDatas();
        if (cur_custom_datas->size() != 0)
        {
            writer.writeStartElement("ofd:CustomDatas");
            for (int i = 0; i < cur_custom_datas->size(); i++)
            {
                writer.writeStartElement("ofd:CustomData");
                writer.writeAttribute("Name", cur_custom_datas->at(i).at(0));
                writer.writeCharacters(cur_custom_datas->at(i).at(1));
                writer.writeEndElement();
            }
            writer.writeEndElement();
        }
        writer.writeEndElement();
        //写DocRoot
        if (!cur_docbody->getDocRoot().isNull())
        {
            ST_Loc root("", data->getRootPath().getPath().remove("OFD.xml"));
            writer.writeTextElement("ofd:DocRoot", cur_docbody->getDocRoot().getRelativePath());
            qDebug() << "DocRoot :" << cur_docbody->getDocRoot();
        }
        writer.writeEndElement();
    }
    //正文结束
    //        qDebug() << "Checkpoint 1.3";
    writer.writeEndDocument();
    current_file->close();
    //修改current_file
    ST_Loc cur_path = current_path;
    for (int i = 0; i < data->getDocBodies()->size(); i++)
    {
        ST_Loc p("Document",
                 data->getDocBodies()->at(i)->getDocRoot().getRelativePath(),
                 cur_path);
        current_path = p;
        //                qDebug() << "Checkpoint 1.4";
        qDebug() << " current Path"
                 << current_path.getRelativePath();
        makePath(current_path);
        //                qDebug() << "Checkpoint 1.5";
        writeDocument(data->getDocuments()->operator [](i));
    }
    qDebug() << "Document write finished";
}

void OFDWriter::writeDocument(Document * data)
{
    qDebug() << "writeDocument";
    createFile();
    //    qDebug() << "Checkpoint 2";
    //qDebug() << "Checkpoint 1 reached.";
    writer.writeStartDocument();
    writer.setAutoFormatting(true);
    //进入正文
    writer.writeStartElement("ofd:Document");
    writer.writeAttribute("xmlns:ofd", "http://www.ofdspec.org");
    //写CommonData
    if (data->getCommonData() != NULL)
    {
        writer.writeStartElement("ofd:CommonData");
        CT_CommonData * cur_common_data = data->getCommonData();
        if (!cur_common_data->getMaxUnitID().isNull())
        {
            writer.writeTextElement("ofd:MaxUnitID",
                                    QString::number(cur_common_data->getMaxUnitID().getID()));
        }
        else
        {
            //             qDebug("CT_CommonData类型数据缺少必要的成员MaxUnitID");
            qDebug() << "CT_CommonData类型数据缺少必要的成员MaxUnitID";
        }
        if (cur_common_data->getPageArea())
        {
            writer.writeStartElement("ofd:PageArea");
            writePageArea(cur_common_data->getPageArea());
            writer.writeEndElement();   //ofd:PageArea
        }
        else
        {
            //             qDebug("CT_CommonData类型数据缺少必要的成员PageArea");
            qDebug() << "CT_CommonData类型数据缺少必要的成员PageArea";
        }

        for (int i = 0; i < data->getCommonData()->getPublicRes()->size(); i++)
        {
            ST_Loc p = data->getCommonData()->getPublicRes()->at(i);
            writer.writeTextElement("ofd:PublicRes", p.getRelativePath());
        }

        for(int i = 0; i < data->getCommonData()->document_res->size(); i++)
        {
            ST_Loc p = data->getCommonData()->document_res->operator [](i);
            writer.writeTextElement("ofd:DocumentRes", p.getRelativePath());
        }
        //TemplatePage & DefaultCS to be implemented
        writer.writeEndElement();   //ofd:CommonData
    }
    else
    {
        //         qDebug("Document类型数据缺少必要的成员CommonData");
        qDebug() << "Document类型数据缺少必要的成员CommonData";
    }
    //写Pages
    if (data->getPages())
    {
        CT_Pages * cur_pages = data->getPages();
        writer.writeStartElement("ofd:Pages");
        for (int i = 0; i < cur_pages->getPages()->size(); i++)
        {
            Page * cur_page = cur_pages->getPages()->at(i);
            writer.writeStartElement("ofd:Page");
            writeBase(cur_page);
            writer.writeAttribute("BaseLoc", cur_page->getBaseLoc().getRelativePath());
            writer.writeEndElement();
        }
        writer.writeEndElement();   //ofd:Pages
    }
    else
    {
        //         qDebug("Document类型数据缺少必要的成员Pages");
        qDebug() << "Document类型数据缺少必要的成员Pages";
    }
    //写CustomTags
    if (!data->getCustomTags().isNull())
    {
        writer.writeTextElement("ofd:CustomTags", data->getCustomTags().getRelativePath());
    }

    //    qDebug() << "Document main part finished";

    //Outlines, extensions, annotations to be implemented
    //正文结束
    writer.writeEndDocument();
    current_file->close();
    ST_Loc cur_path = current_path;
    for (int i = 0; i < data->getPages()->getPages()->size(); i++)
    {
        ST_Loc p("Page", data->getPages()->getPages()->operator [](i)->
                getBaseLoc().getRelativePath(), cur_path);
        //        qDebug() << "Page" << i << " : "
        //                 << data->getPages()->getPages()->operator [](i)->getBaseLoc().getRelativePath();
        current_path = p;
        makePath(current_path);
        writePage(data->getPages()->getPages()->operator [](i));
    }
    //    qDebug() << "write pages finished";
    qDebug() << "PublicRes size = " << data->getPublicRes()->size();
    for (int i = 0; i < data->getPublicRes()->size(); i++)
    {
        ST_Loc p("PublicRes",
                 (data->getCommonData()->getPublicRes()->operator [](i)).getRelativePath(),
                cur_path);
        qDebug() << "publicRes loc: " << p.getRelativePath();
        current_path = p;
        makePath(current_path);
        qDebug() << "current path = " << current_path;
        writeRes(data->getPublicRes()->operator [](i));
        qDebug() << "current res baseloc = " << (data->getPublicRes()->operator [](i))->getBaseLoc();
    }
    //    qDebug() << "write publicRes finished";

    qDebug() << "DocumentRes size " <<data->document_res->size();
    for (int i = 0; i < data->document_res->size(); i++)
    {
        ST_Loc p("DocumentRes",
                 (data->getCommonData()->document_res->operator [](i)).getRelativePath(),
                cur_path);
        qDebug() << "DocumentRes loc: " << p.getRelativePath();
        current_path = p;
        makePath(current_path);
        qDebug() << "current path = " << current_path;
        writeRes(data->document_res->operator [](i));
        qDebug() << "current res baseloc = " << (data->document_res->operator [](i))->getBaseLoc();
    }

    //write CustomTags to be implemented
}

void OFDWriter::writePage(Page * data)
{
    createFile();
    //    qDebug() << "Checkpoint 3";
    writer.writeStartDocument();
    writer.setAutoFormatting(true);
    //进入正文
    writer.writeStartElement("ofd:Page");
    writer.writeAttribute("xmlns:ofd", "http://www.ofdspec.org");
    //Template to be implemented
    //写Area
    if (data->getArea())
    {
        writer.writeStartElement("ofd:Area");
        writePageArea(data->getArea());
        writer.writeEndElement();    //ofd:Area
    }
    //    qDebug() << "Checkpoint 1 reached.";
    //写PageRes
    for (int i = 0; i < data->getPageResLocations()->size(); i++)
    {
        ST_Loc p = data->getPageResLocations()->at(i);
        writer.writeTextElement("ofd:PageRes", p.getRelativePath());
    }
    //    qDebug() << "Checkpoint 2 reached.";
    //写Content
    if (data->getContent()->size())
    {
        writer.writeStartElement("ofd:Content");
        for (int i = 0; i < data->getContent()->size(); i++)
        {
            //            qDebug() << data->getContent()->size();
            CT_Layer * cur_layer = data->getContent()->operator [](i);
            if (cur_layer->getImageObject()->size() + cur_layer->getPathObject()->size()
                    +cur_layer->getTextObject()->size() + cur_layer->getPageBlock()->size()
                    > 0)
            {
                writer.writeStartElement("ofd:Layer");
                writeBase(cur_layer);
                //            qDebug() << "Checkpoint 2.1 reached.";
                QXmlStreamAttributes a = getAttributes(cur_layer);
                writer.writeAttributes(a);
                //            qDebug() << "Checkpoint 2.2 reached.";
                //                        qDebug() << "???";
                //写文档页面的内容（QGraphicUnits的子类集合）
                writePageBlock(cur_layer, true);
                writer.writeEndElement();
            }
        }
        writer.writeEndElement();   //ofd:Content
        //        qDebug() << "????";
    }
    //    qDebug() << "Checkpoint 3 reached.";
    //Actions to be implemented
    writer.writeEndElement();   //ofd:Page
    //正文结束
    writer.writeEndDocument();
    current_file->close();
}

void OFDWriter::writeRes(Res * data)
{
    createFile();
    writer.writeStartDocument();
    writer.setAutoFormatting(true);
    //进入正文
    writer.writeStartElement("ofd:Res");
    writer.writeAttribute("xmlns:ofd", "http://www.ofdspec.org");
    QString resFloderPath;

    //写属性
    if (!data->getBaseLoc().isNull())
    {
        writer.writeAttribute("BaseLoc", data->getBaseLoc().getRelativePath());

        resFloderPath = current_path.getPath().left(current_path.getPath().lastIndexOf('/') + 1)
                + data->getBaseLoc().getPath() + "/"; // 获得绝对路径
        qDebug() << "Res "<< resFloderPath;

        if(!QDir(resFloderPath).exists())
        {
            this->makePath(ST_Loc(resFloderPath,
                                  resFloderPath,
                                  ""));
        }
    }
    else
    {
        qDebug() << "class Res lack the required attribute baseloc";
        //         qDebug("Res类型数据缺少必要的属性BaseLoc");
    }
    //写成员
    // colorSpaces
    if (data->getColorSpaces()->size())
    {
        qDebug() << "Res Save colorspaces";
        writer.writeStartElement("ofd:ColorSpaces");
        for (int i = 0; i < data->getColorSpaces()->size(); i++)
        {
            CT_ColorSpace * cur_colorspace = data->getColorSpaces()->operator [](i);
            writer.writeStartElement("ofd:ColorSpace");
            writeColorSpace(cur_colorspace);
            writer.writeEndElement();   //ofd:ColorSpace
        }
        writer.writeEndElement();   //ofd:ColorSpaces
    }

    // drawParams
    if (data->getDrawParams()->size())
    {
        qDebug() << "Res save drawparams";
        writer.writeStartElement("ofd:DrawParams");
        for (int i = 0; i < data->getDrawParams()->size(); i++)
        {
            CT_DrawParam * cur_draw_param = data->getDrawParams()->operator [](i);
            writer.writeStartElement("ofd:DrawParam");
            writeDrawParam(cur_draw_param);
            writer.writeEndElement();   //ofd:DrawParam
        }
        writer.writeEndElement();   //ofd:DrawParams
    }

    // font
    if (data->getFonts()->size())
    {
        qDebug() << "Res Save fonts";
        writer.writeStartElement("ofd:Fonts");
        for (int i = 0; i < data->getFonts()->size(); i++)
        {
            CT_Font * cur_font = data->getFonts()->operator [](i);
            writer.writeStartElement("ofd:Font");
            writeFont(cur_font);
            writer.writeEndElement();   //ofd:Font
        }
        writer.writeEndElement();   //ofd:Fonts
    }

    //MultiMedias & CompositeGraphicUnits to be implemented
    if (data->getMultiMedia()->size())
    {
        qDebug() << "Res Save multimedia";
        writer.writeStartElement("ofd:MultiMedias");
        for(int i = 0; i < data->getMultiMedia()->size(); i++)
        {
            CT_MultiMedia* multMedia = data->getMultiMedia()->operator [](i);
            writer.writeStartElement("ofd:MultiMedia");
            writeMultiMedia(multMedia);
            writer.writeStartElement("ofd:MediaFile");
            writer.writeCharacters(multMedia->MediaFile);
            writer.writeEndElement();   // mediafile

            writer.writeEndElement();   // ofd:MultiMedia
            qDebug() << this->tempPath + "/" + multMedia->MediaFile;
            qDebug() << current_path.getPath().left(current_path.getPath().lastIndexOf('/') + 1)
                        + data->getBaseLoc().getRelativePath() + "/" + multMedia->MediaFile;

            // 将图片拷贝过去
            qDebug() << QFile::copy(this->tempPath + "/" + multMedia->MediaFile,
                        resFloderPath + "/" + multMedia->MediaFile);
        }
        writer.writeEndElement();       // ofd:MultiMedias

    }

    writer.writeEndElement();   //ofd:Res
    //正文结束
    writer.writeEndDocument();
    current_file->close();
}

void OFDWriter::writePageArea(CT_PageArea * cur_page_area)
{
    if (!cur_page_area->getPhysicalBox().isNull()) {
        ST_Box b = cur_page_area->getPhysicalBox();
        writer.writeTextElement("ofd:PhysicalBox",
                                QString::number(b.getX()) + " " +
                                QString::number(b.getY()) + " " +
                                QString::number(b.getDeltaX())+ " " +
                                QString::number(b.getDeltaY()));
    }
    else
    {
        qDebug("CT_PageArea类型数据缺少必要的成员PhysicalBox");
    }
    if (!cur_page_area->getApplicationBox().isNull())
    {
        ST_Box b = cur_page_area->getApplicationBox();
        writer.writeTextElement("ofd:ApplicationBox",
                                QString::number(b.getX()) + " " +
                                QString::number(b.getY()) + " " +
                                QString::number(b.getDeltaX())+ " " +
                                QString::number(b.getDeltaY()));
    }
    if (!cur_page_area->getContentBox().isNull())
    {
        ST_Box b = cur_page_area->getContentBox();
        writer.writeTextElement("ofd:ContentBox",
                                QString::number(b.getX()) + " " +
                                QString::number(b.getY()) + " " +
                                QString::number(b.getDeltaX())+ " " +
                                QString::number(b.getDeltaY()));
    }
    if (!cur_page_area->getBleedBox().isNull())
    {
        ST_Box b = cur_page_area->getBleedBox();
        writer.writeTextElement("ofd:BleedBox",
                                QString::number(b.getX()) + " " +
                                QString::number(b.getY()) + " " +
                                QString::number(b.getDeltaX())+ " " +
                                QString::number(b.getDeltaY()));
    }
}

void OFDWriter::writePageBlock(CT_PageBlock * cur_page_block, bool is_layer)
{
    if (!is_layer)
        writeBase(cur_page_block);

    //文字内容
    //    qDebug() << "TextObject: " << cur_page_block->getTextObject()->size();
    for (int i = 0; i < cur_page_block->getTextObject()->size(); i++)
    {
        CT_Text * cur_text = cur_page_block->getTextObject()->operator [](i);
        writer.writeStartElement("ofd:TextObject");
        //        qDebug() << "About to write Text Object.";
        writeTextObject(cur_text);
        writer.writeEndElement();   //ofd:TextObject
    }
    //矢量图内容
    //    qDebug() << "PathObject: " << cur_page_block->getPathObject()->size();
    for (int i = 0; i < cur_page_block->getPathObject()->size(); i++)
    {
        CT_Path * cur_path = cur_page_block->getPathObject()->operator [](i);
        writer.writeStartElement("ofd:PathObject");
        writePathObject(cur_path);
        writer.writeEndElement();   //ofd:PathObject
    }
    //位图内容
    //    qDebug() << "ImageObject: " << cur_page_block->getImageObject()->size();
    for (int i = 0; i < cur_page_block->getImageObject()->size(); i++)
    {
        CT_Image * cur_image = cur_page_block->getImageObject()->operator [](i);
        writer.writeStartElement("ofd:ImageObject");
        writeImageObject(cur_image);
        writer.writeEndElement();   //ofd:ImageObject
    }
    //PageBlock嵌套
    //    qDebug() << "PageBlock: " << cur_page_block->getPageBlock()->size();
    for (int i = 0; i < cur_page_block->getPageBlock()->size(); i++)
    {
        CT_PageBlock * cur_inner_page_block = cur_page_block->getPageBlock()->operator [](i);
        writer.writeStartElement("ofd:PageBlock");
        writePageBlock(cur_inner_page_block);
        writer.writeEndElement();
    }
}

void OFDWriter::writeTextObject(CT_Text * cur_text)
{
    writeGraphicUnitAttributes(cur_text);
    //    qDebug() << "!!!!";
    QXmlStreamAttributes a = getAttributes(cur_text);
    //    qDebug() << "!!!!????";
    //    qDebug() << "cur_text->attributes is empty: " << a.isEmpty();
    //    for (int i = 0; i < a.size(); i++)
    //        qDebug() << a.at(i).name() << " " << a.at(i).value();
    writer.writeAttributes(a);
    //    qDebug() << "Checkpoint 5 reached.";
    writeGraphicUnitMemebers(cur_text);
    //    qDebug() << "Checkpoint 6 reached.";
    //CGTransform to be implemented
    if (cur_text->getTextCode())
    {
        QVector<TextCode *> * cur_textcode = cur_text->getTextCode();
        for (int i = 0; i < cur_textcode->size(); i++)
        {
            writer.writeStartElement("ofd:TextCode");
            QXmlStreamAttributes a = getAttributes(cur_textcode->at(i));
            writer.writeAttributes(a);
            writer.writeCharacters(cur_textcode->at(i)->getText());
            writer.writeEndElement();   //ofd:TextCode
        }
        //            qDebug() << "Checkpoint 7 reached.";
    }
    else
    {
        qDebug("CT_Text类型数据缺少必要的成员TextObject");
    }
}

void OFDWriter::writePathObject(CT_Path * cur_path)
{
    writeGraphicUnitAttributes(cur_path);
    QXmlStreamAttributes a = getAttributes(cur_path);
    writer.writeAttributes(a);
    writeGraphicUnitMemebers(cur_path);
    //    qDebug() << "???";
    if (!cur_path->getAbbreviatedData().isNull())
    {
        writer.writeTextElement("ofd:AbbreviatedData",
                                cur_path->getAbbreviatedData());
        //        qDebug() << "????";
    }
    else
    {
        qDebug("CT_Path类型数据缺少必要的成员AbbreviatedData");
    }
}

void OFDWriter::writeImageObject(CT_Image * cur_image)
{
    writeGraphicUnitAttributes(cur_image);
    QXmlStreamAttributes a = getAttributes(cur_image);
    writer.writeAttributes(a);
    writeGraphicUnitMemebers(cur_image);
}

void OFDWriter::writeGraphicUnitAttributes(CT_GraphicUnit * cur_graphic_unit)
{
    writeBase(cur_graphic_unit);
    QXmlStreamAttributes a = getAttributes(cur_graphic_unit);
    writer.writeAttributes(a);
}

void OFDWriter::writeGraphicUnitMemebers(CT_GraphicUnit * cur_graphic_unit)
{
    //    qDebug() << "Checkpoint 10 reached.";
    //    qDebug() << cur_graphic_unit->getID();
    //    qDebug() << "Checkpoint 10.1 reached.";
    if (cur_graphic_unit->getFillColor())
    {
        writer.writeStartElement("ofd:FillColor");
        writeColor(cur_graphic_unit->getFillColor());
        writer.writeEndElement();   //ofd:FillColor
    }
    //    qDebug() << "Checkpoint 11 reached.";
    if (cur_graphic_unit->getStrokeColor())
    {
        qDebug() << "Checkpoint 11.1 reached.";
        writer.writeStartElement("ofd:StrokeColor");
        writeColor(cur_graphic_unit->getStrokeColor());
        qDebug() << "Checkpoint 11.2 reached.";
        writer.writeEndElement();   //ofd:StrokeColor
    }
    //    qDebug() << "Checkpoint 12 reached.";
}

void OFDWriter::writeColor(CT_Color * cur_color)
{

    //    qDebug() << "Checkpoint 20 reached.";
    //    qDebug() << "cur_color = " << cur_color;
    QXmlStreamAttributes a = getAttributes(cur_color);
    writer.writeAttributes(a);
    //Pattern and AxialShd / RadialShd to be implemented
}

void OFDWriter::writeBase(CT_Base * cur_base)
{
    //qDebug() << "Base???";
    if (!cur_base->getID().isNull())
    {
        writer.writeAttribute("ID", QString::number(cur_base->getID().getID()));
    }
    else
    {
        qDebug("CT_Base类型数据缺少必要的属性ID");
    }
}

void OFDWriter::writeColorSpace(CT_ColorSpace * cur_colorspace)
{
    writeBase(cur_colorspace);
    QXmlStreamAttributes a = getAttributes(cur_colorspace);
    writer.writeAttributes(a);
    //Palette to be implemented
}

void OFDWriter::writeDrawParam(CT_DrawParam * cur_draw_param)
{
    writeBase(cur_draw_param);
    QXmlStreamAttributes a = getAttributes(cur_draw_param);
    writer.writeAttributes(a);

    if (cur_draw_param->fillColorUsed())
    {
        writer.writeStartElement("ofd:FillColor");
        writeColor(cur_draw_param->getFillColor());
        writer.writeEndElement();   //ofd:FillColor
    }
    if (cur_draw_param->strokeColorUsed())
    {
        writer.writeStartElement("ofd:StrokeColor");
        writeColor(cur_draw_param->getStrokeColor());
        writer.writeEndElement();   //ofd:StrokeColor
    }
}

void OFDWriter::writeFont(CT_Font * cur_font)
{

    writeBase(cur_font);
    QXmlStreamAttributes a = getAttributes(cur_font);
    writer.writeAttributes(a);
    //FontFile to be implemented
}

void OFDWriter::writeMultiMedia(CT_MultiMedia *cur_multiMedia)
{
    writeBase(cur_multiMedia);
    QXmlStreamAttributes a = getAttributes(cur_multiMedia);
    writer.writeAttributes(a);
//    qDebug() << "writemultimedia finished";
}

QXmlStreamAttributes getAttributes(OFD * data)
{
    QXmlStreamAttributes a;
    if (!data->getDocType().isNull())
    {
        a.append("DocType", data->getDocType());
    }
    else
    {
        qDebug("OFD类型数据缺少必要的属性DocType");
    }
    if (!data->getOfdVersion().isNull())
    {
        a.append("Version", data->getOfdVersion());
    }
    else
    {
        qDebug("OFD类型数据缺少必要的属性Version");
    }
    return a;
}

QXmlStreamAttributes getAttributes(CT_Layer * cur_layer)
{
    QXmlStreamAttributes a;
    //    if (cur_layer->getType() != "Body")
    a.append("Type", cur_layer->getType());
    if (!cur_layer->getDrawParam().isNull())
    {
        a.append("DrawParam",QString::number(cur_layer->getDrawParam().getRefID()));
    }
    return a;
}

QXmlStreamAttributes getAttributes(CT_GraphicUnit * cur_graphic_unit)
{
    QXmlStreamAttributes a;
    //    qDebug() << "Boundary is null: " << cur_graphic_unit->getBoundary().isNull();
    if (!cur_graphic_unit->getBoundary().isNull())
    {
        ST_Box b = cur_graphic_unit->getBoundary();
        a.append("Boundary",
                 QString::number(b.getX()) + " " +
                 QString::number(b.getY()) + " " +
                 QString::number(b.getDeltaX()) + " " +
                 QString::number(b.getDeltaY()));
    }
    else
    {
        qDebug("CT_GraphicUnit类型数据缺少必要的属性Boundary");
    }
    //    qDebug() << "CT_Graphic Unit Name: " << cur_graphic_unit->getName();
    //    qDebug() << "CT_Graphic Unit Name is Null: " << cur_graphic_unit->getName().isNull();
    if (cur_graphic_unit->getName().length() != 0)
    {
        a.append("Name", cur_graphic_unit->getName());
    }

    //当Visible为false时才写出
    if (!cur_graphic_unit->getVisible())
    {
        a.append("Visible", "false");
    }

    //CTM to be implemented

    if (!cur_graphic_unit->getDrawParam().isNull())
    {
        a.append("DrawParam", QString::number(cur_graphic_unit->getDrawParam()));
    }

    //Other attributes to be implemented
    return a;

}

QXmlStreamAttributes getAttributes(CT_Color *cur_color)
{
    QXmlStreamAttributes a;
    //    qDebug() << "Checkpoint 30 reached.";
    if (!cur_color->getValue().isNull())
    {
        //        qDebug() << "Checkpoint 30.1 reached.";
        a.append("Value", cur_color->getValue().getAllContent());
    }
    //    qDebug() << "Checkpoint 31 reached.";
    if (cur_color->indexUsed())
    {
        a.append("Index", QString::number(cur_color->getIndex()));
    }
    //    qDebug() << "Checkpoint 32 reached.";
    if (!cur_color->getColorSpace().isNull())
    {
        a.append("ColorSpace", QString::number(cur_color->getColorSpace().getRefID()));
    }
    //只有当Alpha值为非默认的255时才显示
    //    qDebug() << "Checkpoint 33 reached.";
    if (cur_color->getAlpha() != 255)
    {
        a.append("Alpha", QString::number(cur_color->getAlpha()));
    }

    return a;
}

QXmlStreamAttributes getAttributes(CT_Text * cur_text)
{
    //    qDebug() << "??";
    QXmlStreamAttributes a;
    //    qDebug() << "???";
    //    qDebug() << "cur_text->getFont().isNull(): "
    //             <<cur_text->getFont().isNull();

    if (!cur_text->getFont().isNull())
    {
        a.append("Font", QString::number(cur_text->getFont().getRefID()));
    }
    else
    {
        qDebug("CT_Text类型数据缺少必要的属性Font");
    }
    //    qDebug() << "cur_text size used: " << cur_text->sizeUsed();
    //    qDebug() << "cur_text size: " << cur_text->getSize();
    //    if (cur_text->sizeUsed()) {
    a.append("Size", QString::number(cur_text->getSize()));
    //    } else {
    //        qDebug() << "CT_Text类型数据缺少必要的属性Size";
    //    }
    //Stroke属性不为默认值false时才显示
    if (cur_text->getStroke())
    {
        a.append("Stroke", "true");
    }
    //Fill属性不为默认值true时才显示
    if (!cur_text->getFill())
    {
        a.append("Fill", "false");
    }
    //HScale属性不为默认值1.0时才显示
    if (abs(cur_text->getHScale() - 1.0) > 0.00000001)
    {
        a.append("HScale", QString::number(cur_text->getHScale()));
    }
    if (cur_text->getReadDirection() != 0)
    {
        a.append("ReadDirection", QString::number(cur_text->getReadDirection()));
    }
    qDebug() << "weight ------" << cur_text->getWeight();
    if (cur_text->getWeight() != 400)
    {
        a.append("Weight", QString::number(cur_text->getWeight()));
    }
    if (cur_text->getItalic())
    {
        a.append("Italic", "true");
    }
    return a;
}

QXmlStreamAttributes getAttributes(TextCode * cur_textcode)
{
    QXmlStreamAttributes a;

    a.append("X", QString::number(cur_textcode->getX()));
    a.append("Y", QString::number(cur_textcode->getY()));
    if (!cur_textcode->getDeltaX().isNull())
    {
        a.append("DeltaX", cur_textcode->getDeltaX().getAllContent());
    }
    if (!cur_textcode->getDeltaY().isNull())
    {
        a.append("DeltaY", cur_textcode->getDeltaY().getAllContent());
    }
    return a;
}

QXmlStreamAttributes getAttributes(CT_Path * cur_path)
{
    QXmlStreamAttributes a;

    //Stroke属性为非默认值false时才显示。下同
    if (!cur_path->getStroke())
    {
        a.append("Stroke", "false");
    }
    if (cur_path->getFill())
    {
        a.append("Fill", "true");
    }
    if (cur_path->getRule() != "NonZero")
    {
        a.append("Rule", cur_path->getRule());
    }
    return a;
}

QXmlStreamAttributes getAttributes(CT_Image * cur_image)
{

    QXmlStreamAttributes a;
    if (!cur_image->getResourceID().isNull())
    {
        a.append("ResourceID", QString::number(cur_image->getResourceID().getRefID()));
    }
    else
    {
        qDebug("CT_Image类型数据缺少必要的属性ResourceID");
    }

    if (!cur_image->getSubstitution().isNull())
    {
        a.append("Substitution", QString::number(cur_image->getSubstitution().getRefID()));
    }

    // 图片对象似乎需要插入CTM
    double width = cur_image->boundary.getDeltaX();
    double height = cur_image->boundary.getDeltaY();

    a.append(
                "CTM",
                QString::number(width) + " 0 0 "
                +QString::number(height) + " 0 0");

    return a;
}

QXmlStreamAttributes getAttributes(CT_ColorSpace * cur_colorspace)
{
    QXmlStreamAttributes a;
    if (!cur_colorspace->getType().isNull())
    {
        a.append("Type", cur_colorspace->getType());
    }
    else
    {
        qDebug("CT_ColorSpace类型数据缺少必要的属性Type");
    }
    if (cur_colorspace->getBitsPerComponent() != 8)
    {
        a.append("BitsPerComponent", QString::number(cur_colorspace->getBitsPerComponent()));
    }
    //    qDebug() << "Colorspace Profile: " << cur_colorspace->getProfile();
    //    qDebug() << "Colorspace Profile abs_path: " << cur_colorspace->getProfile().getPath();
    //    qDebug() << "Colorspace Profile is NUll: " << cur_colorspace->getProfile().isNull();
    if (!cur_colorspace->getProfile().isNull())
    {
        ST_Loc p = cur_colorspace->getProfile();
        a.append("Profile", p.getRelativePath());
    }
    return a;
}

QXmlStreamAttributes getAttributes(CT_DrawParam * cur_draw_param)
{
    QXmlStreamAttributes a;
    if (!cur_draw_param->getRelative().isNull())
    {
        ST_RefID p = cur_draw_param->getRelative();
        a.append("Relative", QString::number(p.getRefID()));
    }
    if (abs(cur_draw_param->getLineWidth() - 0.353) > 0.0000001)
    {
        a.append("LineWidth", QString::number(cur_draw_param->getLineWidth()));
    }
    if (cur_draw_param->getJoin() != "Miter")
    {
        a.append("Join", cur_draw_param->getJoin());
    }
    if (cur_draw_param->getCap() != "Butt")
    {
        a.append("Cap", cur_draw_param->getCap());
    }
    if (abs(cur_draw_param->getDashOffset() - 0.0) > 0.0000001)
    {
        a.append("DashOffset", QString::number(cur_draw_param->getDashOffset()));
    }
    if (!cur_draw_param->getDashPattern().isNull())
    {
        a.append("DashPattern", cur_draw_param->getDashPattern().getAllContent());
    }
    if (abs(cur_draw_param->getMiterLimit() - 3.528) > 0.0000001)
    {
        a.append("MiterLimit", QString::number(cur_draw_param->getMiterLimit()));
    }
    return a;
}

QXmlStreamAttributes getAttributes(CT_Font * cur_font)
{
    QXmlStreamAttributes a;
    if (cur_font->getFontName().length() != 0)
    {
        a.append("FontName", cur_font->getFontName());
    }
    else
    {
        qDebug("CT_Font类型数据缺少必要的属性FontName");
    }
    if (cur_font->getFamilyName().length() != 0)
    {
        a.append("FamilyName", cur_font->getFamilyName());
    }
    if (cur_font->getCharset() != "GB18030")
    {
        a.append("Charset", cur_font->getCharset());
    }
    if (cur_font->getItalic())
    {
        a.append("Italic", "true");
    }
    if (cur_font->getBold())
    {
        a.append("Bold", "true");
    }
    if (cur_font->getSerif())
    {
        a.append("Serif", "true");
    }
    if (cur_font->getFixedWidth())
    {
        a.append("FixedWidth", "true");
    }
    return a;
}

void OFDWriter::makePath(ST_Loc path)
{
    QString path_str = path.getPath();
    int n = 0;
    while (path_str[path_str.length() - n - 1] != '/')
        n++;
    path_str.chop(n);
    qDebug() << path_str;
    if (!QDir().mkpath(path_str))
    {
        throw WritingFileException("无法创建文件路径: " + path.getPath());
    }
}

QXmlStreamAttributes getAttributes(CT_MultiMedia *cur_multiMedia)
{
    QXmlStreamAttributes a;
    if(cur_multiMedia->Format.size() != 0)
    {
        a.append("Format", cur_multiMedia->Format);
    }

    if(cur_multiMedia->Type.size() != 0)
    {
        a.append("Type" ,cur_multiMedia->Type);
    }

//    if(cur_multiMedia->MediaFile.size() != 0)
//    {
//        a.append("MediaFile",cur_multiMedia->MediaFile);
//    }

    return a;
}
