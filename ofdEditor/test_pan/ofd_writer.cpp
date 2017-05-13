#include "ofd_writer.h"
#include <QDebug>

OFDWriter::OFDWriter(OFD * _data, QString _path) : data(_data),
                         current_path("OFD", _path) {
    writeOFD();
}

void OFDWriter::createFile() {
    current_file = new QFile(current_path.getPath() + "OFD.xml");
    qDebug() << current_path.getPath() + "OFD.xml";

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
            writer.writeTextElement("ofd:DocRoot", cur_docbody->getDocRoot().getRelativePath(root));
        }
        writer.writeEndElement();
    }
    //正文结束
    writer.writeEndDocument();
    current_file->close();
    //修改current_file
    for (int i = 0; i < data->getDocBodies()->size(); i++) {
        ST_Loc p("Document", data->getDocBodies()->at(i)->getDocRoot().getPath());
        current_path = p;
        writeDocument(data->getDocuments()->at(i));
    }
}

void OFDWriter::writeDocument(Document * data) {
    createFile();
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
            QString str;
            writer.writeTextElement("ofd:MaxUnitID", str.setNum(cur_common_data->getMaxUnitID().getID()));
        } else {
            //Error!
            abort();
        }
        if (cur_common_data->getPageArea()) {
            writer.writeStartElement("ofd:PageArea");
            CT_PageArea * cur_page_area = cur_common_data->getPageArea();
            if (!cur_page_area->getPhysicalBox().isNull()) {
                ST_Box b = cur_page_area->getPhysicalBox();
                QString s1;     s1.setNum(b.getX());
                QString s2;     s2.setNum(b.getY());
                QString s3;     s3.setNum(b.getDeltaX());
                QString s4;     s4.setNum(b.getDeltaY());
                writer.writeTextElement("ofd:PhysicalBox", s1 + " " + s2 + " " + s3 + " " + s4);
            } else {
                //Error!
                abort();
            }
            if (!cur_page_area->getApplicationBox().isNull()) {
                ST_Box b = cur_page_area->getPhysicalBox();
                QString s1;     s1.setNum(b.getX());
                QString s2;     s2.setNum(b.getY());
                QString s3;     s3.setNum(b.getDeltaX());
                QString s4;     s4.setNum(b.getDeltaY());
                writer.writeTextElement("ofd:PhysicalBox", s1 + " " + s2 + " " + s3 + " " + s4);
            }
            if (!cur_page_area->getContentBox().isNull()) {

            }
            if (!cur_page_area->getBleedBox().isNull()) {

            }
            writer.writeEndElement();   //ofd:PageArea
        } else {
            //Error!
            abort();
        }
        writer.writeEndElement();   //ofd:CommonData
    } else {
        //Error!
        abort();
    }




    //正文结束
    writer.writeEndDocument();
    current_file->close();
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
