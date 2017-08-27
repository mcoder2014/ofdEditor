#include "TableSettingDialog.h"
#include "ui_TableSettingDialog.h"

TableSettingDialog::TableSettingDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TableSettingDialog)
{
    ui->setupUi(this);
}

TableSettingDialog::~TableSettingDialog()
{
    delete ui;
}
