#include "imagepropertiesdialog.h"
#include "ui_imagepropertiesdialog.h"
#include "Doc/DocPage.h"
#include "Doc/DocImageBlock.h"
#include "Tool/UnitTool.h"
#include <QDebug>

ImagePropertiesDialog* ImagePropertiesDialog::m_instance = NULL;    // 静态变量初始化

///
/// \brief ImagePropertiesDialog::getInstance
///     获得单例
/// \return
///
ImagePropertiesDialog *ImagePropertiesDialog::getInstance()
{
    if(m_instance != NULL)
    {
        return m_instance;
    }

    m_instance = new ImagePropertiesDialog();
    return m_instance;
}

void ImagePropertiesDialog::DestoryInstance()
{
    m_instance = NULL;
}

void ImagePropertiesDialog::init(DocImageBlock *_block)
{
    this->block = _block;   //  记录下block
    // 先更新数据，后建立连接

    // 保存图片原始尺寸
    this->initial_width = this->block->getRealWidth();
    this->initial_height = this->block->getRealHeight();

    // 页大小
    this->page_width = this->block->getPage()->getWidth();
    this->page_height = this->block->getPage()->getHeight();

    // 图片大小
    this->image_width = UnitTool::pixelToMM(this->block->width());
    this->image_height = UnitTool::pixelToMM(this->block->height());

    // 图片位置+++
    this->pos_x = UnitTool::pixelToMM( this->block->x());
    this->pos_y = UnitTool::pixelToMM( this->block->y());

    // 修改界面显示内容
    // 图片实际尺寸
    ui->InitialSize->setText(
                tr("Width: ")
                + QString::number(this->image_width)
                + tr(" mm")
                + tr(", Height: ")
                + QString::number(this->image_height)
                + tr(" mm"));

    // 图片采用毫米为单位
    ui->WidthInMM->setValue(this->image_width);
    ui->HeightInMM->setValue(this->image_height);

    ui->xInMM->setValue(
                UnitTool::pixelToMM(
                    this->block->x()));
    ui->yInMM->setValue(
                UnitTool::pixelToMM(
                    this->block->y()));

    // 求比例
    ui->WidthInPercentage->setValue(
                this->image_width / this->initial_width * 100.0);
    ui->HeightInPercentage->setValue(
                this->image_height / this->initial_height * 100.0);

    ui->xInPercentage->setValue(
                ui->xInMM->value() / this->page_width * 100.0);
    ui->yInPercentage->setValue(
                ui->yInMM->value() / this->page_height * 100.0);

    // 是否锁定纵横比
    ratio_locked = this->block->isWidthHeightRatioLocked();

    // 设置界面显示
    ui->LockRatio->setChecked(
                this->block->isWidthHeightRatioLocked()); // 是否锁定纵横比

    // 建立连接
    this->connect(
                this,
                SIGNAL(
                    changeImageProperties(
                        double,double,double,double,bool)),
                this->block,
                SLOT(
                    imagePropertiesChanged(
                        double,double,double,double,bool)));

}

ImagePropertiesDialog::~ImagePropertiesDialog()
{
    delete ui;
}


ImagePropertiesDialog::ImagePropertiesDialog(QWidget *parent)
    :QDialog(parent), ui(new Ui::ImagePropertiesDialog)
{
    ui->setupUi(this);
    this->initConnect();
}

void ImagePropertiesDialog::initConnect()
{

    // 关闭窗口后触发
    this->connect(this, SIGNAL(finished(int)),
                  this, SLOT(finished_slots(int)));

    // 锁定纵横比
    this->connect(ui->LockRatio, SIGNAL(stateChanged(int)),
                  this, SLOT(lockRatioStateChanged(int)));

    // x mm
    this->connect(ui->xInMM, SIGNAL(editingFinished()),
                  this, SLOT(editfinished_MM_x()));

    // x percentage
    this->connect(ui->xInPercentage, SIGNAL(editingFinished()),
                  this, SLOT(editfinished_percentage_x()));

    // y mm
    this->connect(ui->yInMM, SIGNAL(editingFinished()),
                  this, SLOT(editfinished_MM_y()));

    // y percentage
    this->connect(ui->yInPercentage, SIGNAL(editingFinished()),
                  this, SLOT(editfinished_percentage_y()));

    // 处理x改变
    this->connect(this, SIGNAL(valueChanged_x(double)),
                  this, SLOT(slots_valueChanged_x(double)));

    // 处理y改变
    this->connect(this, SIGNAL(valueChanged_y(double)),
                  this, SLOT(slots_valueChanged_y(double)));

    // width mm
    this->connect(ui->WidthInMM, SIGNAL(editingFinished()),
                  this, SLOT(editfinished_MM_width()));

    // width percentage
    this->connect(ui->WidthInPercentage, SIGNAL(editingFinished()),
                  this, SLOT(editfinished_percentage_width()));

    // height mm
    this->connect(ui->HeightInMM, SIGNAL(editingFinished()),
                  this, SLOT(editfinished_MM_height()));

    // height percentage
    this->connect(ui->HeightInPercentage, SIGNAL(editingFinished()),
                  this,SLOT(editfinished_percentage_height()));

    // 处理width改变
    this->connect(this, SIGNAL(valueChanged_width(double)),
                  this, SLOT(slots_valueChanged_width(double)));

    // 处理height改变
    this->connect(this, SIGNAL(valueChanged_height(double)),
                  this, SLOT(slots_valueChanged_height(double)));

}

