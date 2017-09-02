#include "ProgramSettingDialog.h"
#include "ui_ProgramSettingDialog.h"

ProgramSettingDialog::ProgramSettingDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ProgramSettingDialog)
{
    ui->setupUi(this);
}

ProgramSettingDialog::~ProgramSettingDialog()
{
    delete ui;
}
