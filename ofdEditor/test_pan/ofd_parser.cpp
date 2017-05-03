#include "ofd_parser.h"
#include <QDebug>

OFDParser::OFDParser(QString _path) : current_path("OFD", _path) {
    document = new QDomDocument();
    data = readOFD();
}

void OFDParser::openFile() {
    QFile ofd_file(current_path.getPath());
    //qDebug() << "At the very beginning: " << current_path << endl;
    if (!ofd_file.open(QFile::ReadOnly | QFile::Text)) {    //检查文件是否存在&正常打开
            //出现异常！！！to be implemented
        qDebug() << "xml文件打开出现错误。" << endl;
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
        qDebug() << "xml解析出现错误。" << endl;
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
                if (!(t = new_docinfo.firstChildElement("ofd:DocID")).isNull())
                    docinfo_data->doc_id = t.text();
                if (!(t = new_docinfo.firstChildElement("ofd:CreationDate")).isNull())
                    docinfo_data->creation_date = t.text();
                if (!(t = new_docinfo.firstChildElement("ofd:ModDate")).isNull())
                    docinfo_data->mod_date = t.text();
                if (!(t = new_docinfo.firstChildElement("ofd:Title")).isNull())
                    docinfo_data->title = t.text();
                if (!(t = new_docinfo.firstChildElement("ofd:Author")).isNull())
                    docinfo_data->author = t.text();
                if (!(t = new_docinfo.firstChildElement("ofd:Subject")).isNull())
                    docinfo_data->subject = t.text();
                if (!(t = new_docinfo.firstChildElement("ofd:Creator")).isNull())
                    docinfo_data->creator = t.text();
                if (!(t = new_docinfo.firstChildElement("ofd:CreatorVersion")).isNull())
                    docinfo_data->creator_version = t.text();
                if (!(t = new_docinfo.firstChildElement("ofd:DocUsage")).isNull())
                    docinfo_data->doc_usage = t.text();
            } else {
                //Error
                abort();
            }
            QDomElement new_docroot = new_docbody.firstChildElement("ofd:DocRoot");
            if (!new_docroot.isNull()) {
                //qDebug() << current_path << endl;;
                ST_Loc p("Document", new_docroot.text(), current_path);
                docbody_data->doc_root = p;
                //qDebug() << docbody_data->doc_root << endl;
            } else {
                //qDebug() << "非法的OFD文档格式。" << endl;
                //Error
                abort();
            }
            new_docbody = new_ofd.nextSiblingElement("ofd:DocBody");
        }
    } else {
        qDebug() << "非法的OFD文档格式。" << endl;
        abort();
    }
    if (ofd_data) {
        for (int i = 0; i < ofd_data->docbodys->length(); i++) {
            //qDebug() << ofd_data->docbodys->at(i)->doc_root << endl;
            //qDebug() << current_path << endl;
            current_path = ofd_data->docbodys->at(i)->doc_root;
            //qDebug() << current_path << endl;
            ofd_data->docs->push_back(readDocument());
        }
    }
    return ofd_data;
}

Document * OFDParser::readDocument(){
    //qDebug() << current_path << endl;
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
                commondata_data->max_unit_id = t.text().toInt();
            } else {
                //Error
            }

            if (!(t = new_commondata.firstChildElement("ofd:PageArea")).isNull()) {
                CT_PageArea *pagearea_data = new CT_PageArea();
                commondata_data->page_area = pagearea_data;
                readPageArea(pageare_data, t);
            } else {
                //Error
            }

            t = new_commondata.firstChildElement("ofd:PublicRes");
            while (!t.isNull()) {
                ST_Loc p("PublicRes", t.text(), current_path);
                commondata_data->public_res->push_back(p);
                t = new_commondata.nextSiblingElement("ofd:PublicRes");
            }

            if (!(t = new_commondata.firstChildElement("ofd:DefaultCS")).isNull()) {
                //to be implemented
            }

        } else {
            //Error
        }

        QDomElement new_pages = new_document.firstChildElement("ofd:Pages");
        CT_Pages *pages_data;
        if (!new_pages.isNull()) {
            pages_data = new CT_Pages();
            QDomElement new_page = new_pages.firstChildElement("ofd:Page");
            //qDebug() << new_page.tagName() << endl;
            while (!new_page.isNull()) {
                Page *p = new Page();
                ST_ID i(new_page.attribute("ID").toLong());
                p->id = i;
                ST_Loc path("Page", new_page.attribute("BaseLoc"), current_path);
                p->base_loc = path;
                pages_data->pages->push_back(p);
                new_page = new_page.nextSiblingElement("ofd:Page");
            }
        } else {
            //Error
            abort();
        }
        document_data->pages = pages_data;

        QDomElement new_outlines = new_document.firstChildElement("ofd:Outlines");
        if (!new_outlines.isNull()) {
            //to be implemented
        }

        QDomElement new_customtags = new_document.firstChildElement("ofd:CustomTags");
        if (!new_customtags.isNull()) {
            ST_Loc p("CustomTags", new_customtags.text(), current_path);
            document_data->custom_tags = p;
        }

        QDomElement new_extensions = new_document.firstChildElement("ofd:Extensions");
        if (!new_extensions.isNull()) {
            //to be implemented
        }

        QDomElement new_annotations = new_document.firstChildElement("ofd:Annotations");
        if (!new_annotations.isNull()) {
            //to be implemented
        }

    } else {
        //Error
        abort();
    }

    for (int i = 0; i < document_data->pages->pages->size(); i++) {
        current_path = document_data->pages->pages->at(i)->base_loc;
        readPage(document_data->pages->pages->at(i));
    }

    for (int i = 0; i < document_data->common_data->public_res->size(); i++) {
        current_path = document_data->common_data->public_res->at(i);
        //to be implemented
    }
    return document_data;
}

