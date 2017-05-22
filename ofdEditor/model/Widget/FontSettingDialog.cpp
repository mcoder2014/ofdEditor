#include "FontSettingDialog.h"
#include "ui_FontSettingDialog.h"

FontSettingDialog::FontSettingDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FontSettingDialog)
{
    ui->setupUi(this);
}

FontSettingDialog::~FontSettingDialog()
{
    delete ui;
}
