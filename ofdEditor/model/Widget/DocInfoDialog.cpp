#include "DocInfoDialog.h"
#include "ui_DocInfoDialog.h"

DocInfoDialog::DocInfoDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DocInfoDialog)
{
    ui->setupUi(this);
}

DocInfoDialog::~DocInfoDialog()
{
    delete ui;
}
