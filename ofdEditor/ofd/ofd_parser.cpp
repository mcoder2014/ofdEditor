#include "ofd_parser.h"
#include <QDebug>
#include "DataTypes/document/OFD.h"
#include "DataTypes/document/DocBody.h"
#include "DataTypes/document/CT_DocInfo.h"
#include "DataTypes/document/Document.h"
#include "DataTypes/page/Page.h"
#include "DataTypes/Color/CT_Color.h"
#include "DataTypes/image/CT_GraphicUnit.h"
#include "DataTypes/text/CT_Text.h"
#include "DataTypes/image/CT_Path.h"
#include "DataTypes/image/CT_Image.h"
#include "DataTypes/image/CT_MultiMedia.h"

OFDParser::OFDParser(QString _path) : current_path("OFD", _path) {
    document = new QDomDocument();
    data = readOFD();
}

void OFDParser::openFile() {
    QFile ofd_file(current_path.getPath());
    if (!ofd_file.open(QFile::ReadOnly | QFile::Text)) {    //检查文件是否存在&正常打开
        qDebug() << "无法打开XML文件: " + current_path.getPath();
    }
    if (document->setContent(&ofd_file,         //解析OFD文档并将树状内容存在document文件中
                             false,
                             &error_msg,
                             &error_line,
                             &error_column)) {
        ofd_file.close();
    } else {
        qDebug() << "XML文件格式有错误: " + current_path.getPath();
    }
}

OFD * OFDParser::readOFD() {
    openFile();
    QDomElement new_ofd = document->firstChildElement("ofd:OFD");
    OFD *ofd_data = NULL;
    if (!new_ofd.isNull()) {
        ofd_data = new OFD();
        if (new_ofd.hasAttribute("DocType"))
            ofd_data->doc_type = new_ofd.attribute("DocType");
        else
            qDebug() << "OFD类型的数据中中缺少必要的DocType属性\n位于"
                     << current_path.getRelativePath();
        if (new_ofd.hasAttribute("Version"))
            ofd_data->version = new_ofd.attribute("Version");
        else
            qDebug() << "OFD类型的数据中中缺少必要的Version属性\n位于"
                     << current_path.getRelativePath();
        ofd_data->root_path = current_path;
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
                //读取CustomsDatas
                if (!(t = new_docinfo.firstChildElement("ofd:CustomDatas")).isNull()) {
                    QDomElement t2 = t.firstChildElement("ofd:CustomData");
                    while (!t2.isNull()) {
                        QStringList l;
                        l.append(t2.attribute("Name"));
                        l.append(t2.text());
                        docinfo_data->custom_datas->push_back(l);
                        t2 = t2.nextSiblingElement("ofd:CustomData");
                    }
                }
            } else {
                qDebug() << "DocBody类型的数据中中缺少必要的DocInfo成员\n位于"
                         << current_path.getRelativePath();
            }
            QDomElement new_docroot = new_docbody.firstChildElement("ofd:DocRoot");
            if (!new_docroot.isNull()) {
                ST_Loc p("Document", new_docroot.text(), current_path);
                docbody_data->doc_root = p;
            } else {
                qDebug() << "DocBody类型的数据中中缺少必要的DocInfo成员\n位于"
                         << current_path.getRelativePath();
            }
            new_docbody = new_ofd.nextSiblingElement("ofd:DocBody");
        }

    } else {
        qDebug() << "OFD类型的数据中中缺少必要的DocBody成员\n位于"
                 << current_path.getRelativePath();
    }
    if (ofd_data) {
        for (int i = 0; i < ofd_data->docbodys->size(); i++) {
            current_path = ofd_data->docbodys->at(i)->doc_root;
            ofd_data->docs->push_back(readDocument());
        }
    }
    return ofd_data;
}

