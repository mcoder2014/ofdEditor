#include "mainwindow.h"

#include <QToolBar>
#include <QFileDialog>
#include <QStandardPaths>
#include <QDebug>
#include <exception>

#include "Loaders/ZipTool.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    this->setWindowTitle(tr("Chaoqun's test window"));
    this->setMinimumSize(800 , 600);
    this->initUI();
    this->initActions();
}

MainWindow::~MainWindow()
{
    if(this->compress != NULL)
    {
        delete this->compress;
        this->compress = NULL;
    }
    if(this->extract != NULL)
    {
        delete this->extract;
        this->extract = NULL;
    }
    if(this->selectFile != NULL)
    {
        delete this->selectFile;
        this->selectFile = NULL;
    }

    if(this->toolBar != NULL)
    {
        delete this->toolBar;
        this->toolBar = NULL;
    }
}

void MainWindow::initUI()
{
    this->toolBar = new QToolBar();
    this->addToolBar(this->toolBar);
}

void MainWindow::initActions()
{
    this->compress = new QAction();
    this->compress->setText(tr("Compress"));
    connect(this->compress, &QAction::triggered,
            this,&MainWindow::compressDir);
    this->toolBar->addAction(this->compress);

    this->selectFile = new QAction();
    this->selectFile->setText(tr("files"));
    connect(this->selectFile, &QAction::triggered,
            this,&MainWindow::selectFiles);
    this->toolBar->addAction(this->selectFile);

    this->extract = new QAction();
    this->extract->setText(tr("extract"));
    connect(this->extract, &QAction::triggered,
            this,&MainWindow::extractDir);
    this->toolBar->addAction(this->extract);

}

/**
 * @Author Chaoqun
 * @brief  简单的选择文件窗口
 * @param  参数
 * @return 返回值
 * @date   2017/04/29
 */
void MainWindow::selectFiles()
{

    QFileDialog * fileDialog = new QFileDialog(this);           // 新建一个QFileDialog
    fileDialog->setAcceptMode(QFileDialog::AcceptOpen);         // 设置对话框为打开文件类型
    fileDialog->setFileMode(QFileDialog::Directory);         // 设置文件对话框能够存在的文件
    fileDialog->setViewMode(QFileDialog::Detail);               // 文件以细节形式显示出来
    //fileDialog->setNameFilter(tr("OFD files(*.ofd)"));            // 设置文件过滤器
    fileDialog->setWindowTitle(tr("Open A ofd File"));
    fileDialog->setMinimumSize(600,300);

    if(fileDialog->exec() == QDialog::Accepted)
    {
        QString path = fileDialog->selectedFiles()[0];      // 用户选择文件名
        qDebug() << path;
        this->file = path;
    }
}

/**
 * @Author Chaoqun
 * @brief  压缩文件夹
 * @param  参数
 * @return 返回值
 * @date   2017/04/29
 */
void MainWindow::compressDir()
{

    ZipTool::compressDir(
      QStandardPaths::writableLocation(QStandardPaths::DesktopLocation) + "/test.ofd",
                         this->file, true);
}

void MainWindow::extractDir()
{
    try{

        ZipTool::extractDir(this->file,
         QStandardPaths::writableLocation(QStandardPaths::DesktopLocation));

    }catch(std::exception & e)
    {
        qDebug() << "接收异常" <<e.what();
    }
    qDebug() << "调用函数extractDir()";
}
