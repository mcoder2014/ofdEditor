#include "mainwindow.h"
#include "PassageMainWindow.h"
#include <QApplication>
#include "Doc/DocPassage.h"
#include "Doc/DocPage.h"
#include <QLabel>
#include <QVBoxLayout>
#include <QScreen>
#include <QSizePolicy>
#include <QDebug>
#include <QGraphicsProxyWidget>
#include <QGraphicsScene>

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

    w.setPassage(pas);

    QLabel * label = new QLabel("gfgfdsgfdsgfdgdsfgfdgfdgfdtestetstests");
    QWidget * widget = new QWidget();
    widget->setBackgroundRole(QPalette::Dark);

    DocBlock * block = new DocBlock();

    block->setWidget(label);
    block->setPos(0,0);
    block->resize(400,300);
    block->setVisible(true);
    block->setZValue(100);


//    QGraphicsProxyWidget *proxy = new QGraphicsProxyWidget();
//    proxy->setWidget( widget);
//    proxy->resize(400,400);
//    page1->addItem(proxy);

//    QGraphicsProxyWidget * proxy1 = page1->addWidget(label,Qt::Window);
//    proxy1->resize(400,400);
//    proxy1->setVisible(true);
//    proxy1->setPos(10,50);

//    qDebug() << proxy1->pos().x() <<", "<<proxy1->pos().y();






    page1->addBlock(block, DocPage::Foreground);
        block->setWidget(label);
    //page1->addBlock(new DocTextBlock(page1),DocPage::Body);




//    w.setSizePolicy(QSizePolicy(QSizePolicy::Maximum,
//                                QSizePolicy::Maximum));


    w.show();

    //qDebug()<<bloc->isVisible();

    return a.exec();
}