Document * OFDParser::readDocument() {
    openFile();
    qDebug() << "Checkpoint 1: Entering Document...";
    QDomElement new_document = document->firstChildElement("ofd:Document");
    Document *document_data;
    if (!new_document.isNull()) {
        document_data = new Document();
        id_table = document_data->id_table;
        // commonData
        QDomElement new_commondata = new_document.firstChildElement("ofd:CommonData");
        if (!new_commondata.isNull()) {
            CT_CommonData * commondata_data = new CT_CommonData();
            document_data->common_data = commondata_data;
            qDebug() << "Checkpoint 1.1...";
            QDomElement t;

            // MaxUnitID
            if (!(t = new_commondata.firstChildElement("ofd:MaxUnitID")).isNull()) {
                commondata_data->max_unit_id = t.text().toInt();
            } else {
                qDebug("缺少MaxID");
                //Error
            }

            // PageArea
            if (!(t = new_commondata.firstChildElement("ofd:PageArea")).isNull()) {
                CT_PageArea *pagearea_data = new CT_PageArea();
                commondata_data->page_area = pagearea_data;
                readPageArea(pagearea_data, t);
            } else {
                qDebug("PageArea");
            }
            qDebug() << "Checkpoint 1.1.1...";

            // publicRes
            t = new_commondata.firstChildElement("ofd:PublicRes");
            while (!t.isNull()) {
                ST_Loc p("PublicRes", t.text(), current_path);
                //qDebug() << "PublicRes = " << t.text();
                commondata_data->public_res->push_back(p);
                t = t.nextSiblingElement("ofd:PublicRes");
            }

            // documentRes
            t = new_commondata.firstChildElement("ofd:DocumentRes");
            while (!t.isNull()) {
                ST_Loc p("DocumentRes", t.text(), current_path);
                //qDebug() << "PublicRes = " << t.text();
                commondata_data->document_res->push_back(p);
                t = t.nextSiblingElement("ofd:DocumentRes");
            }


            if (!(t = new_commondata.firstChildElement("ofd:DefaultCS")).isNull()) {
                //to be implemented
            }

        } else {
            qDebug("Document类型的数据中中缺少必要的CommonData成员\n位于Document.xml");
        }

        qDebug() << "Checkpoint 1.2...";
        // Pages
        QDomElement new_pages = new_document.firstChildElement("ofd:Pages");
        CT_Pages *pages_data;
        if (!new_pages.isNull()) {
            pages_data = new CT_Pages();
            QDomElement new_page = new_pages.firstChildElement("ofd:Page");
            //qDebug() << new_page.tagName() << endl;
            while (!new_page.isNull()) {
                Page *p = new Page();
                ST_ID i(new_page.attribute("ID").toLong());
                p->setID(i, id_table);
                ST_Loc path("Page", new_page.attribute("BaseLoc"), current_path);
                p->base_loc = path;
                pages_data->pages->push_back(p);
                new_page = new_page.nextSiblingElement("ofd:Page");
            }
        } else {
            qDebug("Document类型的数据中中缺少必要的Pages成员\n位于Document.xml");
        }
        document_data->pages = pages_data;
        qDebug() << "Checkpoint 1.3...";
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
        qDebug() << "Document.xml文档中缺少必要的Document标签\n位于"
                 << current_path.getRelativePath();
    }

    // publicRes
    for (int i = 0; i < document_data->common_data->public_res->size(); i++) {
        current_path = document_data->common_data->public_res->at(i);
        Res * res_data = new Res();
        readResource(res_data);
        document_data->public_res->push_back(res_data);
    }

    // documentRes
    for (int i = 0; i < document_data->common_data->document_res->size(); i++) {
        current_path = document_data->common_data->document_res->operator [](i);
        Res * res_data = new Res();
        readResource(res_data);
        document_data->public_res->push_back(res_data);
    }

    //访问页面
    for (int i = 0; i < document_data->pages->pages->size(); i++) {
        current_path = document_data->pages->pages->at(i)->base_loc;
        readPage(document_data->pages->pages->at(i));
    }

    //访问自定义标签
    if (!document_data->custom_tags.isNull()) {
        current_path = document_data->custom_tags;

    }
    return document_data;
}

