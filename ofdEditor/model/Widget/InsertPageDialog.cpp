#include "InsertPageDialog.h"
#include "ui_InsertPageDialog.h"

InsertPageDialog::InsertPageDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::InsertPageDialog)
{
    ui->setupUi(this);
}

InsertPageDialog::~InsertPageDialog()
{
    delete ui;
}
