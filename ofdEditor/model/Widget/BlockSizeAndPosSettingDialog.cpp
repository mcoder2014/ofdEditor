#include "BlockSizeAndPosSettingDialog.h"
#include "ui_BlockSizeAndPosSettingDialog.h"

#include "Doc/DocBlock.h"
#include "Tool/UnitTool.h"

BlockSizeAndPosSettingDialog *BlockSizeAndPosSettingDialog::m_instance = NULL;

BlockSizeAndPosSettingDialog::BlockSizeAndPosSettingDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::BlockSizeAndPosSettingDialog)
{
    ui->setupUi(this);

    connect(this, SIGNAL(finished(int)),
            this, SLOT(slot_finished(int)));
}

void BlockSizeAndPosSettingDialog::setUI(int width, int height, int x, int y)
{
    ui->widthValue->setValue(width);
    ui->heightValue->setValue(height);
    ui->xValue->setValue(x);
    ui->yValue->setValue(y);
}

void BlockSizeAndPosSettingDialog::slot_finished(int value)
{
    if(value == QDialog::Accepted)
    {
        int height = ui->heightValue->value();
        int width = ui->widthValue->value();
        int x = ui->xValue->value();
        int y = ui->yValue->value();

        this->_block->resize(
                    UnitTool::mmToPixel(width),
                    UnitTool::mmToPixel(height));

        this->_block->setPos(
                    UnitTool::mmToPixel(x),
                    UnitTool::mmToPixel(y));
    }
    else if(value == QDialog::Rejected)
    {

    }
}

BlockSizeAndPosSettingDialog *BlockSizeAndPosSettingDialog::getInstance()
{
    if(m_instance == NULL)
    {
        m_instance = new BlockSizeAndPosSettingDialog();
    }

    return m_instance;
}

void BlockSizeAndPosSettingDialog::DestoryInsatance()
{
    m_instance = NULL;
}

void BlockSizeAndPosSettingDialog::init(DocBlock *block)
{
    this->_block = block;       // 记录操作的对象

    this->setUI(
                UnitTool::pixelToMM( block->size().width()),
                UnitTool::pixelToMM( block->size().height()),
                UnitTool::pixelToMM( block->x()),
                UnitTool::pixelToMM( block->y()));

}

BlockSizeAndPosSettingDialog::~BlockSizeAndPosSettingDialog()
{
    delete ui;
}