///
/// \brief OFDParser::readPage
///     解析页
/// \param page_data
///
void OFDParser::readPage(Page * page_data) {
    openFile();
    QDomElement new_page = document->firstChildElement("ofd:Page");
    qDebug() << "Checkpoint 2: Entering Page...";
    if (!new_page.isNull())
    {
        QDomElement t;

        // pageArea
        if (!(t = new_page.firstChildElement("ofd:Area")).isNull())
        {
            CT_PageArea * area_data = new CT_PageArea();
            readPageArea(area_data, t);
            page_data->area = area_data;
        }

        // pageRes
        t = new_page.firstChildElement("ofd:PageRes");
        while (!t.isNull())
        {
            ST_Loc p("PageRes", t.text(), current_path);
            Res * new_res = new Res();
            current_path = p;
            readResource(new_res);
            page_data->page_res->push_back(new_res);
            t = t.nextSiblingElement("ofd:PageRes");
        }

        // Content
        if (!(t = new_page.firstChildElement("ofd:Content")).isNull())
        {
            //            qDebug() << "Checkpoint 2.1";
            // layer
            QDomElement new_layer = t.firstChildElement("ofd:Layer");
            while (!new_layer.isNull())
            {
                CT_Layer * layer_data = new CT_Layer();
                // ID
                page_data->getContent()->push_back(layer_data);
                if (new_layer.hasAttribute("ID"))
                {
                    ST_ID i(new_layer.attribute("ID").toInt());
                    layer_data->setID(i, id_table);
                }
                else
                {
                    qDebug() << "CT_Layer类型数据中缺少必要的ID属性\n位于"
                             << current_path.getRelativePath();
                }

                // Type
                if(new_layer.hasAttribute("Type"))
                {
                    QString tempType = new_layer.attribute("Type");
                    layer_data->setType(tempType);
                }

                // DrawParam
                if (new_layer.hasAttribute("DrawParam"))
                {
                    ST_RefID ri(new_layer.attribute("DrawParam").toInt());
                    if (id_table->contains(ri.getRefID()))
                        layer_data->draw_param = ri;
                    else
                    {
                        qDebug() << "Checkpoint ID 1...";
                        qDebug() << "Layer类型数据的DrawParam属性引用了未注册的ID 位于"
                                 << current_path.getRelativePath();
                    }
                }
                //                qDebug() << "Checkpoint 2.2";
                QDomElement t;
                //解析文字对象
                bool is_first_textcode_x = true, is_first_textcode_y = true;
                double last_x = 0.0, last_y = 0.0;

                // TextObject
                t = new_layer.firstChildElement("ofd:TextObject");
                while (!t.isNull())
                {
                    //qDebug() << "Start to read TextObject..." << endl;
                    CT_Text * text_data = new CT_Text();
                    layer_data->text_object->push_back(text_data);

                    readGraphicUnit(text_data, t);
                    //Read TextObject attributes
                    if (t.hasAttribute("Font"))
                    {
                        ST_RefID ri(t.attribute("Font").toInt());
                        if (id_table->contains(ri.getRefID()))
                            text_data->font = ri;
                        else
                        {
                            qDebug() << "Checkpoint ID 2...";
                            qDebug() << "CT_Text类型数据的Font属性引用了未注册的ID:"
                                     << QString::number(ri.getRefID())
                                     << " 位于"
                                     << current_path.getRelativePath();
                        }
                    }
                    else
                    {
                        qDebug() << "CT_Text类型数据中缺少必要的Font属性\n位于"
                                 << current_path.getRelativePath();
                    }

                    // size
                    if (t.hasAttribute("Size"))
                    {
                        text_data->size = t.attribute("Size").toDouble();
                        text_data->size_used = true;
                    }
                    else
                    {
                        qDebug() << "CT_Text类型数据中缺少必要的Size属性\n位于"
                                 << current_path.getRelativePath();
                    }

                    // Italic
                    if(t.hasAttribute("Italic"))
                    {
                        QString italic = t.attribute("Italic");
                        italic = italic.toLower();
                        if(italic == "true")
                        {
                            text_data->italic = true;
                        }
                    }

                    // Weight
                    if(t.hasAttribute("Weight"))
                    {
                        text_data->weight = t.attribute("Weight").toInt();
                    }


                    //                    qDebug() << "Checkpoint 2.3";
                    //many optional attributes to be implemented
                    // textCode
                    QDomElement t2 = t.firstChildElement("ofd:TextCode");
                    if (t2.isNull())
                    {
                        qDebug() << "CT_Text类型数据中缺少必要的TextCode成员\n位于"
                                 << current_path.getRelativePath();
                    }
                    //                    qDebug() << "Checkpoint 2.3.1";
                    while(!t2.isNull())
                    {
                        TextCode * text_code_data = new TextCode();
                        //                        qDebug() << "Checkpoint 2.3.1.0";
                        text_data->text_code->push_back(text_code_data);
                        //                        qDebug() << "Checkpoint 2.3.1.1";
                        if (t2.hasAttribute("X"))
                        {
                            last_x = text_code_data->x = t2.attribute("X").toDouble();
                            if (is_first_textcode_x)
                                is_first_textcode_x = false;
                        }
                        else
                        {
                            if (!is_first_textcode_x && !is_first_textcode_y)
                                text_code_data->x = last_x;
                            else
                            {
                                qDebug() << "TextCode类型数据中缺少第一个X值\n位于"
                                         << current_path.getRelativePath();
                            }
                        }
                        //                    qDebug() << "Checkpoint 2.3.2";
                        if (t2.hasAttribute("Y"))
                        {
                            last_y = text_code_data->y = t2.attribute("Y").toDouble();
                            if (is_first_textcode_y)
                                is_first_textcode_y = false;
                        }
                        else
                        {
                            if (!is_first_textcode_x && !is_first_textcode_y)
                                text_code_data->y = last_y;
                            else
                            {
                                qDebug() << "TextCode类型数据中缺少第一个Y值\n位于"
                                         << current_path.getRelativePath();
                            }
                        }
                        if (t2.hasAttribute("DeltaX"))
                        {
                            ST_Array delta_x_data("DeltaX", t2.attribute("DeltaX"));
                            text_code_data->delta_x = delta_x_data;
                        }
                        if (t2.hasAttribute("DeltaY"))
                        {
                            ST_Array delta_y_data("DeltaX", t2.attribute("DeltaY"));
                            text_code_data->delta_y = delta_y_data;
                        }
                        text_code_data->text = t2.text();

                        t2 = t2.nextSiblingElement("ofd:TextCode");
                    }
                    t = t.nextSiblingElement("ofd:TextObject");
                }
                //                qDebug() << "Checkpoint 2.4";

                //解析矢量图对象
                t = new_layer.firstChildElement("ofd:PathObject");
                while (!t.isNull())
                {
                    CT_Path * path_data = new CT_Path();
                    layer_data->path_object->push_back(path_data);
                    readGraphicUnit(path_data, t);
                    if (t.hasAttribute("Stroke"))
                    {
                        path_data->stroke = t.attribute("Stroke") == "false" ? false : true;
                    }
                    if (t.hasAttribute("Fill"))
                    {
                        path_data->fill = t.attribute("Fill") == "false" ? false : true;
                    }
                    if (t.hasAttribute("Rule"))
                    {
                        if (t.attribute("Rule") == "NonZero")
                            path_data->rule = "NonZero";
                        else if (t.attribute("Rule") == "Even-Odd")
                            path_data->rule = "Even-Odd";
                        else
                        {
                            qDebug() << "CT_Path类型数据中的Rule属性值为非法值\n位于"
                                     << current_path.getRelativePath()
                                     << " 值为"
                                     << t.attribute("Rule");
                        }
                    }
                    // 图形变换的描述
                    if (!t.firstChildElement("ofd:AbbreviatedData").isNull())
                    {
                        path_data->abbreviated_data = t.firstChildElement("ofd:AbbreviatedData").text();
                    }
                    else
                    {
                        qDebug() << "CT_Path类型数据中缺少必要的AbbreviatedData成员\n位于"
                                 << current_path.getRelativePath();
                    }
                    t = t.nextSiblingElement("ofd:PathObject");
                }

                //解析位图对象
                // ImageObject
                t = new_layer.firstChildElement("ofd:ImageObject");
                while (!t.isNull())
                {
                    CT_Image * image_data = new CT_Image();
                    layer_data->image_object->push_back(image_data);
                    readGraphicUnit(image_data, t);             // 读取图元里的共用的数据
                    if (t.hasAttribute("ResourceID"))
                    {
                        ST_RefID ri(t.attribute("ResourceID").toInt());
                        if (id_table->contains(ri.getRefID()))
                            image_data->resource_id = ri;
                        else
                        {
                            qDebug() << ri.getRefID();
                            qDebug() << "Checkpoint ID 3...";
                            //qDebug("CT_Image类型数据的ResourceID属性引用了未注册的ID 位于" + current_path.getRelativePath());
                        }
                    }
                    else
                    {
                        qDebug() << "CT_Image类型数据中缺少必要的ResourceID属性\n位于"
                                 << current_path.getRelativePath();
                    }

                    if (t.hasAttribute("Substitution"))
                    {
                        ST_RefID ri(t.attribute("Substitution").toInt());
                        if (id_table->contains(ri.getRefID()))
                            image_data->substitution = ri;
                        else
                        {
                            qDebug() << "Checkpoint ID 4...";
                            qDebug() << "CT_Image类型数据的Substitution属性引用了未注册的ID 位于"
                                     << current_path.getRelativePath();
                        }
                    }
                    t = t.nextSiblingElement("ofd:ImageObject");
                }
                //Other GraphicUnit objects to be implemented
                //qDebug() << "End of reading content..." << endl;
                new_layer = new_layer.nextSiblingElement("ofd:Layer");
            }
        }
    }
    else
    {
        qDebug() << "Content.xml文档中缺少必要的Page标签\n位于"
                 << current_path.getRelativePath();
    }
    //    qDebug() << "Checkpoint 3";
}

