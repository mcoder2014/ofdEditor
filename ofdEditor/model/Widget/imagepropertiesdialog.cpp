#include "imagepropertiesdialog.h"
#include "ui_imagepropertiesdialog.h"
#include <QDebug>
/**
 * @Author Pan
 * @brief  构造函数，完成了成员组件的初始化、设置，以及信号槽的连接
 * @param  DocImageBlock * _block, QWidget * parent
 * @return
 * @date   2017/06/25
 */
ImagePropertiesDialog::ImagePropertiesDialog(DocImageBlock * _block, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ImagePropertiesDialog),
    block(_block)
{
    ui->setupUi(this);
    this->connect(_block,
            SIGNAL(sendImageInfo(double,double,
                                 double,double,
                                 double,double,bool)),
                  this,
                  SLOT(receiveImageInfo(double,double,
                                        double,double,
                                        double,double,bool)));

    ui->WidthInPixel->setRange(0.0, 100000);
    ui->HeightInPixel->setRange(0.0, 100000);
    ui->xInPixel->setRange(0.0, 10000);
    ui->yInPixel->setRange(0.0, 10000);
    ui->WidthInPercentage->setRange(0.0, 10000.0);
    ui->HeightInPercentage->setRange(0.0, 10000.0);
    ui->xInPercentage->setRange(0.0, 100.0);
    ui->yInPercentage->setRange(0.0, 100.0);
    this->connect(this, SIGNAL(accepted()), this, SLOT(emitMessage()));
    this->connect(ui->LockRatio, SIGNAL(stateChanged(int)), this, SLOT(lockRatioStateChanged(int)));
    this->connect(ui->HeightInPercentage, SIGNAL(valueChanged(double)), this, SLOT(Percentage2Pixel(double)));
    this->connect(ui->WidthInPercentage, SIGNAL(valueChanged(double)), this, SLOT(Percentage2Pixel(double)));
    this->connect(ui->xInPercentage, SIGNAL(valueChanged(double)), this, SLOT(Percentage2Pixel(double)));
    this->connect(ui->yInPercentage, SIGNAL(valueChanged(double)), this, SLOT(Percentage2Pixel(double)));
    this->connect(ui->HeightInPixel, SIGNAL(valueChanged(double)), this, SLOT(Pixel2Percentage(double)));
    this->connect(ui->WidthInPixel, SIGNAL(valueChanged(double)), this, SLOT(Pixel2Percentage(double)));
    this->connect(ui->xInPixel, SIGNAL(valueChanged(double)), this, SLOT(Pixel2Percentage(double)));
    this->connect(ui->yInPixel, SIGNAL(valueChanged(double)), this, SLOT(Pixel2Percentage(double)));
    this->connect(ui->WidthInPixel,
                  SIGNAL(valueChanged(double)),
                         this, SLOT(Width2HeightTrans(double)));
    this->connect(ui->HeightInPixel,
                  SIGNAL(valueChanged(double)),
                         this, SLOT(Height2WidthTrans(double)));
}
/**
 * @Author Pan
 * @brief  槽函数，用语接收DocImageBlock的信号后更新自身的信息
 * @param  若干参数
 * @return void
 * @date   2017/06/25
 */
void ImagePropertiesDialog::receiveImageInfo(double image_width,
                                             double image_height,
                                             double image_x,
                                             double image_y,
                                             double page_width,
                                             double page_height,
                                             bool ratio_is_locked) {
    ui->LockRatio->setChecked(ratio_is_locked);
    ui->InitialSize->setText(tr("Width: ") + QString::number(image_width) + tr(", Height: ") + QString::number(image_height));
    initial_width = image_width;
    initial_height = image_height;
    ratio_locked = ratio_is_locked;
    this->page_width = page_width;
    this->page_height = page_height;

//    ui->WidthInPercentage->setValue(100.0);
//    ui->HeightInPercentage->setValue(100.0);
//    ui->xInPercentage->setValue(image_x / page_width * 100.0);
//    ui->yInPercentage->setValue(image_y / page_height * 100.0);

    ui->WidthInPixel->setValue(image_width);
    ui->HeightInPixel->setValue(image_height);
    ui->xInPixel->setValue(image_x);
    ui->yInPixel->setValue(image_y);

    ui->WidthInPercentage->setValue(ui->WidthInPixel->value() / image_width * 100.0);
    ui->HeightInPercentage->setValue(ui->HeightInPixel->value() / image_height * 100.0);
    ui->xInPercentage->setValue(ui->xInPixel->value() / page_width * 100.0);
    ui->yInPercentage->setValue(ui->yInPixel->value() / page_width * 100.0);

}



ImagePropertiesDialog::~ImagePropertiesDialog()
{
    delete ui;
}

/**
 * @Author Pan
 * @brief  发送信号
 * @param  void
 * @return void
 * @date   2017/06/25
 */
void ImagePropertiesDialog::emitMessage()
{
    emit changeImageProperties(ui->WidthInPixel->value(),
                               ui->HeightInPixel->value(),
                               ui->xInPixel->value(),
                               ui->yInPixel->value(),
                               ui->LockRatio->isChecked());
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



