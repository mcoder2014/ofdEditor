#include "mainwindow.h"
#include <QApplication>
#include <QDebug>
#include "../ofd/DataTypes/basic_datatype.h"
#include "../ofd/ofd_parser.h"
#include "../ofd/ofd_writer.h"


//#include <QTabWidget>
//#include <QLabel>
//#include <QPixmap>
//#include "../test_chaoqun/PassageMainWindow.h"
//#include "../model/Doc/DocPassage.h"
//#include "../model/Doc/DocPage.h"
//#include "../model/Doc/DocTextBlock.h"
//#include <QLabel>
//#include <QVBoxLayout>
//#include <QScreen>
//#include <QSizePolicy>
//#include <QDebug>
//#include <QGraphicsProxyWidget>
//#include <QGraphicsScene>
//#include <QTextEdit>
//#include <QScrollArea>

using namespace std;
int main(int argc, char *argv[])
{

    try {
        OFDParser t("F:/第六届中软杯/OFD Document/OFD.xml");
        OFD * i = t.getData();
        qDebug() << "从Document_0中收集到的ID数量: " << i->getDocuments()->at(0)->getIDTable()->size();
        qDebug() << "Document.xml中的MaxUnitID属性值: " << i->getDocuments()->at(0)->getCommonData()->getMaxUnitID();
//        OFDWriter w(i, "F:/第六届中软杯/OFD Writing Test 2/");
    } catch (Exception e) {
        qDebug() << e.getMessage();
    }
}