void OFDParser::readPageArea(CT_PageArea * data, QDomElement & root_node) {
    QDomElement t;
    //        qDebug() << "Checkpoint PA 1...";
    if (!(t = root_node.firstChildElement("ofd:PhysicalBox")).isNull())
    {
        QStringList values = t.text().split(" ");
        //qDebug() << values[0] << values[1] << values[2] << endl;
        if (values.size() == 4)
        {
            data->physical_box = ST_Box(
                    values[0].toDouble(),
                    values[1].toDouble(),
                    values[2].toDouble(),
                    values[3].toDouble());
        }
        else
        {
            qDebug() << "CT_PageArea类型数据的PhysicalBox成员的值的数目错误\n位于"
                     << current_path.getRelativePath();
        }
    }
    else
    {
        qDebug() << "CT_PageArea类型的数据中中缺少必要的PhysicalBox成员\n位于"
                 << current_path.getRelativePath();
    }
    //        qDebug() << "Checkpoint PA 2...";
    if (!(t = root_node.firstChildElement("ofd:ApplicationBox")).isNull())
    {
        QStringList values = t.text().split(" ");
        if (values.size() == 4)
            data->application_box = ST_Box(
                        values[0].toDouble(),
                    values[1].toDouble(),
                    values[2].toDouble(),
                    values[3].toDouble());
        else
        {
            qDebug() << "CT_PageArea类型数据的ApplicationBox成员的值的数目错误\n位于"
                     << current_path.getRelativePath();
        }
    }
    //        qDebug() << "Checkpoint PA 3...";
    if (!(t = root_node.firstChildElement("ofd:ContentBox")).isNull())
    {
        QStringList values = t.text().split(" ");
        if (values.size() == 4)
            data->content_box = ST_Box(
                        values[0].toDouble(),
                    values[1].toDouble(),
                    values[2].toDouble(),
                    values[3].toDouble());
        else
            qDebug() << "CT_PageArea类型数据的ContentBox成员的值的数目错误\n位于"
                     << current_path.getRelativePath();
    }

    if (!(t = root_node.firstChildElement("ofd:BleedBox")).isNull())
    {
        QStringList values = t.text().split(" ");
        if (values.size() == 4)
            data->bleed_box = ST_Box(
                        values[0].toDouble(),
                    values[1].toDouble(),
                    values[2].toDouble(),
                    values[3].toDouble());
        else
        {
            qDebug() << "CT_PageArea类型数据的BleedBox成员的值的数目错误\n位于"
                     << current_path.getRelativePath();
        }
    }
}

