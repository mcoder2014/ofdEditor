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
//    qDebug() << "Entering readOFD module..." << endl;
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
                ST_Loc p("Document", new_docroot.text(), current_path);
                docbody_data->doc_root = p;
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
            current_path = ofd_data->docbodys->at(i)->doc_root;
            ofd_data->docs->push_back(readDocument());
        }
    }
    return ofd_data;
}

Document * OFDParser::readDocument(){
    openFile();
//    qDebug() << "Entering readDocument module..." << endl;
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
                readPageArea(pagearea_data, t);
            } else {
                //Error
            }

            t = new_commondata.firstChildElement("ofd:PublicRes");
            while (!t.isNull()) {
                ST_Loc p("PublicRes", t.text(), current_path);
                commondata_data->public_res->push_back(p);
                t = t.nextSiblingElement("ofd:PublicRes");
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
                p->setID(i);
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
    //qDebug() << document_data->common_data->public_res->size() << endl;
    for (int i = 0; i < document_data->common_data->public_res->size(); i++) {
        //qDebug() << "Start to read PublicRes..." << endl;
        current_path = document_data->common_data->public_res->at(i);
        Res * res_data = new Res();
        readResource(res_data);
        document_data->public_res->push_back(res_data);
    }
    return document_data;
}

void OFDParser::readPage(Page * page_data) {
    openFile();
//    qDebug() << "Entering readPage module..." << endl;
    QDomElement new_page = document->firstChildElement("ofd:Page");
    if (!new_page.isNull()) {
        QDomElement t;
        if (!(t = new_page.firstChildElement("ofd:Area")).isNull()) {
//            qDebug() << "Start to read PageArea..." << endl;
            CT_PageArea * area_data = new CT_PageArea();
            readPageArea(area_data, t);
            page_data->area = area_data;
        }

        t = new_page.firstChildElement("ofd:PageRes");
        while (!t.isNull()) {
            ST_Loc p("PageRes", t.text(), current_path);
            Res * new_res = new Res();
            current_path = p;
            readResource(new_res);
            page_data->page_res->push_back(new_res);
            t = t.nextSiblingElement("ofd:PageRes");
        }

        if (!(t = new_page.firstChildElement("ofd:Content")).isNull()) {
//            qDebug() << "Start to read Content..." << endl;
            QDomElement new_layer = t.firstChildElement("ofd:Layer");
            while (!new_layer.isNull()) {
                CT_Layer * layer_data = new CT_Layer();
                page_data->getContent()->push_back(layer_data);
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
                    //qDebug() << "Start to read TextObject..." << endl;
                    CT_Text * text_data = new CT_Text();
                    layer_data->text_object->push_back(text_data);
                    readGraphicUnit(text_data, t);
                    //Read TextObject attributes
                    if (t.hasAttribute("Font")) {
                        ST_RefID ri(t.attribute("Font").toInt());
                        text_data->font = ri;
                    } else {
                        //Error
                        abort();
                    }

                    if (t.hasAttribute("Size")) {
                        text_data->size = t.attribute("Size").toDouble();
                    } else {
                        //Error
                        abort();
                    }

                    //many optional attributes to be implemented

                    QDomElement t2;
                    if (!(t2 = t.firstChildElement("ofd:TextCode")).isNull()) {
                        TextCode * text_code_data = new TextCode();
                        text_data->text_code = text_code_data;
                        if (t2.hasAttribute("X"))
                            text_code_data->x = t2.attribute("X").toDouble();
                        else {
                            //Error
                            abort();
                        }
                        if (t2.hasAttribute("Y"))
                            text_code_data->y = t2.attribute("Y").toDouble();
                        else {
                            //Error
                            abort();
                        }
                        if (t2.hasAttribute("DeltaX")) {
                            ST_Array delta_x_data("DeltaX", t2.attribute("DeltaX"));
                            text_code_data->delta_x = delta_x_data;
                        }
                        if (t2.hasAttribute("DeltaY")) {
                            ST_Array delta_y_data("DeltaX", t2.attribute("DeltaY"));
                            text_code_data->delta_y = delta_y_data;
                        }
                        text_code_data->text = t2.text();
                    } else {
                        //Error
                        abort();
                    }
                    t = t.nextSiblingElement("ofd:TextObject");
                }
                //Other GraphicUnit objects to be implemented
                //qDebug() << "End of reading content..." << endl;
                new_layer = new_layer.nextSiblingElement("ofd:Layer");
            }
        }
    } else {
        //Error
        abort();
    }
}

