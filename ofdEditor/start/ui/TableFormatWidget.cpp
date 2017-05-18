#include "TableFormatWidget.h"
#include "ui_TableFormatWidget.h"

TableFormatWidget::TableFormatWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TableFormatWidget)
{
    ui->setupUi(this);
}

TableFormatWidget::~TableFormatWidget()
{
    delete ui;
}