///
/// \brief OFDParser::readGraphicUnit
/// \param data
/// \param root_node
///
void OFDParser::readGraphicUnit(CT_GraphicUnit *data, QDomElement &root_node)
{
    // ID
    if (root_node.hasAttribute("ID"))
    {
        ST_ID i(root_node.attribute("ID").toInt());
        data->setID(i, id_table);

    }
    else
    {
        qDebug() << "CT_GraphicUnit类型数据中缺少必要的ID属性\n位于"
                 << current_path.getRelativePath();
    }

    // Boundary
    if (root_node.hasAttribute("Boundary"))
    {
        QStringList values = root_node.attribute("Boundary").split(" ");
        if (values.size() == 4)
        {
            data->boundary = ST_Box(
                    values[0].toDouble(),
                    values[1].toDouble(),
                    values[2].toDouble(),
                    values[3].toDouble());
        }
        else
        {
            qDebug() << "CT_GraphicUnit类型数据的Boundary成员的值的数目错误\n位于"
                     << current_path.getRelativePath();
        }
    }
    else
    {
        qDebug() << "CT_GraphicUnit类型数据中缺少必要的ID属性\n位于"
                 << current_path.getRelativePath();
    }

    //读取成员
    QDomElement t;
    if (!(t = root_node.firstChildElement("ofd:FillColor")).isNull())
    {
        CT_Color * fill_color_data = new CT_Color();
        data->fill_color = fill_color_data;
        readColor(fill_color_data, t);
        if (root_node.hasAttribute("ID")
                && root_node.attribute("ID").toInt() == 3)
            qDebug() << "In OFDParser: " << fill_color_data;
    }

    //Other members to be implemented
}

