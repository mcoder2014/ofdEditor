#include "ParagraphFormatWidget.h"
#include "ui_ParagraphFormatWidget.h"

ParagraphFormatWidget::ParagraphFormatWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ParagraphFormatWidget)
{
    ui->setupUi(this);      // 安装ui文件
    this->init();
}

ParagraphFormatWidget::~ParagraphFormatWidget()
{
    delete ui;
}

ParagraphFormatWidget::ParagraphFormatWidget(
        const QTextBlockFormat &blockFormat, QWidget *parent)
    :QWidget(parent),ui(new Ui::ParagraphFormatWidget)
{
    ui->setupUi(this);                  // 安装ui文件
    this->init(blockFormat);            // 初始化

}

void ParagraphFormatWidget::init()
{

}

void ParagraphFormatWidget::init(const QTextBlockFormat &blockFormat)
{

}

void ParagraphFormatWidget::closeWidget()
{

}
