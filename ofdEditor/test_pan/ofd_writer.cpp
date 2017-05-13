#include "ofd_writer.h"
#include <QDebug>

OFDWriter::OFDWriter(OFD * _data, QString _path) : data(_data),
                         current_path("OFD", _path + "OFD.xml") {
    writeOFD();
}

void OFDWriter::createFile() {
    current_file = new QFile(current_path.getPath());
    qDebug() << current_path.getPath() << endl;
    if (!current_file->open(QFile::WriteOnly | QFile::Text)) {
        qDebug() << "Error: Cannot write file.";
        abort();
    }
    writer.setDevice(current_file);
}

void OFDWriter::writeOFD() {
    createFile();
    writer.writeStartDocument();
    writer.setAutoFormatting(true);
    //进入正文
    writer.writeStartElement("ofd:OFD");
    writer.writeAttribute("xmlns:ofd", "http://www.ofdspec.org");
    writer.writeAttributes(getAttributes(data));
    //写DocBody
    for (int i = 0; i < data->getDocBodies()->size(); i++) {
        DocBody * cur_docbody = data->getDocBodies()->at(i);
        writer.writeStartElement("ofd:DocBody");
        CT_DocInfo * cur_docinfo = cur_docbody->getDocInfo();
        writer.writeStartElement("ofd:DocInfo");
        //写DocInfo
        if (!cur_docinfo->getDocID().isNull()) {
            writer.writeTextElement("ofd:DocID", cur_docinfo->getDocID());
        } else {
            //Error!
            abort();
        }
        if (!cur_docinfo->getTitle().isNull()) {
            writer.writeTextElement("ofd:Title", cur_docinfo->getTitle());
        }
        if (!cur_docinfo->getAuthor().isNull()) {
            writer.writeTextElement("ofd:Author", cur_docinfo->getAuthor());
        }
        if (!cur_docinfo->getSubject().isNull()) {
            writer.writeTextElement("ofd:Subject", cur_docinfo->getSubject());
        }
        if (!cur_docinfo->getAbstract().isNull()) {
            writer.writeTextElement("ofd:Abstract", cur_docinfo->getAbstract());
        }
        if (!cur_docinfo->getCreationDate().isNull()) {
            writer.writeTextElement("ofd:CreationDate", cur_docinfo->getCreationDate());
        }
        if (!cur_docinfo->getModDate().isNull()) {
            writer.writeTextElement("ofd:ModDate", cur_docinfo->getModDate());
        }
        if (!cur_docinfo->getDocUsage().isNull()) {
            writer.writeTextElement("ofd:DocUsage", cur_docinfo->getDocUsage());
        }
        //Cover, keywords to be implmented
        if (!cur_docinfo->getCreator().isNull()) {
            writer.writeTextElement("ofd:Creator", cur_docinfo->getCreator());
        }
        if (!cur_docinfo->getCreatorVersion().isNull()) {
            writer.writeTextElement("ofd:CreatorVersion", cur_docinfo->getCreatorVersion());
        }
        //写CustomDatas
        QVector<QStringList> * cur_custom_datas = cur_docinfo->getCustomDatas();
        if (cur_custom_datas->size() != 0) {
            writer.writeStartElement("ofd:CustomDatas");
            for (int i = 0; i < cur_custom_datas->size(); i++) {
                writer.writeStartElement("ofd:CustomData");
                writer.writeAttribute("Name", cur_custom_datas->at(i).at(0));
                writer.writeCharacters(cur_custom_datas->at(i).at(1));
                writer.writeEndElement();
            }
            writer.writeEndElement();
        }
        writer.writeEndElement();
        //写DocRoot
        if (!cur_docbody->getDocRoot().isNull()) {
            ST_Loc root("", data->getRootPath().getPath().remove("OFD.xml"));
            writer.writeTextElement("ofd:DocRoot", cur_docbody->getDocRoot().getRelativePath());
        }
        writer.writeEndElement();
    }
    //正文结束
    writer.writeEndDocument();
    current_file->close();
    //修改current_file
    ST_Loc cur_path = current_path;
    for (int i = 0; i < data->getDocBodies()->size(); i++) {
        qDebug() << "To write Document.xml..." << endl;
        qDebug() << "cur_path = " << cur_path << endl;
        ST_Loc p("Document",
                 data->getDocBodies()->at(i)->getDocRoot().getRelativePath(), cur_path);
        current_path = p;
        qDebug() << "current_path = " << current_path << endl;
        QDir dir(cur_path.getPath());

        QString str;
        dir.mkdir("Doc_" + str.setNum(i));
        writeDocument(data->getDocuments()->at(i));
    }
}

