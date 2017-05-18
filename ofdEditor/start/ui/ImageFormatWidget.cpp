#include "ImageFormatWidget.h"
#include "ui_ImageFormatWidget.h"

ImageFormatWidget::ImageFormatWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ImageFormatWidget)
{
    ui->setupUi(this);
}

ImageFormatWidget::~ImageFormatWidget()
{
    delete ui;
}
