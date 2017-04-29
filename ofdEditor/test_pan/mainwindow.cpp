#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <qDebug>
using namespace std;
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
//    file = new QFile("F:\\第六届中软杯\\OFD Document\\Doc_0\\Pages\\Page_0\\Content.xml");
//    if (!file->open(QFile::ReadOnly | QFile::Text)) {
//            QMessageBox::critical(this, tr("Error"),
//                                  tr("Cannot read file %1").arg("F:\\第六届中软杯\\OFD Document\\Doc_0\\Pages\\Page_0\\Content.xml"));
//    }
//    reader.setDevice(file);
//    reader.readNext();
//    if (reader.isStartDocument()) {
//        qDebug() << "XML Version : " << reader.documentVersion()
//             << " XML Encoding : " << reader.documentEncoding() << endl;
//        readPage();
//    } else {
//        qDebug() << "An error has occured. " << endl;
//        abort();
//    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::readPage() {                   //读取页面
//    Q_ASSERT(reader.isStartDocument());
//    reader.setNamespaceProcessing(false);
//    reader.readNext();
//    if (reader.isStartElement() && reader.name() == "Page" ) {
//        while (!reader.isEndElement() || reader.name() != "Page")
//        {
//            reader.readNext();
//            if (reader.isStartElement() && reader.name() == "Area")
//                readPageArea();
//            else if (reader.isStartElement() && reader.name() == "Content")
//                readContent();
//        }
//    }
//    else {
//        qDebug() << "The page file doesn't start with a <page> tag." << endl;
//        abort();
//    }
}

void MainWindow::readPageArea() {                   //读取页面区域信息
    Q_ASSERT(reader.isStartElement() && reader.name() == "Area");
    while (!reader.isEndElement() || reader.name() != "Area")
    {
        reader.readNext();
        if (reader.isStartElement() && reader.name() == "PhysicalBox") {
            QString str = reader.readElementText();
            QStringList strl = str.split(" ");
            qDebug() << strl.at(0).toDouble() << " ";
            qDebug() << strl.at(1).toDouble() << " ";
            qDebug() << strl.at(2).toDouble() << " ";
            qDebug() << strl.at(3).toDouble() << "\n";
        }
    }
}

void MainWindow::readContent() {                    //读取页面内容
    Q_ASSERT(reader.isStartElement() && reader.name() == "Content");
    while (!reader.isEndElement() || reader.name() != "Content")
    {
        reader.readNext();
        if (reader.isStartElement() && reader.name() == "Layer") {  //层
            while (!reader.isEndElement() || reader.name() != "Layer") {
                reader.readNext();
                if (reader.isStartElement() && reader.name() == "TextObject") { //文字图元对象
                    QXmlStreamAttributes textobject_attributes = reader.attributes();  //属性
                    if (textobject_attributes.hasAttribute("ID"))
                    {
                        qDebug() << textobject_attributes.value("ID") << " ";
                    }
                    if (textobject_attributes.hasAttribute("Boundary"))
                    {
                        qDebug() << textobject_attributes.value("Boundary") << " ";
                    }
                    if (textobject_attributes.hasAttribute("Font"))
                    {
                        qDebug() << textobject_attributes.value("Font") << " ";
                    }
                    if (textobject_attributes.hasAttribute("Size"))
                    {
                        qDebug() << textobject_attributes.value("Size") << "\n";
                    }
                    while (!reader.isEndElement() || reader.name() != "TextObject") {
                        reader.readNext();
                        //FillColor待实现
                        if (reader.isStartElement() && reader.name() == "TextCode")
                        {
                            QXmlStreamAttributes textcode_attributes = reader.attributes();
                            if (textcode_attributes.hasAttribute("X"))
                            {
                                qDebug() << textcode_attributes.value("X") << " ";
                            }
                            if (textcode_attributes.hasAttribute("Y"))
                            {
                                qDebug() << textcode_attributes.value("Y") << " ";
                            }
                            if (textcode_attributes.hasAttribute("DeltaX"))
                            {
                                qDebug() << textcode_attributes.value("DeltaX") << " ";
                            }
                            if (textcode_attributes.hasAttribute("DeltaY"))
                            {
                                qDebug() << textcode_attributes.value("DeltaY") << " ";
                            }
                            QString str = reader.readElementText();
                            qDebug() << str << "\n";
                        }
                    }

                }
            }
        }
    }
}

void MainWindow::start()
{
    QFile ofd_file("F:\\第六届中软杯\\OFD Document\\OFD.xml");
    if (!ofd_file.open(QFile::ReadOnly | QFile::Text)) {
            QMessageBox::critical(this, tr("Error"),
                                  tr("Cannot read file %1").arg(ofd_file.fileName()));
    }
    reader.setDevice(&ofd_file);
    reader.readNext();
    if (reader.isStartDocument()) {
        qDebug() << "XML Version : " << reader.documentVersion()
             << " XML Encoding : " << reader.documentEncoding() << endl;
        readOFD();
        ofd_file.close();
    } else {
        qDebug() << "An error has occured. " << endl;
        abort();
    }
}