///
/// \brief OFDParser::readResource
///     解析资源文件
/// \param res_data
///
void OFDParser::readResource(Res * res_data) {
    qDebug() << "read res-----------";
    openFile();
    QDomElement new_res = document->firstChildElement("ofd:Res");
    if (!new_res.isNull())
    {
        //读取属性
        if (new_res.hasAttribute("BaseLoc"))
        {
            ST_Loc new_baseloc("BaseLoc", new_res.attribute("BaseLoc"), current_path);
            res_data->base_loc = new_baseloc;
        }
        else
        {
            qDebug() << "Res类型数据中缺少必要的BaseLoc属性\n位于"
                     <<  current_path.getRelativePath();
        }

        //读取成员--font
        QDomElement t;
        if (!(t = new_res.firstChildElement("ofd:Fonts")).isNull())
        {
            QDomElement t2 = t.firstChildElement("ofd:Font");
            while (!t2.isNull())
            {
                CT_Font * font_data = new CT_Font();
                res_data->fonts->push_back(font_data);
                readFont(font_data, t2);
                t2 = t2.nextSiblingElement("ofd:Font");
            }
        }

        // colorSpaces
        if (!(t = new_res.firstChildElement("ofd:ColorSpaces")).isNull())
        {
            QDomElement t2 = t.firstChildElement("ofd:ColorSpace");
            while (!t2.isNull())
            {
                CT_ColorSpace * colorspace_data = new CT_ColorSpace();
                res_data->colorspaces->push_back(colorspace_data);
                readColorSpace(colorspace_data, t2);
                t2 = t2.nextSiblingElement("ofd:ColorSpace");
            }
        }

        // DrawParams
        if (!(t = new_res.firstChildElement("ofd:DrawParams")).isNull())
        {
            QDomElement t2 = t.firstChildElement("ofd:DrawParam");
            while (!t2.isNull())
            {
                CT_DrawParam * draw_param_data = new CT_DrawParam();
                res_data->draw_params->push_back(draw_param_data);
                readDrawParam(draw_param_data, t2);
                t2 = t2.nextSiblingElement("ofd:DrawParam");
            }
        }

        // MultMedia
        if (!(t = new_res.firstChildElement("ofd:MultiMedias")).isNull())
        {
//            qDebug() << "multmedias flag read";

            QDomElement t2 = t.firstChildElement("ofd:MultiMedia");
            while (!t2.isNull())
            {

                CT_MultiMedia *multimedia = new CT_MultiMedia();

                // ID
                if(t2.hasAttribute("ID"))
                {
                    ST_ID i(t2.attribute("ID").toLong());
//                    qDebug() << "multimedia set id";
                    multimedia->setID(i, id_table);
//                    qDebug() << "multimedia set id";
                }
                else
                {
                    qDebug() << "multimedia don't has ID attribute";
                    continue;
                }

                // Type
                if(t2.hasAttribute("Type"))
                {
                    multimedia->Type = t2.attribute("Type");
                }
                else
                {
                    qDebug() << "Multimedia has no Type attribute";
                    continue;
                }

                // mediaFile
                QDomElement mediaFile = t2.firstChildElement("ofd:MediaFile");
                // 直接转换为了绝对路径
                multimedia->MediaFile = res_data->base_loc + "/" + mediaFile.text();       // 文件路径
                res_data->multMedias->push_back(multimedia);        // 插入

//                qDebug() << multimedia->getID().getID()
//                        << multimedia->Type
//                        << multimedia->MediaFile;

                t2 = t2.nextSiblingElement("ofd:MultiMedia");
            }
        }

        //Other stuff to be implemented
    }
    else
    {
        qDebug() << "Resource.xml文档中缺少必要的Res标签\n位于"
                 << current_path.getRelativePath();
    }

    qDebug() << "read res-----------end";
    //qDebug() << "End of reading Resourse..." << endl;
}

