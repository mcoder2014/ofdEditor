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

    QApplication a(argc, argv);
    PassageMainWindow w;
    w.show();

    return a.exec();
}
