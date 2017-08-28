#include "BlockSizeAndPosSettingDialog.h"
#include "ui_BlockSizeAndPosSettingDialog.h"

BlockSizeAndPosSettingDialog::BlockSizeAndPosSettingDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::BlockSizeAndPosSettingDialog)
{
    ui->setupUi(this);
}

BlockSizeAndPosSettingDialog::~BlockSizeAndPosSettingDialog()
{
    delete ui;
}