void OFDParser::readPage(Page * page_data) {
    openFile();
    QDomElement new_page = document->firstChildElement("ofd:Page");
    if (!new_page.isNull()) {
        QDomElement t;
        if (!(t = new_page.firstChildElement("ofd:Area").isnull())) {
            CT_PageArea * area_data;
            readPageArea(area_data, t);
            page_data->area = area_data;
        }

        t = new_commondata.firstChildElement("ofd:PageRes");
        while (!t.isNull()) {
            ST_Loc p("PageRes", t.text(), current_path);
            page_data->page_res->push_back(p);
            t = new_commondata.nextSiblingElement("ofd:PageRes");
        }

        if (!(t = new_page.firstChildElement("ofd:Content").isnull())) {
            QDomElement new_layer = t.firstChildElement("ofd:Layer");
            while (!new_layer.isNull()) {
                CT_Layer * layer_data = new CT_Layer();
                if (new_layer.hasAttribute("ID")) {
                    ST_ID i(new_layer.attribute("ID").toInt());
                    layer_data->setID(i);
                } else {
                    //Error
                    abort();
                }
                if (new_layer.hasAttribute("DrawParam")) {
                    ST_RefID ri(new_layer.attribute("DrawParam").toInt());
                    layer_data->draw_param = ri;
                }
                QDomElement t;
                t = new_layer.firstChildElement("ofd:TextObject");
                while (!t.isNull()) {
                    CT_Text * text_data = new CT_Text();
                    layer_data->text_object->push_back(text_data);

                    if (t.hasAttribute("ID")) {
                        ST_ID i(t.attribute("ID").toInt());
                        text_data->setID(i);
                    } else {
                        //Error
                        abort();
                    }

                    if (t.hasAttribute("Font")) {
                        ST_RefID ri(t.attribute("Font").toInt());
                        text_data->font(ri);
                    } else {
                        //Error
                        abort();
                    }

                    if (t.hasAttribute("Size")) {
                        text_data->size(t.attribute("Size").toDouble());
                    } else {
                        //Error
                        abort();
                    }

                    if (t.hasAttribute("Boundary")) {
                        QStringList values = t.attribute("Boundary").split(" ");
                        if (values.size() == 4) {
                            text_data->boundary = St_Box(values[0].toDouble(), values[1].toDouble(), values[2].toDouble(), values[3].toDouble());
                        }
                        else {
                            //Error
                            abort();
                        }
                    } else {
                        //Error
                        abort();
                    }

                    //many optional attributes to be implemented

                    QDomElement t2;
                    if (!(t2 = t.firstChildElement("FillColor").isNull())) {

                    }
                }

            }
        }
    } else {
        //Error
        abort();
    }
}

void OFDParser::readPageArea(CT_PageArea * data, QDomElement & root_node) {
    QDomeElement t;
    if (!(t = root_node.firstChildElement("ofd:PhysicalBox")).isNull()) {
        QStringList values = t.text().split(" ");
        //qDebug() << values[0] << values[1] << values[2] << endl;
        if (values.size() == 4) {
            data->physical_box = St_Box(values[0].toDouble(), values[1].toDouble(), values[2].toDouble(), values[3].toDouble());
        }
        else {
            //Error
            abort();
        }
    } else {
        //Error
    }

    if (!(t = root_node.firstChildElement("ofd:ApplicationBox")).isNull()) {
        QStringList values = t.text().split(" ");
        if (values.size() == 4)
            data->application_box = St_Box(values[0].toDouble(), values[1].toDouble(), values[2].toDouble(), values[3].toDouble());
        else {
            //Error
            abort();
        }
    }

    if (!(t = root_node.firstChildElement("ofd:ContentBox")).isNull()) {
        QStringList values = t.text().split(" ");
        if (values.size() == 4)
            data->content_box = St_Box(values[0].toDouble(), values[1].toDouble(), values[2].toDouble(), values[3].toDouble());
        else {
            //Error
            abort();
        }
    }

    if (!(t = root_node.firstChildElement("ofd:BleedBox")).isNull()) {
        QStringList values = t.text().split(" ");
        if (values.size() == 4)
            data->bleed_box = St_Box(values[0].toDouble(), values[1].toDouble(), values[2].toDouble(), values[3].toDouble());
        else {
            //Error
            abort();
        }
    }
}
