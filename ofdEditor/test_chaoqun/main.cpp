#include "mainwindow.h"
#include "PassageMainWindow.h"
#include <QApplication>
#include "Doc/DocPassage.h"
#include "Doc/DocPage.h"
#include <QLabel>
#include <QVBoxLayout>
#include <QScreen>
#include <QSizePolicy>

#include "Doc/DocTextBlock.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

//    MainWindow w;
//    w.show();
    PassageMainWindow w;
    w.setMinimumSize(1000,800);
//    w.setMinimumSize(QApplication::primaryScreen()->size().width(),
//                     QApplication::primaryScreen()->size().height());
//    w.showMaximized();


    DocPage* page1 = new DocPage(210,297,1,&w);
    DocPage* page2 = new DocPage(210,297,1,&w);

    DocPassage * pas = new DocPassage();
    pas->addPage(page1);
    pas->addPage(page2);

    page1->addBlock(new DocTextBlock(page1),DocPage::Body);

    w.setPassage(pas);


//    w.setSizePolicy(QSizePolicy(QSizePolicy::Maximum,
//                                QSizePolicy::Maximum));


    w.show();

    return a.exec();
}
