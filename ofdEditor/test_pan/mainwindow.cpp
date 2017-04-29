#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "text_object_classes.h"
#include <QMessageBox>
#include <qDebug>
using namespace std;
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    file = new QFile("F:\\第六届中软杯\\OFD Document\\Doc_0\\Pages\\Page_0\\Content.xml");
    if (!file->open(QFile::ReadOnly | QFile::Text)) {
            QMessageBox::critical(this, tr("Error"),
                                  tr("Cannot read file %1").arg("F:\\第六届中软杯\\OFD Document\\Doc_0\\Pages\\Page_0\\Content.xml"));
    }
    reader.setDevice(file);
    reader.readNext();
    if (reader.isStartDocument()) {
        qDebug() << "XML Version : " << reader.documentVersion()
             << " XML Encoding : " << reader.documentEncoding() << endl;
        readPage();
    } else {
        qDebug() << "An error has occured. " << endl;
        abort();
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::readPage() {                   //读取页面
    Q_ASSERT(reader.isStartDocument());
    reader.setNamespaceProcessing(false);
    reader.readNext();
    if (reader.isStartElement() && reader.name() == "Page" ) {
        while (!reader.isEndElement() || reader.name() != "Page") //?????????
        {
            reader.readNext();
            if (reader.isStartElement() && reader.name() == "Area")
                readPageArea();
            else if (reader.isStartElement() && reader.name() == "Content")
                readContent();
        }
    }
    else {
        qDebug() << "The page file doesn't start with a <page> tag." << endl;
        abort();
    }
}

void MainWindow::readPageArea() {                   //读取页面区域信息
    Q_ASSERT(reader.isStartElement() && reader.name() == "Area");
    while (!reader.isEndElement() || reader.name() != "Area")
    {
        reader.readNext();
        if (reader.isStartElement() && reader.name() == "PhysicalBox") {
            QString str = reader.readElementText();
            QStringList strl = str.split(" ");
            p.area.physical_box.start_x = strl.at(0).toDouble();
            p.area.physical_box.start_y = strl.at(1).toDouble();
            p.area.physical_box.delta_x = strl.at(2).toDouble();
            p.area.physical_box.delta_y = strl.at(3).toDouble();
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

                    }
                    if (textobject_attributes.hasAttribute("Boundary"))
                    {

                    }
                    if (textobject_attributes.hasAttribute("Font"))
                    {

                    }
                    if (textobject_attributes.hasAttribute("Size"))
                    {

                    }
                    while (!reader.isEndElement() || reader.name() != "TextObject") {
                        reader.readNext();
                        //FillColor待实现
                        if (reader.isStartElement() && reader.name() == "TextCode")
                        {
                            QXmlStreamAttributes textcode_attributes = reader.attributes();
                            if (textcode_attributes.hasAttribute("X"))
                            {

                            }
                            if (textcode_attributes.hasAttribute("Y"))
                            {

                            }
                            if (textcode_attributes.hasAttribute("DeltaX"))
                            {

                            }
                            if (textcode_attributes.hasAttribute("DeltaY"))
                            {

                            }
                            QString str = reader.readElementText();
                            //将内容存储
                        }
                    }

                }
            }
        }
    }
}