void OFDWriter::writeDocument(Document * data) {
    createFile();
    //qDebug() << "Checkpoint 1 reached.";
    writer.writeStartDocument();
    writer.setAutoFormatting(true);
    //进入正文
    writer.writeStartElement("ofd:Document");
    writer.writeAttribute("xmlns:ofd", "http://www.ofdspec.org");
    //写CommonData
    if (data->getCommonData() != nullptr) {
        writer.writeStartElement("ofd:CommonData");
        CT_CommonData * cur_common_data = data->getCommonData();
        if (!cur_common_data->getMaxUnitID().isNull()) {
            writer.writeTextElement("ofd:MaxUnitID",
                                    QString::number(cur_common_data->getMaxUnitID().getID()));
        } else {
            //Error!
            abort();
        }
        if (cur_common_data->getPageArea()) {
            writer.writeStartElement("ofd:PageArea");
            writePageArea(cur_common_data->getPageArea(), writer);
            writer.writeEndElement();   //ofd:PageArea
        } else {
            //Error!
            abort();
        }

        for (int i = 0; i < data->getCommonData()->getPublicRes()->size(); i++) {
            ST_Loc p = data->getCommonData()->getPublicRes()->at(i);
            writer.writeTextElement("ofd:PublicRes", p.getRelativePath());
        }
        //TemplatePage & DefaultCS to be implemented
        writer.writeEndElement();   //ofd:CommonData
    } else {
        //Error!
        abort();
    }
    //写Pages
    if (data->getPages()) {
        CT_Pages * cur_pages = data->getPages();
        writer.writeStartElement("ofd:Pages");
        for (int i = 0; i < cur_pages->getPages()->size(); i++) {
            Page * cur_page = cur_pages->getPages()->at(i);
            writer.writeStartElement("ofd:Page");
            writeBase(cur_page, writer);
            writer.writeAttribute("BaseLoc", cur_page->getBaseLoc().getRelativePath());
            writer.writeEndElement();
        }
        writer.writeEndElement();   //ofd:Pages
    } else {
        //Error!
        abort();
    }
    //写CustomTags
    if (!data->getCustomTags().isNull()) {
        writer.writeTextElement("ofd:CustomTags", data->getCustomTags().getRelativePath());
    }
    //Outlines, extensions, annotations to be implemented
    //正文结束
    writer.writeEndDocument();
    current_file->close();
    ST_Loc cur_path = current_path;
    for (int i = 0; i < data->getPages()->getPages()->size(); i++) {
        qDebug() << "To write Page.xml..." << endl;
        qDebug() << "cur_path = " << cur_path << endl;
        if (i == 0) {
            QDir dir(cur_path.getPath().remove("Document.xml"));
            dir.mkdir("Pages");
        }
        ST_Loc p("Page", data->getPages()->getPages()->at(i)->
                 getBaseLoc().getRelativePath(), cur_path);
        current_path = p;
        qDebug() << "current_path = " << current_path << endl;
        QDir dir(cur_path.getPath().remove("Document.xml").append("Pages\\"));
        dir.mkdir("Page_" + QString::number(i));
        writePage(data->getPages()->getPages()->at(i));
    }
//    for (int i = 0; i < data->getPublicRes()->size(); i++) {
//        ST_Loc cur_path = current_path;
//        ST_Loc p("PublicRes", data->getPublicRes()->at(i)->getBaseLoc().getRelativePath(), cur_path);
//        current_path = p;
//        writePublicRes(data->getPublicRes()->at(i));
//    }
    //write CustomTags to be implemented
}

