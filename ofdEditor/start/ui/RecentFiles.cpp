#include "RecentFiles.h"
#include "ui_RecentFiles.h"

RecentFiles::RecentFiles(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RecentFiles)
{
    ui->setupUi(this);
}

RecentFiles::~RecentFiles()
{
    delete ui;
}