void OFDParser::readPageArea(CT_PageArea * data, QDomElement & root_node) {
    QDomElement t;
    if (!(t = root_node.firstChildElement("ofd:PhysicalBox")).isNull()) {
        QStringList values = t.text().split(" ");
        //qDebug() << values[0] << values[1] << values[2] << endl;
        if (values.size() == 4) {
            data->physical_box = ST_Box(values[0].toDouble(), values[1].toDouble(), values[2].toDouble(), values[3].toDouble());
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
            data->application_box = ST_Box(values[0].toDouble(), values[1].toDouble(), values[2].toDouble(), values[3].toDouble());
        else {
            //Error
            abort();
        }
    }

    if (!(t = root_node.firstChildElement("ofd:ContentBox")).isNull()) {
        QStringList values = t.text().split(" ");
        if (values.size() == 4)
            data->content_box = ST_Box(values[0].toDouble(), values[1].toDouble(), values[2].toDouble(), values[3].toDouble());
        else {
            //Error
            abort();
        }
    }

    if (!(t = root_node.firstChildElement("ofd:BleedBox")).isNull()) {
        QStringList values = t.text().split(" ");
        if (values.size() == 4)
            data->bleed_box = ST_Box(values[0].toDouble(), values[1].toDouble(), values[2].toDouble(), values[3].toDouble());
        else {
            //Error
            abort();
        }
    }
}

void OFDParser::readGraphicUnit(CT_GraphicUnit *data, QDomElement &root_node) {
    //读取属性
    if (root_node.hasAttribute("ID")) {
        ST_ID i(root_node.attribute("ID").toInt());
        data->setID(i);
    } else {
        //Error
        abort();
    }

    if (root_node.hasAttribute("Boundary")) {
        QStringList values = root_node.attribute("Boundary").split(" ");
        if (values.size() == 4) {
            data->boundary = ST_Box(values[0].toDouble(), values[1].toDouble(), values[2].toDouble(), values[3].toDouble());
        }
        else {
            //Error
            abort();
        }
    } else {
        //Error
        abort();
    }

    //读取成员
    QDomElement t;
    if (!(t = root_node.firstChildElement("ofd:FillColor")).isNull()) {
        CT_Color * fill_color_data = new CT_Color();
        data->fill_color = fill_color_data;
        if (t.hasAttribute("Value")) {
            ST_Array value_data("Value", t.attribute("Value"));
            fill_color_data->value = value_data;
        }
        if (t.hasAttribute("ColorSpace")) {
            ST_RefID colorspace_data(t.attribute("ColorSpace").toInt());
            fill_color_data->color_space = colorspace_data;
        }
        //Other attributes to be implemented
    }

    //Other members to be implemented
}

void OFDParser::readResource(Res * res_data) {
    openFile();
    QDomElement new_res = document->firstChildElement("ofd:Res");
//    qDebug() << "Entering readResource module..." << endl;
    if (!new_res.isNull()) {
        //读取属性
        if (new_res.hasAttribute("BaseLoc")) {
            ST_Loc new_baseloc("BaseLoc", new_res.attribute("BaseLoc"), current_path);
            res_data->base_loc = new_baseloc;
        } else {
            //Error
            abort();
        }
        //读取成员
        QDomElement t;
        if (!(t = new_res.firstChildElement("ofd:Fonts")).isNull()) {
            QDomElement t2 = t.firstChildElement("ofd:Font");
            while (!t2.isNull()) {
                CT_Font * font_data = new CT_Font();
                res_data->fonts->push_back(font_data);
                if (t2.hasAttribute("ID")) {
                    ST_ID i(t2.attribute("ID").toInt());
                    font_data->setID(i);
                } else {
                    //Error
                    abort();
                }
                if (t2.hasAttribute("FontName")) {
                    font_data->font_name = t2.attribute("FontName");
                } else {
                    //Error
                    abort();
                }
                if (t2.hasAttribute("FamilyName")) {
                    font_data->family_name = t2.attribute("FamilyName");
                }
                //Other stuff to be implemented
                t2 = t2.nextSiblingElement("ofd:Font");
            }
        }
        if (!(t = new_res.firstChildElement("ofd:ColorSpaces")).isNull()) {
            QDomElement t2 = t.firstChildElement("ofd:ColorSpace");
            while (!t2.isNull()) {
                CT_ColorSpace * colorspace_data = new CT_ColorSpace();
                res_data->colorspaces->push_back(colorspace_data);
                if (t2.hasAttribute("ID")) {
                    ST_ID i(t2.attribute("ID").toInt());
                    colorspace_data->setID(i);
                } else {
                    //Error
                    abort();
                }
                if (t2.hasAttribute("Type")) {
                    colorspace_data->type = t2.attribute("Type");
                } else {
                    //Error
                    abort();
                }
                //Other stuff to be implemented
                t2 = t2.nextSiblingElement("ofd:ColorSpace");
            }
        }
        //Other stuff to be implemented
    } else {
        //Error
        abort();
    }
    qDebug() << "End of reading Resourse..." << endl;
}