void OFDWriter::writePage(Page * data) {
    createFile();
    writer.writeStartDocument();
    writer.setAutoFormatting(true);
    //进入正文
    writer.writeStartElement("ofd:Page");
    writer.writeAttribute("xmlns:ofd", "http://www.ofdspec.org");
    //Template to be implemented
    //写Area
    if (data->getArea()) {
        writer.writeStartElement("ofd:Area");
        writePageArea(data->getArea(), writer);
        wrier.writeEndElement();    //ofd:Area
    }
    //写PageRes
    for (int i = 0; i < data->getPageResLocations()->size(); i++) {
        writer.writeTextElement("ofd:PageRes", data->getPageResLocations()->at(i));
    }
    //写Content
    if (data->getContent()->size()) {
        writer.writeStartElement("ofd:Content");
        for (int i = 0; i < data->getContent()->size(); i++) {
            CT_Layer * cur_layer = data->getContent()->at(i);
            writer.writeStartElement("ofd:Layer");
            writeBase(cur_layer, writer);
            writer.writeAttributes(getAttributes(cur_layer));
            //写文档页面的内容（QGraphicUnits的子类集合）
            writePageBlock(cur_layer, writer, true);
        }
        writer.writeEndElement();   //ofd:Content
    }
    writer.writeEndElement();   //ofd:Page
    //正文结束
    writer.writeEndDocument();
    current_file->close();
}

void OFDWriter::writePublicRes(Res * data) {
    createFile();
}

void OFDWriter::writePageArea(CT_PageArea * cur_page_area,
                              QXmlStreamWriter & writer) {
    if (!cur_page_area->getPhysicalBox().isNull()) {
        ST_Box b = cur_page_area->getPhysicalBox();
        writer.writeTextElement("ofd:PhysicalBox",
                                QString::number(b.getX()) + " " +
                                QString::number(b.getY()) + " " +
                                QString::number(b.getDeltaX())+ " " +
                                QString::number(b.getDeltaY()));
    } else {
        //Error!
        abort();
    }
    if (!cur_page_area->getApplicationBox().isNull()) {
        ST_Box b = cur_page_area->getApplicationBox();
        writer.writeTextElement("ofd:ApplicationBox",
                                QString::number(b.getX()) + " " +
                                QString::number(b.getY()) + " " +
                                QString::number(b.getDeltaX())+ " " +
                                QString::number(b.getDeltaY()));
    }
    if (!cur_page_area->getContentBox().isNull()) {
        ST_Box b = cur_page_area->getContentBox();
        writer.writeTextElement("ofd:ContentBox",
                                QString::number(b.getX()) + " " +
                                QString::number(b.getY()) + " " +
                                QString::number(b.getDeltaX())+ " " +
                                QString::number(b.getDeltaY()));
    }
    if (!cur_page_area->getBleedBox().isNull()) {
        ST_Box b = cur_page_area->getBleedBox();
        writer.writeTextElement("ofd:BleedBox",
                                QString::number(b.getX()) + " " +
                                QString::number(b.getY()) + " " +
                                QString::number(b.getDeltaX())+ " " +
                                QString::number(b.getDeltaY()));
    }
}

void OFDWriter::writePageBlock(CT_PageBlock * cur_page_block,
                               QXmlStreamWriter & writer, bool is_layer) {
    writeBase(cur_page_block, writer);
    //文字内容
    for (int i = 0; i < cur_page_block->getTextObject()->size(); i++) {
        CT_Text * cur_text = cur_page_block->getTextObject()->at(i);
        writer.writeStartElement("ofd:TextObject");
        writeTextObject(cur_text, writer);
        writer.writeEndElement();   //ofd:TextObject
    }
    //矢量图内容
    for (int i = 0; i < cur_page_block->getPathObject()->size(); i++) {

    }
    //位图内容
    for (int i = 0; i < cur_page_block->getImageObject()->size(); i++) {

    }
    //PageBlock嵌套
    for (int i = 0; i < cur_page_block->getPageBlock()->size(); i++) {

    }
}

void OFDWriter::writeTextObject(CT_Text * cur_text, QXmlStreamWriter & writer) {
    writeGraphicUnit(cur_text, writer);
    writer.writeAttributes(getAttributes(cur_text));
}

