#include "mainwindow.h"
#include <QApplication>
#include <iostream>
#include <QDebug>
#include "DataTypes/basic_datatype.h"
#include "ofd_parser.h"

using namespace std;
int main(int argc, char *argv[])
{
    OFDParser t("F://第六届中软杯//OFD Document//OFD.xml");
    OFD * i = t.getData();
    qDebug() << i->getDocuments()->at(0)->getCommonData()->getMaxUnitID().getID();
    qDebug() << i->getDocuments()->at(0)->getCommonData()->getPageArea()->getPhysicalBox().getX();
    qDebug() << i->getDocuments()->at(0)->getPages()->getPages()->at(2)->getID().getID();
    return 0;
}
