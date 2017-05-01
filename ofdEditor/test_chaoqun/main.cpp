#include "mainwindow.h"
#include "PassageMainWindow.h"
#include <QApplication>
#include "Doc/DocPassage.h"
#include "Doc/DocPage.h"
#include <QLabel>
#include <QVBoxLayout>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

//    MainWindow w;
//    w.show();
    PassageMainWindow w;

    DocPage* page1 = new DocPage(900,300,1,&w);
    DocPage* page2 = new DocPage(600,400,1,&w);

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
    w.setMinimumSize(600,400);

    w.show();

    return a.exec();
}