void OFDWriter::writeGraphicUnit(CT_GraphicUnit * cur_graphic_unit,
                                 QXmlStreamWriter & writer) {
    writeBase(cur_graphic_unit, writer);
    writer.writeAttributes(getAttributes(cur_graphic_unit));

    if (cur_graphic_unit->getFillColor()) {
        writer.writeStartElement("ofd:FillColor");
        writeColor(cur_graphic_unit->getFillColor(), writer);
        writer.writeEndElement();   //ofd:FillColor
    }

    if (cur_graphic_unit->getStrokeColor()) {
        writer.writeStartElement("ofd:StrokeColor");
        writeColor(cur_graphic_unit->getStrokeColor(), writer);
        writer.writeEndElement();   //ofd:StrokeColor
    }
}

void OFDWriter::writeColor(CT_Color * cur_color, QXmlStreamWriter & writer) {
    writer.writeAttributes(getAttributes(cur_color));
    //Pattern and AxialShd / RadialShd to be implemented
}

void OFDWriter::writeBase(CT_Base * cur_base,
                          QXmlStreamWriter & writer) {
    if (!cur_base->getID().isNull()) {
        writer.writeAttribute("ID", cur_base->getID());
    } else {
        //Error!
        abort();
    }
}

QXmlStreamAttributes getAttributes(OFD * data) {
    QXmlStreamAttributes a;
    if (!data->getDocType().isNull()) {
        a.append("DocType", data->getDocType());
    } else {
        //Error
        abort();
    }
    if (!data->getOfdVersion().isNull()) {
        a.append("Version", data->getOfdVersion());
    } else {
        //Error
        abort();
    }
    return a;
}

QXmlStreamAttributes getAttributes(CT_Layer * cur_layer) {
    QXmlStreamAttributes a;

    switch(cur_layer->getType()) {
    case CT_Layer::Body :
        break;
    case CT_Layer::Foreground :
        a.append("Type", "Foreground");
        break;
    case CT_Layer::Background :
        a.append("Type", "Background");
        break;
    }

    if (!cur_layer->getDrawParam().isNull()) {
        a.append("DrawParam",QString::number(cur_layer->getDrawParam().getRefID()));
    }
}

QXmlStreamAttributes getAttributes(CT_GraphicUnit * cur_graphic_unit) {
    QXmlStreamAttributes a;

    if (!cur_graphic_unit->getBoundary().isNull()) {
        ST_Box b = cur_graphic_unit->getBoundary();
        a.append("Boundary",
                 QString::number(b.getX()) + " " +
                 QString::number(b.getY()) + " " +
                 QString::number(b.getDeltaX()) + " " +
                 QString::number(b.getDeltaY()));
    } else {
        //Error
        abort();
    }

    if (!cur_graphic_unit->getName().isNull()) {
        a.append("Name", cur_graphic_unit->getName());
    }

    //当Visible为false时才写出
    if (!cur_graphic_unit->getVisible()) {
        a.append("Visible", "false");
    }

    //CTM to be implemented

    if (!cur_graphic_unit->getDrawParam.isNull()) {
        a.append("DrawParam", QString::number(cur_graphic_unit->getDrawParam()));
    }

    //Other attributes to be implemented
    return a;

}

QXmlStreamAttributes getAttributes(CT_Color *cur_color) {
    QXmlStreamAttributes a;

    if (!cur_color->getValue().isNull()) {
        ST_Array & v = cur_color->getValue();
        QString str;
        for (int i = 0; i < v.size(); i++) {
            if (i)  str.append(" ");
            str.append(v[i]);
        }
        a.append("Value", str);
    }
    if (cur_color->indexUsed()) {
        a.append("Index", cur_color->getIndex());
    }
    if (!cur_color->getColorSpace().isNull()) {
        a.append("ColorSpace", QString::number(cur_color->getColorSpace().getRefID()));
    }
    //只有当Alpha值为非默认的255时才显示
    if (cur_color->getAlpha() != 255) {
        a.append("Alpha", QString::number(cur_color->getAlpha()));
    }

    return a;
}


QXmlStreamAttributes getAttributes(CT_Text * cur_text) {
    QXmlStreamAttributes a;

}
