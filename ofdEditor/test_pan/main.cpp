#include "mainwindow.h"
#include <QApplication>
#include <iostream>
#include <QDebug>
#include "DataTypes/basic_datatype.h"
#include "ofd_parser.h"
#include <QGraphicsView>

using namespace std;
int main(int argc, char *argv[])
{
    OFDParser t("F://第六届中软杯//OFD Document//OFD.xml");
    OFD * i = t.getData();
    qDebug() << i->getDocuments()->at(0)->getPublicRes()->at(0)->getBaseLoc().getPath();
    qDebug() << i->getDocuments()->at(0)->getPages()->getPages()->at(1)->getBaseLoc();
    qDebug() << i->getDocuments()->at(0)->getPages()->getPages()->at(1)->getContent()->at(0)->getTextObject()->size();
    for (int k = 0; k < i->getDocuments()->at(0)->getPages()->getPages()->at(1)->getContent()->at(0)->getTextObject()->size(); k++)
        qDebug() << i->getDocuments()->at(0)->getPages()->getPages()->at(1)->getContent()->at(0)->getTextObject()->at(k)->getTextCode()->getText();
//    return 0;

}
