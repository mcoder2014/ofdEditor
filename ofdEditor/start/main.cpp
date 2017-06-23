#include "mainwindow.h"
#include <QApplication>
#include "ui/PassageMainWindow.h"
#include <QDebug>
#include <../ofd/ofdexceptions.h>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    PassageMainWindow w;
    w.show();

    return a.exec();
}
