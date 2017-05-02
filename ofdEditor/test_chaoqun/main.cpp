#include "mainwindow.h"
#include "PassageMainWindow.h"
#include <QApplication>
#include "Doc/DocPassage.h"
#include "Doc/DocPage.h"
#include <QLabel>
#include <QVBoxLayout>
#include <QScreen>
#include <QSizePolicy>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

//    MainWindow w;
//    w.show();
    PassageMainWindow w;
    //w.setMinimumSize(800,600);
    w.setMinimumSize(QApplication::primaryScreen()->size().width(),
                     QApplication::primaryScreen()->size().height());
//    w.showMaximized();


    DocPage* page1 = new DocPage(210,297,1,&w);
    DocPage* page2 = new DocPage(210,297,1,&w);

    DocPassage * pas = new DocPassage();
    pas->addPage(page1);
    pas->addPage(page2);

    QLabel *label = new QLabel("test");
    QVBoxLayout * layout = new QVBoxLayout;
    layout->addWidget(label);

    QLabel *label2 = new QLabel("I want to test");
    QVBoxLayout * layout2 = new QVBoxLayout;
    layout2->addWidget(label2);

    page1->setLayout(layout);
    page2->setLayout(layout2);

    w.setPassage(pas);


//    w.setSizePolicy(QSizePolicy(QSizePolicy::Maximum,
//                                QSizePolicy::Maximum));


    w.show();

    return a.exec();
}
