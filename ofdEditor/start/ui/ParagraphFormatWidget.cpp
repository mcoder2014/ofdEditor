#include "ParagraphFormatWidget.h"
#include "ui_ParagraphFormatWidget.h"

ParagraphFormatWidget::ParagraphFormatWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ParagraphFormatWidget)
{
    ui->setupUi(this);
}

ParagraphFormatWidget::~ParagraphFormatWidget()
{
    delete ui;
}
