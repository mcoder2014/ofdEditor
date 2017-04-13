#include "mainwindow.h"
#include <QApplication>

#include <model.h>
#include <ofd.h>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    Model test();     // 多工程测试,使用时可以去掉
    Ofd test2();

    return a.exec();
}
