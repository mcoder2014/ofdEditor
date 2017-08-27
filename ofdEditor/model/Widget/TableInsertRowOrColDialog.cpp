#include "TableInsertRowOrColDialog.h"
#include "ui_TableInsertRowOrColDialog.h"

TableInsertRowOrColDialog::TableInsertRowOrColDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TableInsertRowOrColDialog)
{
    ui->setupUi(this);
}

TableInsertRowOrColDialog::~TableInsertRowOrColDialog()
{
    delete ui;
}
