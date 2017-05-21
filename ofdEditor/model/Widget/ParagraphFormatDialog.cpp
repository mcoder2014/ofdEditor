#include "ParagraphFormatDialog.h"
#include "ui_ParagraphFormatDialog.h"

ParagraphFormatDialog::ParagraphFormatDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ParagraphFormatDialog)
{
    ui->setupUi(this);
}

ParagraphFormatDialog::~ParagraphFormatDialog()
{
    delete ui;
}