void MainWindow::readOFD()
{
    vector<QString> doc_root_list;
    Q_ASSERT(reader.isStartDocument());
    reader.setNamespaceProcessing(false);
    reader.readNext();
    if (reader.isStartElement() && reader.name() == "OFD") {
        QXmlStreamAttributes ofd_attributes = reader.attributes();
        qDebug() << "DocType: " << ofd_attributes.value("DocType") << " ";
        qDebug() << "Version: " << ofd_attributes.value("Version") << "\n";
        while (!reader.isEndElement() || reader.name() != "OFD") {
            reader.readNext();
            //qDebug() << reader.name();
            if (reader.isStartElement() && reader.name() == "DocBody") {
                reader.readNextStartElement();
                //qDebug() << reader.name();
                if (reader.name() == "DocInfo") {
                    while (!reader.isEndElement() || reader.name() != "DocInfo") {
                        reader.readNext();
                        if (reader.isStartElement() && reader.name() == "DocID") {
                            qDebug() << "DocID: " << reader.readElementText() << "\n";
                        } else if (reader.isStartElement() && reader.name() == "CreationDate") {
                            qDebug() << "CreationDate: " << reader.readElementText() << "\n";
                        } else if (reader.isStartElement() && reader.name() == "ModDate") {
                            qDebug() << "ModDate: " << reader.readElementText() << "\n";
                        } else if (reader.isStartElement() && reader.name() == "Title") {
                            qDebug() << "Title: " << reader.readElementText() << "\n";
                        } else if (reader.isStartElement() && reader.name() == "CustomDatas") {
                            qDebug() << "自定义数据元： ";
                            while (!reader.isEndElement() || reader.name() != "CustomDatas") {
                                reader.readNext();
                                if (reader.isStartElement() && reader.name() == "CustomData") {
                                    QXmlStreamAttributes customtag_attributes = reader.attributes();
                                    qDebug() << customtag_attributes.value("Name") << ": " << reader.readElementText() << "\n";
                                }
                            }
                        }
                    }
                }
                reader.readNextStartElement();
                if (reader.name() == "DocRoot") {
//                    qDebug() << reader.readElementText() << "\n";
                    doc_root_list.push_back(reader.readElementText());
                }
            }
        }
        vector<QString>::iterator iter;
        for (iter = doc_root_list.begin(); iter != doc_root_list.end(); iter++)
        {
            QString dir = "F:\\第六届中软杯\\OFD Document\\" + *iter;
            //qDebug() << dir << "\n";
            dir.replace("/", "\\");
            //qDebug() << dir << "\n";
            QFile document_file(dir);
            if (!document_file.open(QFile::ReadOnly | QFile::Text)) {
                    QMessageBox::critical(this, tr("Error"),
                                          tr("Cannot read file %1").arg(document_file.fileName()));
            }
            QIODevice * current_device = reader.device();
            reader.setDevice(&document_file);
            reader.readNext();
            if (reader.isStartDocument()) {
                qDebug() << "XML Version : " << reader.documentVersion()
                     << " XML Encoding : " << reader.documentEncoding() << endl;
                readDocument();
                reader.setDevice(current_device);
                document_file.close();
            }
        }
    }
    else {
        qDebug() << "The OFD.xml does not start with a <OFD> tag.";
        abort();
    }
}

