#include "mainwindow.h"
#include <QApplication>
#include <iostream>
#include <QDebug>
#include "DataTypes/basic_datatype.h"
#include "ofd_parser.h"

using namespace std;
int main(int argc, char *argv[])
{
//    QXmlStreamReader reader;
//    QFile file("F:\\test_xml.xml");
//    file.open(QFile::ReadOnly | QFile::Text);
//    reader.setDevice(&file);
//    reader.readNext();
//    if (reader.isStartDocument()) {
//        qDebug() << "I've got it.\n";
//        reader.readNext();
//        qDebug() << reader.name();
//        qDebug() << reader.readElementText();
//        //readNext();
//        qDebug() << reader.name();
//        qDebug() << "Is End Element ? " << reader.isEndElement();
//        reader.readNext();
//        qDebug() << reader.name();
//        qDebug() << "Is End Element ? " << reader.isEndElement();
//    }
//    else qDebug() << "Nothing happened." << reader.name();
//    file.close();
//    QApplication a(argc, argv);
//    MainWindow w;
//    w.start();
    OFDParser t("F://第六届中软杯//OFD Document//OFD.xml");
    OFD * i = t.getData();
    qDebug() << i->getOfdVersion() << i->getDocType();
}
