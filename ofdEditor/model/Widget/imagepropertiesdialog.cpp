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
    // 建立连接
    this->connect( this, SIGNAL(changeImageProperties(double,double,double,double,bool)),
                   this->block, SLOT(imagePropertiesChanged(double,double,double,double,bool)));

    // 设置界面显示
    ui->LockRatio->setChecked(this->block->isWidthHeightRatioLocked()); // 是否锁定纵横比
    // 图片实际尺寸
    ui->InitialSize->setText(
                tr("Width: ")
                + QString::number(this->block->getRealWidth())
                + tr(" mm")
                + tr(", Height: ")
                + QString::number(this->block->getRealHeight())
                + tr(" mm"));

    // 保存图片原始尺寸
    initial_width = this->block->getRealWidth();
    initial_height = this->block->getRealWidth();
    // 是否锁定纵横比
    ratio_locked = this->block->isWidthHeightRatioLocked();

    // 页大小
    this->page_width = this->block->getPage()->getWidth();
    this->page_height = this->block->getPage()->getHeight();

    // 像素大小？？
    ui->WidthInPixel->setValue(this->block->width());
    ui->HeightInPixel->setValue(this->block->height());
    ui->xInPixel->setValue(this->block->x());
    ui->yInPixel->setValue(this->block->y());

    // 图片采用毫米为单位
    ui->WidthInMM->setValue(
                UnitTool::pixelToMM(
                    this->block->width()));
    ui->HeightInMM->setValue(
                UnitTool::pixelToMM(
                    this->block->height()));

    ui->xInMM->setValue(
                UnitTool::pixelToMM(
                    this->block->x()));
    ui->yInMM->setValue(
                UnitTool::pixelToMM(
                    this->block->y()));

    // 求比例
    ui->WidthInPercentage->setValue(
                this->block->width() / this->initial_width * 100.0);
    ui->HeightInPercentage->setValue(
                this->block->height() / this->initial_height * 100.0);

    ui->xInPercentage->setValue(
                ui->xInMM->value() / this->page_width * 100.0);
    ui->yInPercentage->setValue(
                ui->yInMM->value() / this->page_height * 100.0);

}

///**
// * @Author Pan
// * @brief  槽函数，用语接收DocImageBlock的信号后更新自身的信息
// * @param  若干参数
// * @return void
// * @date   2017/06/25
// */
//void ImagePropertiesDialog::receiveImageInfo(double image_width,
//                                             double image_height,
//                                             double image_x,
//                                             double image_y,
//                                             double page_width,
//                                             double page_height,
//                                             bool ratio_is_locked) {
//    ui->LockRatio->setChecked(ratio_is_locked);
//    ui->InitialSize->setText(tr("Width: ") + QString::number(image_width) + tr(", Height: ") + QString::number(image_height));
//    initial_width = image_width;
//    initial_height = image_height;
//    ratio_locked = ratio_is_locked;
//    this->page_width = page_width;
//    this->page_height = page_height;

//    ui->WidthInPixel->setValue(image_width);
//    ui->HeightInPixel->setValue(image_height);
//    ui->xInPixel->setValue(image_x);
//    ui->yInPixel->setValue(image_y);

//    ui->WidthInPercentage->setValue(ui->WidthInPixel->value() / image_width * 100.0);
//    ui->HeightInPercentage->setValue(ui->HeightInPixel->value() / image_height * 100.0);
//    ui->xInPercentage->setValue(ui->xInPixel->value() / page_width * 100.0);
//    ui->yInPercentage->setValue(ui->yInPixel->value() / page_width * 100.0);

//}

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

    this->connect(ui->LockRatio, SIGNAL(stateChanged(int)),
                  this, SLOT(lockRatioStateChanged(int)));
    this->connect(ui->HeightInPercentage, SIGNAL(valueChanged(double)),
                  this, SLOT(Percentage2Pixel(double)));
    this->connect(ui->WidthInPercentage, SIGNAL(valueChanged(double)),
                  this, SLOT(Percentage2Pixel(double)));
    this->connect(ui->xInPercentage, SIGNAL(valueChanged(double)),
                  this, SLOT(Percentage2Pixel(double)));
    this->connect(ui->yInPercentage, SIGNAL(valueChanged(double)),
                  this, SLOT(Percentage2Pixel(double)));
    this->connect(ui->HeightInPixel, SIGNAL(valueChanged(double)),
                  this, SLOT(Pixel2Percentage(double)));
    this->connect(ui->WidthInPixel, SIGNAL(valueChanged(double)),
                  this, SLOT(Pixel2Percentage(double)));
    this->connect(ui->xInPixel, SIGNAL(valueChanged(double)),
                  this, SLOT(Pixel2Percentage(double)));
    this->connect(ui->yInPixel, SIGNAL(valueChanged(double)),
                  this, SLOT(Pixel2Percentage(double)));
    this->connect(ui->WidthInPixel,SIGNAL(valueChanged(double)),
                  this, SLOT(Width2HeightTrans(double)));
    this->connect(ui->HeightInPixel,SIGNAL(valueChanged(double)),
                  this, SLOT(Height2WidthTrans(double)));

}

