#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    QAction * selectFile;
    QAction * compress;
    QAction * extract;

    QString file;

    QToolBar* toolBar;

    void initUI();
    void initActions();

    void selectFiles();     // 选择文件
    void compressDir();     // 压缩
    void extractDir();      // 解压


};

#endif // MAINWINDOW_H