void OFDParser::readColor(CT_Color *data, QDomElement & root_node) {
    if (root_node.hasAttribute("Value"))
    {
        ST_Array value_data("Value", root_node.attribute("Value"));
        data->value = value_data;
    }
    if (root_node.hasAttribute("ColorSpace"))
    {
        ST_RefID colorspace_data(root_node.attribute("ColorSpace").toInt());
        if (id_table->contains(colorspace_data.getRefID()))
            data->color_space = colorspace_data;
        else
        {
            qDebug() << "CT_Color类型数据的ColorSpace属性引用了未注册的ID:"
                     << QString::number(colorspace_data.getRefID())
                     << " 位于"
                     << current_path.getRelativePath();
        }

    }
    if (root_node.hasAttribute("Index"))
    {
        data->index = root_node.attribute("Index").toInt();
        data->index_used = true;
    }
    if (root_node.hasAttribute("Alpha"))
    {
        data->alpha = root_node.attribute("Alpha").toInt();
    }
    //Other stuff to be implemented
}
void OFDParser::readFont(CT_Font * data, QDomElement & root_node)
{
    if (root_node.hasAttribute("ID"))
    {
        ST_ID i(root_node.attribute("ID").toInt());
        data->setID(i, id_table);
    }
    else
    {
        qDebug() << "CT_Font类型数据中缺少必要的ID属性\n位于"
                 << current_path.getRelativePath();
    }

    if (root_node.hasAttribute("FontName"))
    {
        data->font_name = root_node.attribute("FontName");
    }
    else
    {
        qDebug() << "CT_Font类型数据中缺少必要的FontName属性\n位于"
                 << current_path.getRelativePath();
    }

    if (root_node.hasAttribute("FamilyName"))
    {
        data->family_name = root_node.attribute("FamilyName");
    }

    if (root_node.hasAttribute("Charset"))
    {
        data->charset = root_node.attribute("Charset");
    }

    if (root_node.hasAttribute("Italic"))
    {
        data->italic = root_node.attribute("Italic") == "true" ? true : false;
    }

    if (root_node.hasAttribute("Bold"))
    {
        data->bold = root_node.attribute("Bold") == "true" ? true : false;
    }

    if (root_node.hasAttribute("Serif"))
    {
        data->serif = root_node.attribute("Serif") == "true" ? true : false;
    }

    if (root_node.hasAttribute("FixedWidth"))
    {
        data->fixed_width = root_node.attribute("FixedWidth") == "true" ? true : false;
    }
}
void OFDParser::readColorSpace(CT_ColorSpace * data, QDomElement & root_node)
{
    if (root_node.hasAttribute("ID"))
    {
        ST_ID i(root_node.attribute("ID").toInt());
        data->setID(i, id_table);
    }
    else
    {
        qDebug() << "CT_ColorSpace类型数据中缺少必要的ID属性\n位于"
                 <<  current_path.getRelativePath();
    }

    if (root_node.hasAttribute("Type"))
    {
        data->type = root_node.attribute("Type");
    }
    else
    {
        qDebug() << "CT_ColorSpace类型数据中缺少必要的Type属性\n位于"
                 <<  current_path.getRelativePath();
    }

    if (root_node.hasAttribute("BitsPerComponent"))
    {
        data->bits_per_component = root_node.attribute("BitsPerComponent").toInt();
    }

    if (root_node.hasAttribute("Profile"))
    {
        ST_Loc p("Profile", root_node.attribute("Profile"), current_path);
        data->profile = p;
    }
    //Some other stuff to be implemented
}
void OFDParser::readDrawParam(CT_DrawParam * data, QDomElement & root_node)
{
    if (root_node.hasAttribute("ID"))
    {
        ST_ID i(root_node.attribute("ID").toInt());
        data->setID(i, id_table);
    }
    else
    {
        qDebug() << "CT_DrawParam类型数据中缺少必要的ID属性\n位于"
                 <<  current_path.getRelativePath();
    }

    if (root_node.hasAttribute("Relative"))
    {
        ST_RefID i(root_node.attribute("Relative").toInt());
        if (id_table->contains(i.getRefID()))
            data->relative = i;
        else
        {
            qDebug() << "CT_DrawParam类型数据的Relative属性引用了未注册的ID 位于"
                     << current_path.getRelativePath();
        }

    }

    if (root_node.hasAttribute("LineWidth"))
    {
        data->line_width = root_node.attribute("LineWidth").toDouble();
    }

    if (root_node.hasAttribute("Join"))
    {
        QString j = root_node.attribute("Join");
        if (j == "Miter" || j == "Round" || j == "Bevel")
            data->join = j;
        else
        {
            qDebug() << "CT_DrawParam类型数据中的Join属性值为非法值\n位于"
                     << current_path.getRelativePath();
        }
    }

    if (root_node.hasAttribute("Cap"))
    {
        QString c = root_node.attribute("Cap");
        if (c == "Butt" || c == "Round" || c == "Square")
            data->cap = c;
        else
        {
            qDebug() << "CT_DrawParam类型数据中的Cap属性值为非法值\n位于"
                     <<  current_path.getRelativePath();
        }
    }

    if (root_node.hasAttribute("DashOffset"))
    {
        data->dash_offset = root_node.attribute("DashOffset").toDouble();
    }

    if (root_node.hasAttribute("DashPattern"))
    {
        ST_Array d("DashPattern", root_node.attribute("DashPattern"));
        data->dash_pattern = d;
    }

    if (root_node.hasAttribute("MiterLimit"))
    {
        if (data->getJoin() == "Miter")
            data->miter_limit = root_node.attribute("MiterLimit").toDouble();
    }

    if (!root_node.firstChildElement("ofd:FillColor").isNull())
    {
        CT_Color * fill_color_data = new CT_Color();
        delete data->fill_color;
        data->fill_color = fill_color_data;
        readColor(fill_color_data, root_node);
        data->fill_color_used = true;
    }

    if (!root_node.firstChildElement("ofd:StrokeColor").isNull())
    {
        CT_Color * stroke_color_data = new CT_Color();
        delete data->stroke_color;
        data->stroke_color = stroke_color_data;
        readColor(stroke_color_data, root_node);
        data->stroke_color_used = true;
    }
}