void MainWindow::readDocument()
{
    Q_ASSERT(reader.isStartDocument());
    int debug_cnt = 0;
    vector<QString> public_res_list;
    vector<QString> page_list;
    QString custom_tag_dir;
    //qDebug() << "Entering readDocument module..." << "\n";
    reader.readNextStartElement();
    if (reader.name() == "Document") {
        //qDebug() << "Entering Point 1..." << "\n";
        while (!reader.isEndElement() || reader.name() != "Document") {
            reader.readNext();
            //qDebug() << reader.isStartElement() << reader.name() << "\n";
            if (reader.isStartElement() && reader.name() == "CommonData") {
                while (!reader.isEndElement() || reader.name() != "CommonData") {
                    reader.readNext();
                    if (reader.isStartElement() && reader.name() == "MaxUnitID") {
                        qDebug() << "MaxUnitId: " << reader.readElementText();
                    } else if (reader.isStartElement() && reader.name() == "PageArea") {
                        while (!reader.isEndElement() || reader.name() != "PageArea") {
                            reader.readNext();
                            if (reader.isStartElement() && reader.name() == "PhysicalBox") {
                                qDebug() << "PhysicalBox: " << "\n";
                                QString str = reader.readElementText();
                                QStringList strl = str.split(" ");
                                qDebug() << strl.at(0).toDouble() << " ";
                                qDebug() << strl.at(1).toDouble() << " ";
                                qDebug() << strl.at(2).toDouble() << " ";
                                qDebug() << strl.at(3).toDouble() << "\n";
                            }
                            //其他区域 to be implemented
                        }
                    } else if (reader.isStartElement() && reader.name() == "PublicRes") {
                        public_res_list.push_back(reader.readElementText());
                    }
                }
            } else if (reader.isStartElement() && reader.name() == "Pages") {
                while (!reader.isEndElement() || reader.name() != "Pages") {
                    reader.readNext();
                    if (reader.isStartElement() && reader.name() == "Page") {
                        QXmlStreamAttributes pages_attributes = reader.attributes();
                        page_list.push_back(pages_attributes.value("BaseLoc").toString());
                    }
                }
            } else if (reader.isStartElement() && reader.name() == "Outlines") {
                //To be implemented
            } else if (reader.isStartElement() && reader.name() == "CustomTags") {
                //qDebug() << reader.readElementText() << "\n";
                custom_tag_dir = "F:\\第六届中软杯\\OFD Document\\Doc_0\\" + reader.readElementText(); //!!!!!!!
            }
        }
    }
    custom_tag_dir.replace("/", "\\");
    QFile custom_tag_file(custom_tag_dir);
    qDebug() << custom_tag_file.fileName() << "\n";
    if (!custom_tag_file.open(QFile::ReadOnly | QFile::Text)) {
            QMessageBox::critical(this, tr("Error"),
                                  tr("Cannot read file %1").arg(custom_tag_file.fileName()));
    }
    QIODevice * current_device = reader.device();
    reader.setDevice(&custom_tag_file);
    reader.readNext();
    if (reader.isStartDocument()) {
        qDebug() << "XML Version : " << reader.documentVersion()
             << " XML Encoding : " << reader.documentEncoding() << "\n";
        readCustomTags();
        reader.setDevice(current_device);
        custom_tag_file.close();
    }
    vector<QString>::iterator iter;
    for (iter = public_res_list.begin(); iter != public_res_list.end(); iter++)
    {
        QString dir = "F:\\第六届中软杯\\OFD Document\\Doc_0\\" + *iter;
        dir.replace("/", "\\");
        QFile public_res_file(dir);
        qDebug() << public_res_file.fileName() << "\n";
        if (!public_res_file.open(QFile::ReadOnly | QFile::Text)) {
                QMessageBox::critical(this, tr("Error"),
                                      tr("Cannot read file %1").arg(public_res_file.fileName()));
        }
        QIODevice * current_device = reader.device();
        reader.setDevice(&public_res_file);
        reader.readNext();
        if (reader.isStartDocument()) {
            qDebug() << "XML Version : " << reader.documentVersion()
                 << " XML Encoding : " << reader.documentEncoding() << endl;
            readPublicRes();
            reader.setDevice(current_device);
            public_res_file.close();
        }
    }

    for (iter = page_list.begin(); iter != page_list.end(); iter++)
    {
        //qDebug() << *iter << "\n";
        QString dir = "F:\\第六届中软杯\\OFD Document\\Doc_0\\" + *iter;
        dir.replace("/", "\\");
        QFile page_file(dir);
        qDebug() << page_file.fileName() << "\n";
        if (!page_file.open(QFile::ReadOnly | QFile::Text)) {
                QMessageBox::critical(this, tr("Error"),
                                      tr("Cannot read file %1").arg(page_file.fileName()));
        }
        QIODevice * current_device = reader.device();
        reader.setDevice(&page_file);
        reader.readNext();
        if (reader.isStartDocument()) {
            qDebug() << "XML Version : " << reader.documentVersion()
                 << " XML Encoding : " << reader.documentEncoding() << endl;
            readPage();
            reader.setDevice(current_device);
            page_file.close();
        }
    }
}

void MainWindow::readCustomTags()
{
    //to be implemented
}

void MainWindow::readPublicRes()
{
    Q_ASSERT(reader.isStartDocument());
    reader.readNextStartElement();
    if (reader.name() == "Res") {
        QXmlStreamAttributes res_attributes = reader.attributes();
        qDebug() << "BaseLoc: " << res_attributes.value("BaseLoc") <<  endl;
        while (!reader.isEndElement() || reader.name() != "Res") {
            reader.readNext();
            if (reader.isStartElement() && reader.name() == "Fonts") {
                while (!reader.isEndElement() || reader.name() != "Fonts") {
                    reader.readNext();
                    if (reader.isStartElement() && reader.name() == "Font") {
                        QXmlStreamAttributes font_attributes = reader.attributes();
                        if (font_attributes.hasAttribute("FontName"))
                            qDebug() << "FontName: " << font_attributes.value("FontName");
                        if (font_attributes.hasAttribute("FamilyName"))
                            qDebug() << "FamilyName: " << font_attributes.value("FamilyName");
                        //more attributes to be implemented
                    }
                }
            } else if (reader.isStartElement() && reader.name() == "ColorSpaces") {
                while (!reader.isEndElement() || reader.name() != "ColorSpaces") {
                    reader.readNext();
                    if (reader.isStartElement() && reader.name() == "ColorSpace") {
                        QXmlStreamAttributes colorspace_attributes = reader.attributes();
                        if (colorspace_attributes.hasAttribute("FontName"))
                            qDebug() << "ID: " << colorspace_attributes.value("ID");
                        if (colorspace_attributes.hasAttribute("FamilyName"))
                            qDebug() << "Type: " << colorspace_attributes.value("Type");
                        //more attributes to be implemented
                    }
                }
            } //more resource types to be implemented
        }
    }
}
