#include "ofd_parser.h"
#include <QDebug>

OFDParser::OFDParser(QString _path) : current_path("OFD", _path) {
    document = new QDomDocument();
    data = readOFD();
}

void OFDParser::openFile() {
    QFile ofd_file(current_path.getPath());
    if (!ofd_file.open(QFile::ReadOnly | QFile::Text)) {    //检查文件是否存在&正常打开
            //出现异常！！！to be implemented
        qDebug() << "ofd.xml文件打开出现错误。" << endl;
        abort();
    }
    if (document->setContent(&ofd_file,         //解析OFD文档并将树状内容存在document文件中
                             false,
                             &error_msg,
                             &error_line,
                             &error_column)) {
        ofd_file.close();
    } else {
        //出现异常！！！to be implemented
        qDebug() << "ofd.xml解析出现错误。" << endl;
        abort();
    }
}

OFD * OFDParser::readOFD() {
    openFile();
    QDomElement new_ofd = document->firstChildElement("ofd:OFD");
    OFD *ofd_data = nullptr;
    if (!new_ofd.isNull()) {
        ofd_data = new OFD();
        ofd_data->doc_type = new_ofd.attribute("DocType");
        ofd_data->version = new_ofd.attribute("Version");
        QDomElement new_docbody = new_ofd.firstChildElement("ofd:DocBody");

        while (!new_docbody.isNull()) {
            DocBody * docbody_data = new DocBody();
            ofd_data->docbodys->push_back(docbody_data);

            QDomElement new_docinfo = new_docbody.firstChildElement("ofd:DocInfo");
            if (!new_docinfo.isNull()) {
                CT_DocInfo *docinfo_data = new CT_DocInfo();
                docbody_data->doc_info = docinfo_data;

                QDomElement t;
                if (!(t = new_docinfo.firstChildElement("ofd:DocId")).isNull())
                    docinfo_data->doc_id = t.nodeName();
                if (!(t = new_docinfo.firstChildElement("ofd:CreationDate")).isNull())
                    docinfo_data->creation_date = t.nodeName();
                if (!(t = new_docinfo.firstChildElement("ofd:ModDate")).isNull())
                    docinfo_data->mod_date = t.nodeName();
                if (!(t = new_docinfo.firstChildElement("ofd:Title")).isNull())
                    docinfo_data->title = t.nodeName();
                if (!(t = new_docinfo.firstChildElement("ofd:Author")).isNull())
                    docinfo_data->author = t.nodeName();
                if (!(t = new_docinfo.firstChildElement("ofd:Subject")).isNull())
                    docinfo_data->subject = t.nodeName();
                if (!(t = new_docinfo.firstChildElement("ofd:Creator")).isNull())
                    docinfo_data->creator = t.nodeName();
                if (!(t = new_docinfo.firstChildElement("ofd:CreatorVersion")).isNull())
                    docinfo_data->creator_version = t.nodeName();
                if (!(t = new_docinfo.firstChildElement("ofd:DocUsage")).isNull())
                    docinfo_data->doc_usage = t.nodeName();
            } else {
                //Error
                abort();
            }
            QDomElement new_docroot = new_docbody.firstChildElement("ofd:DocRoot");
            if (!new_docroot.isNull()) {
                ST_Loc p("Document", new_docroot.nodeValue(), current_path);
                docbody_data->doc_root = p;
            } else {
                qDebug() << "非法的OFD文档格式。" << endl;
                //Error
                abort();
            }
            new_docbody = new_ofd.nextSiblingElement("ofd:DocBody");
        }
    } else {
        qDebug() << "非法的OFD文档格式。" << endl;
        abort();
    }

    for (int i = 0; i < ofd_data->docbodys->length; i++) {
        current_path = ofd_data->docbodys->at(i)->doc_root;
        ofd_data->docs->push_back(readDocument());
    }
    return ofd_data;
}

Document * OFDParser::readDocument(){
    openFile();
    QDomElement new_document = document->firstChildElement("ofd:Document");
    Document *document_data;
    if (!new_document.isNull()) {
        document_data = new Document();
        QDomElement new_commondata = new_document.firstChildElement("ofd:CommonData");
        if (!new_commondata.isNull()) {
            CT_CommonData * commondata_data = new CT_CommonData();
            document_data->common_data = commondata_data;

            QDomElement t;
            if (!(t = new_commondata.firstChildElement("ofd:MaxUnitID")).isNull()) {
                commondata_data->max_unit_id = t.nodeValue().toInt();
            } else {
                //Error
            }

            if (!(t = new_commondata.firstChildElement("ofd:PageArea")).isNull()) {
                CT_PageArea *pagearea_data = new CT_PageArea();
                QDomElement t2;
                if (!(t2 = t.firstChildElement("ofd:PhysicalBox")).isNull()) {
                    QStringList values = t2.nodeValue().split(" ");

                } else {
                    //Error
                }
            } else {
                //Error
            }
        } else {
            //Error
        }

        QDomElement new_pages = new_document.firstChildElement("ofd:Pages");
        if (!new_pages.isNull()) {

        } else {
            //Error
        }

        QDomElement new_outlines = new_document.firstChildElement("ofd:Outlines");
        if (!new_outlines.isNull()) {

        }

        QDomElement new_customtags = new_document.firstChildElement("ofd:CustomTags");
        if (!new_customtags.isNull()) {

        }

        QDomElement new_extensions = new_document.firstChildElement("ofd:Extensions");
        if (!new_extensions.isNull()) {

        }

        QDomElement new_annotations = new_document.firstChildElement("ofd:Annotations");
        if (!new_annotations.isNull()) {

        }

    } else {
        //Error
        abort();
    }
    return document_data;
}