///**
// * @Author Pan
// * @brief  发送信号
// * @param  void
// * @return void
// * @date   2017/06/25
// */
//void ImagePropertiesDialog::emitMessage()
//{
//    emit changeImageProperties(ui->WidthInPixel->value(),
//                               ui->HeightInPixel->value(),
//                               ui->xInPixel->value(),
//                               ui->yInPixel->value(),
//                               ui->LockRatio->isChecked());

////    this->disconnect(_block,SIGNAL(sendImageInfo(double,double,double,double,double,double,bool)),
////                  this,SLOT(receiveImageInfo(double,double,double,double,double,double,bool)));

//}

///
/// \brief ImagePropertiesDialog::finished_slots
/// \param value
///
void ImagePropertiesDialog::finished_slots(int value)
{
    if(value == QDialog::Accepted)
    {
        // 点击确认
        emit changeImageProperties(ui->WidthInPixel->value(),
                                   ui->HeightInPixel->value(),
                                   ui->xInPixel->value(),
                                   ui->yInPixel->value(),
                                   ui->LockRatio->isChecked());
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
    qDebug() << "Waaaa";
    if (locked == Qt::Checked && !ratio_locked)
    {
        qDebug() << "Set to Checked.";
        ratio_locked = true;

    }
    else if (locked == Qt::Unchecked && ratio_locked)
    {
        qDebug() << "Set to Unchecked.";
        ratio_locked = false;
        this->disconnect(ui->WidthInPercentage,
                      SIGNAL(valueChanged(double)),
                             this, SLOT(Width2HeightTrans(double)));
        this->disconnect(ui->HeightInPercentage,
                      SIGNAL(valueChanged(double)),
                             this, SLOT(Height2WidthTrans(double)));
    }
}

/**
 * @Author Pan
 * @brief  锁定纵横比时，宽度到高度的按比例同步
 * @param  double value
 * @return void
 * @date   2017/06/25
 */
void ImagePropertiesDialog::Width2HeightTrans(double value)
{
    //qDebug() << "W2H";
    if (ratio_locked)
    {
        double ratio = initial_height / initial_width;
        ui->HeightInPixel->setValue(value * ratio);
    }
}

/**
 * @Author Pan
 * @brief  锁定纵横比时，高度到宽度的按比例同步
 * @param  double value
 * @return void
 * @date   2017/06/25
 */
void ImagePropertiesDialog::Height2WidthTrans(double value)
{
    if (ratio_locked)
    {
        double ratio = initial_width / initial_height;
        ui->WidthInPixel->setValue(value * ratio);
    }
}

/**
 * @Author Pan
 * @brief  从像素量到比例量的同步
 * @param  double value
 * @return void
 * @date   2017/06/25
 */
void ImagePropertiesDialog::Pixel2Percentage(double value)
{
//    static int cnt = 1;
//    qDebug() << cnt++;
//    qDebug() << ui->HeightInPixel->value();
    ui->HeightInPercentage->setValue(100.0 * ui->HeightInPixel->value() / initial_height);
    ui->WidthInPercentage->setValue(100.0 * ui->WidthInPixel->value() / initial_width);
    ui->xInPercentage->setValue(100.0 * ui->xInPixel->value() / page_width);
    ui->yInPercentage->setValue(100.0 * ui->yInPixel->value() / page_height);
}

/**
 * @Author Pan
 * @brief  从比例量到像素量的同步
 * @param  double value
 * @return void
 * @date   2017/06/25
 */
void ImagePropertiesDialog::Percentage2Pixel(double value)
{
    ui->HeightInPixel->setValue(ui->HeightInPercentage->value() * initial_height / 100.0);
    ui->WidthInPixel->setValue(ui->WidthInPercentage->value() * initial_width / 100.0);
    ui->xInPixel->setValue(ui->xInPercentage->value() * page_width / 100.0);
    ui->yInPixel->setValue(ui->yInPercentage->value() * page_height / 100.0);
}



