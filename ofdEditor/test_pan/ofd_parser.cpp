#include "ofd_parser.h"
#include <QDebug>

OFDParser::OFDParser(QString _path) : current_path("OFD", _path) {
    openFile();
}

void OFDParser::openFile() {
    reader.setNamespaceProcessing(false);   //不处理namespace
    QFile ofd_file(current_path.getPath());
    if (!ofd_file.open(QFile::ReadOnly | QFile::Text)) {    //检查文件是否存在&正常打开
            //出现异常！！！to be implemented
        qDebug() << "ofd.xml读取出现错误。" << endl;
        abort();
    }
    reader.setDevice(&ofd_file);
    reader.readNext();
    if (reader.isStartDocument()) {
        readOFD();          //解析OFD.xml
        ofd_file.close();
    } else {
        qDebug() << "An error has occured. " << endl;
        abort();
    }
}

void OFDParser::readOFD() {
    Q_ASSERT(reader.isStartDocument());         //确保是文档的开头
    ofd_data = new OFD();       //建立OFD数据
    ofd_data->docbodys = new QVector<DocBody *>();
    ofd_data->docs = new QVector<Document *>();

    while (!reader.isEndDocument()) {
        reader.readNext();
        if (reader.isStartElement() && reader.name() == "OFD") {
            QXmlStreamAttributes ofd_attributes = reader.attributes();
            ofd_data->doc_type = ofd_attributes.value("DocType").toString();
            ofd_data->version = ofd_attributes.value("Version").toString();

            while (!reader.isEndElement() || reader.name() != "OFD") {
                reader.readNext();
                if (reader.isStartElement() && reader.name() == "DocBody") {
                    DocBody * new_doc_body = new DocBody();
                    while (!reader.isEndDocument() || reader.name() != "DocBody") {
                        reader.readNext();
                        if (reader.isStartElement() && reader.name() == "DocInfo") {
                            CT_DocInfo * new_doc_info = new CT_DocInfo();
                            QVector<QStringList> * custom_datas_ = new QVector<QStringList>();
                            while (!reader.isEndElement() || reader.name() != "DocInfo") {
                                reader.readNext();
                                if (reader.isStartElement() && reader.name() == "DocID") {
                                    new_doc_info->doc_id = reader.readElementText();
                                } else if (reader.isStartElement() && reader.name() == "CreationDate") {
                                    new_doc_info->creation_date = reader.readElementText();
                                } else if (reader.isStartElement() && reader.name() == "ModDate") {
                                    new_doc_info->mod_date = reader.readElementText();
                                } else if (reader.isStartElement() && reader.name() == "Title") {
                                    new_doc_info->title = reader.readElementText();
                                } else if (reader.isStartElement() && reader.name() == "Author") {
                                    new_doc_info->author = reader.readElementText();
                                } else if (reader.isStartElement() && reader.name() == "Creator") {
                                    new_doc_info->creator = reader.readElementText();
                                } else if (reader.isStartElement() && reader.name() == "CreatorVersion") {
                                    new_doc_info->creator_version = reader.readElementText();
                                } else if (reader.isStartElement() && reader.name() == "DocUsage") {
                                    new_doc_info->doc_usage = reader.readElementText();
                                } else if (reader.isStartElement() && reader.name() == "Cover") {
                                    QString str = reader.readElementText();
                                    ST_Loc t("Cover", str, current_path);
                                    new_doc_info->cover = t;
                                } else if (reader.isStartElement() && reader.name() == "CustomDatas") {
                                    while (!reader.isEndElement() || reader.name() != "CustomDatas") {
                                        reader.readNext();
                                        if (reader.isStartElement() && reader.name() == "CustomData") {
                                            QXmlStreamAttributes customtag_attributes = reader.attributes();
                                            QStringList t;
                                            t.push_back(customtag_attributes.value("Name").toString());
                                            t.push_back(reader.readElementText());
                                            custom_datas_->push_back(t);
                                        }
                                    }
                                }
                            }
                            new_doc_info->custom_datas = custom_datas_;
                            new_doc_body->doc_info = new_doc_info;
                        } else if (reader.isStartElement() && reader.name() == "DocRoot") {
                            ST_Loc t("Document", reader.readElementText());
                            new_doc_body->doc_root = t;
                        }
                    }
                    ofd_data->docbodys->push_back(new_doc_body);
                }
            }
        }
    }
    for (int i = 0; i < ofd_data->docbodys->size(); i++) {
        ST_Loc t("Document", ofd_data->docbodys->at(i)->doc_root.getPath());
        DocumentParser dp(t);
        ofd_data->docs->push_back(dp.getData());
    }
}


