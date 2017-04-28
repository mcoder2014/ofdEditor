#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QXmlStreamReader>
#include <QFile>
#include "text_object_classes.h"
#include "document_object_classes.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    void readPage();
    void readPageArea();
    void readContent();
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QFile *file;
    Page p;
    QXmlStreamReader reader;
};



#endif // MAINWINDOW_H
