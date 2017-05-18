#include "TextFormatWidget.h"
#include "ui_TextFormatWidget.h"

TextFormatWidget::TextFormatWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TextFormatWidget)
{
    ui->setupUi(this);
}

TextFormatWidget::~TextFormatWidget()
{
    delete ui;
}
