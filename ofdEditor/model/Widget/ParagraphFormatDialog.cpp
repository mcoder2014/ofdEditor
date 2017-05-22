#include "ParagraphFormatDialog.h"
#include "ui_ParagraphFormatDialog.h"

#include "Doc/DocTextBlock.h"

ParagraphFormatDialog::ParagraphFormatDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ParagraphFormatDialog)
{


    // 设置默认选项
    QTextBlockFormat blockformat();
    this->init(blockformat);
}

ParagraphFormatDialog::~ParagraphFormatDialog()
{
    delete ui;
}

QTextBlockFormat ParagraphFormatDialog::getQTextBlockFormat()
{

}

ParagraphFormatDialog::ParagraphFormatDialog(
        const QTextBlockFormat &blockFormat,
        DocTextBlock *textBlock, QWidget *parent)
    :QDialog(parent),
    ui(new Ui::ParagraphFormatDialog)
{
    this->init(blockFormat);
    this->textBlock = textBlock;        // 留下引用，仅用来连接信号槽

    connect(this, &ParagraphFormatDialog::finished,
            this->textBlock,&DocTextBlock::setTextBlockFormat); // 连接信号和信号槽
}

/**
 * @Author Chaoqun
 * @brief  初始化这个Dialog
 * @param  const QTextBlockFormat &blockFormat
 * @return void
 * @date   2017/05/22
 */
void ParagraphFormatDialog::init(const QTextBlockFormat &blockFormat)
{
    ui->setupUi(this);

    // 此信号槽用来将accepted信号接收，然后通过函数发送finished信号
    connect(this,&ParagraphFormatDialog::accepted,
            this,&ParagraphFormatDialog::accept_slots);



}

/**
 * @Author Chaoqun
 * @brief  绑定accepted信号，用来向外界发送信号
 * @param  void
 * @return void
 * @date   2017/05/22
 */
void ParagraphFormatDialog::accept_slots()
{

//    emit this->finished();
}
