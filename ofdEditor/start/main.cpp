#include "mainwindow.h"
#include <QApplication>
#include "ui/PassageMainWindow.h"
#include <QDebug>
#include <../ofd/ofdexceptions.h>
#include <QTranslator>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setWindowIcon(QIcon(":/icons/source/icons/ofdEditor2.png"));

    QTranslator model_translator;
    if(model_translator.load("cn_model.qm"))
    {
        a.installTranslator(&model_translator);
    }
    QTranslator start_translator;
    if(start_translator.load("cn_start.qm"))
    {
        a.installTranslator(&start_translator);
    }

    QTranslator qt_translator;
    if(qt_translator.load("qt_zh_CN.qm"))
    {
        a.installTranslator(&qt_translator);
    }

    PassageMainWindow w;
    w.show();

    return a.exec();
}
