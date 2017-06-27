#include "InsertTableDialog.h"
#include "ui_InsertTableDialog.h"

InsertTableDialog::InsertTableDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::InsertTableDialog)
{
    ui->setupUi(this);
}

InsertTableDialog::~InsertTableDialog()
{
    delete ui;
}