///
/// \brief ImagePropertiesDialog::finished_slots
/// \param value
///
void ImagePropertiesDialog::finished_slots(int value)
{
    if(value == QDialog::Accepted)
    {
        // 点击确认
        emit changeImageProperties(
                    UnitTool::mmToPixel(ui->WidthInMM->value()),
                    UnitTool::mmToPixel(ui->HeightInMM->value()),
                    UnitTool::mmToPixel(ui->xInMM->value()),
                    UnitTool::mmToPixel(ui->yInMM->value()),
                    UnitTool::mmToPixel(ui->LockRatio->isChecked()));
    }

    this->disconnect( this, SIGNAL(changeImageProperties(double,double,double,double,bool)),
                   this->block, SLOT(imagePropertiesChanged(double,double,double,double,bool)));

}

/**
 * @Author Pan
 * @brief  切换锁定纵横比的模式
 * @param  int locked
 * @return void
 * @date   2017/06/25
 */
void ImagePropertiesDialog::lockRatioStateChanged(int locked)
{
    this->ratio_locked = locked;

}

///
/// \brief ImagePropertiesDialog::editfinished_MM_x
///     修改了 x mm
void ImagePropertiesDialog::editfinished_MM_x()
{
    this->pos_x = ui->xInMM->value();
    emit this->valueChanged_x(this->pos_x);

}

///
/// \brief ImagePropertiesDialog::editfinished_MM_y
///     修改了 y mm
///
void ImagePropertiesDialog::editfinished_MM_y()
{
    this->pos_y = ui->yInMM->value();
    emit this->valueChanged_y(this->pos_y);
}

///
/// \brief ImagePropertiesDialog::editfinished_percentage_x
///     修改了 x percentage
///
void ImagePropertiesDialog::editfinished_percentage_x()
{
    this->pos_x = ui->xInPercentage->value() * this->page_width / 100.0;
    emit this->valueChanged_x(this->pos_x);
}

///
/// \brief ImagePropertiesDialog::editfinished_percentage_y
///     修改了 y percentages
///
void ImagePropertiesDialog::editfinished_percentage_y()
{
    this->pos_y = ui->yInPercentage->value() * this->page_height / 100.0;
    emit this->valueChanged_y(this->pos_y);
}

void ImagePropertiesDialog::editfinished_MM_width()
{
    this->image_width = ui->WidthInMM->value();
    emit this->valueChanged_width(this->image_width);

}

void ImagePropertiesDialog::editfinished_MM_height()
{
    this->image_height = ui->HeightInMM->value();
    emit this->valueChanged_height(this->image_height);
}

void ImagePropertiesDialog::editfinished_percentage_width()
{
    this->image_width = ui->WidthInPercentage->value() * this->initial_width / 100.0;
    emit this->valueChanged_width(this->image_width);
}

void ImagePropertiesDialog::editfinished_percentage_height()
{
    this->image_height = ui->HeightInPercentage->value() / 100.0 * this->initial_height;
    emit this->valueChanged_height(this->image_height);
}

///
/// \brief ImagePropertiesDialog::slots_valueChanged_x
///     当x发生改变时进行处理
/// \param x
///
void ImagePropertiesDialog::slots_valueChanged_x(double x)
{
    ui->xInMM->setValue(x);
    ui->xInPercentage->setValue(
                100.0 * x / this->page_width);
}

///
/// \brief ImagePropertiesDialog::slots_valueChanged_y
///     当y发生改变时触发
/// \param y
///
void ImagePropertiesDialog::slots_valueChanged_y(double y)
{
    ui->yInMM->setValue(y);
    ui->yInPercentage->setValue(
                100.0 * y / this->page_height);
}

///
/// \brief ImagePropertiesDialog::slots_valueChanged_width
///     处理当宽度发生改变时
/// \param width
///
void ImagePropertiesDialog::slots_valueChanged_width(double width)
{
    ui->WidthInMM->setValue(width);
    ui->WidthInPercentage->setValue(
                100.0 * width / this->initial_width);

    // 如果锁定纵横比
    if(this->ratio_locked)
    {
        double ratio = initial_height / initial_width;
        this->image_height = width * ratio;

        ui->HeightInMM->setValue(this->image_height);
        ui->HeightInPercentage->setValue(
                    100.0 * this->image_height / this->initial_height);
    }
}

///
/// \brief ImagePropertiesDialog::slots_valueChanged_height
///     处理当高度发生改变时
/// \param height
///
void ImagePropertiesDialog::slots_valueChanged_height(double height)
{
    ui->HeightInMM->setValue(height);
    ui->HeightInPercentage->setValue(
                100.0 * height / this->initial_height);

    // 如果锁定纵横比
    if(this->ratio_locked)
    {
        double ratio = this->initial_width / this-> initial_height;
        this->image_width = height * ratio;

        ui->WidthInMM->setValue(this->image_width);
        ui->WidthInPercentage->setValue(
                    100.0 * this->image_width / this->initial_width);
    }
}
