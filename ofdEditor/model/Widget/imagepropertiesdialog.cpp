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
//    if(ratio_locked)
//    {
//        this->connect(ui->WidthInMM,SIGNAL(valueChanged(double)),
//                      this, SLOT(Width2HeightTrans(double)));
//        this->connect(ui->HeightInMM,SIGNAL(valueChanged(double)),
//                      this, SLOT(Height2WidthTrans(double)));
//    }
//    // 设置界面显示
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


//    // 初始化信号槽链接
//    this->connect(ui->HeightInPercentage, SIGNAL(valueChanged(double)),
//                  this, SLOT(Percentage2mm(double)));
//    this->connect(ui->WidthInPercentage, SIGNAL(valueChanged(double)),
//                  this, SLOT(Percentage2mm(double)));
//    this->connect(ui->xInPercentage, SIGNAL(valueChanged(double)),
//                  this, SLOT(Percentage2mm(double)));
//    this->connect(ui->yInPercentage, SIGNAL(valueChanged(double)),
//                  this, SLOT(Percentage2mm(double)));

//    this->connect(ui->HeightInMM, SIGNAL(valueChanged(double)),
//                  this, SLOT(mm2Percentage(double)));
//    this->connect(ui->WidthInMM, SIGNAL(valueChanged(double)),
//                  this, SLOT(mm2Percentage(double)));
//    this->connect(ui->xInMM, SIGNAL(valueChanged(double)),
//                  this, SLOT(mm2Percentage(double)));
//    this->connect(ui->yInMM, SIGNAL(valueChanged(double)),
//                  this, SLOT(mm2Percentage(double)));

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

//    if(this->ratio_locked)
//    {
//        // 解除长和宽之间的关联
//        this->disconnect(ui->WidthInPercentage,
//                      SIGNAL(valueChanged(double)),
//                             this, SLOT(Width2HeightTrans(double)));

//        this->disconnect(ui->HeightInPercentage,
//                      SIGNAL(valueChanged(double)),
//                             this, SLOT(Height2WidthTrans(double)));
//    }

//    this->disconnect(ui->HeightInPercentage, SIGNAL(valueChanged(double)),
//                  this, SLOT(Percentage2mm(double)));
//    this->disconnect(ui->WidthInPercentage, SIGNAL(valueChanged(double)),
//                  this, SLOT(Percentage2mm(double)));
//    this->disconnect(ui->xInPercentage, SIGNAL(valueChanged(double)),
//                  this, SLOT(Percentage2mm(double)));
//    this->disconnect(ui->yInPercentage, SIGNAL(valueChanged(double)),
//                  this, SLOT(Percentage2mm(double)));

//    this->disconnect(ui->HeightInMM, SIGNAL(valueChanged(double)),
//                  this, SLOT(mm2Percentage(double)));
//    this->disconnect(ui->WidthInMM, SIGNAL(valueChanged(double)),
//                  this, SLOT(mm2Percentage(double)));
//    this->disconnect(ui->xInMM, SIGNAL(valueChanged(double)),
//                  this, SLOT(mm2Percentage(double)));
//    this->disconnect(ui->yInMM, SIGNAL(valueChanged(double)),
//                  this, SLOT(mm2Percentage(double)));
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
//    qDebug() << "lockRatio ";
//    if (locked == Qt::Checked && !ratio_locked)
//    {
//        qDebug() << "Set to Checked.";
//        ratio_locked = true;

//        // 长和宽之间的关联
//        this->connect(ui->WidthInMM,
//                      SIGNAL(valueChanged(double)),
//                             this, SLOT(Width2HeightTrans(double)));

//        this->connect(ui->HeightInMM,
//                      SIGNAL(valueChanged(double)),
//                             this, SLOT(Height2WidthTrans(double)));

//    }
//    else if (locked == Qt::Unchecked && ratio_locked)
//    {

//        qDebug() << "Set to Unchecked.";
//        ratio_locked = false;

//        // 解除长和宽之间的关联
//        this->disconnect(ui->WidthInMM,
//                      SIGNAL(valueChanged(double)),
//                             this, SLOT(Width2HeightTrans(double)));

//        this->disconnect(ui->HeightInMM,
//                      SIGNAL(valueChanged(double)),
//                             this, SLOT(Height2WidthTrans(double)));
//    }
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

/**
 * @Author Pan
 * @brief  锁定纵横比时，宽度到高度的按比例同步
 * @param  double value
 * @return void
 * @date   2017/06/25
 */
//void ImagePropertiesDialog::Width2HeightTrans(double value)
//{
//    qDebug() << "W2H";
//    if (ratio_locked)
//    {
//        double ratio = initial_height / initial_width * 1.0;
//        qDebug() << value
//                 <<" "
//                << value * ratio
//               << ratio
//               << initial_width
//              << initial_height;
//        ui->HeightInMM->setValue(value * ratio);
//    }
//}

/**
 * @Author Pan
 * @brief  锁定纵横比时，高度到宽度的按比例同步
 * @param  double value
 * @return void
 * @date   2017/06/25
 */
//void ImagePropertiesDialog::Height2WidthTrans(double value)
//{
//    qDebug() << "H2W";;
//    if (ratio_locked)
//    {
//        double ratio = initial_width / initial_height * 1.0;
//        qDebug() << value
//                 <<" "
//                <<value * ratio;
//        ui->WidthInMM->setValue(value * ratio);
//    }
//}

///
/// \brief ImagePropertiesDialog::mm2Percentage
///     修改毫米值时将内容同步到百分比的值上
/// \param value
///
//void ImagePropertiesDialog::mm2Percentage(double value)
//{
//    // 修改百分比时先断开和毫米的链接
//    this->disconnect(ui->HeightInPercentage, SIGNAL(valueChanged(double)),
//                  this, SLOT(Percentage2mm(double)));
//    this->disconnect(ui->WidthInPercentage, SIGNAL(valueChanged(double)),
//                  this, SLOT(Percentage2mm(double)));
//    this->disconnect(ui->xInPercentage, SIGNAL(valueChanged(double)),
//                  this, SLOT(Percentage2mm(double)));
//    this->disconnect(ui->yInPercentage, SIGNAL(valueChanged(double)),
//                  this, SLOT(Percentage2mm(double)));
//    this->disconnect(ui->HeightInMM, SIGNAL(valueChanged(double)),
//                  this, SLOT(mm2Percentage(double)));
//    this->disconnect(ui->WidthInMM, SIGNAL(valueChanged(double)),
//                  this, SLOT(mm2Percentage(double)));
//    this->disconnect(ui->xInMM, SIGNAL(valueChanged(double)),
//                  this, SLOT(mm2Percentage(double)));
//    this->disconnect(ui->yInMM, SIGNAL(valueChanged(double)),
//                  this, SLOT(mm2Percentage(double)));

//    ui->HeightInPercentage->setValue(
//                100.0 * ui->HeightInMM->value() / initial_height);
//    ui->WidthInPercentage->setValue(
//                100.0 * ui->WidthInMM->value() / initial_width);
//    ui->xInPercentage->setValue(
//                100.0 * ui->xInMM->value() / page_width);
//    ui->yInPercentage->setValue(
//                100.0 * ui->yInMM->value() / page_height);

//    this->connect(ui->HeightInPercentage, SIGNAL(valueChanged(double)),
//                  this, SLOT(Percentage2mm(double)));
//    this->connect(ui->WidthInPercentage, SIGNAL(valueChanged(double)),
//                  this, SLOT(Percentage2mm(double)));
//    this->connect(ui->xInPercentage, SIGNAL(valueChanged(double)),
//                  this, SLOT(Percentage2mm(double)));
//    this->connect(ui->yInPercentage, SIGNAL(valueChanged(double)),
//                  this, SLOT(Percentage2mm(double)));
//    this->connect(ui->HeightInMM, SIGNAL(valueChanged(double)),
//                  this, SLOT(mm2Percentage(double)));
//    this->connect(ui->WidthInMM, SIGNAL(valueChanged(double)),
//                  this, SLOT(mm2Percentage(double)));
//    this->connect(ui->xInMM, SIGNAL(valueChanged(double)),
//                  this, SLOT(mm2Percentage(double)));
//    this->connect(ui->yInMM, SIGNAL(valueChanged(double)),
//                  this, SLOT(mm2Percentage(double)));

//    qDebug() << "execute 1";
//}

///
/// \brief ImagePropertiesDialog::Percentage2mm
///     修改百分比的值时，将毫米的值也给修改了
/// \param value
///
//void ImagePropertiesDialog::Percentage2mm(double value)
//{
//    this->disconnect(ui->HeightInPercentage, SIGNAL(valueChanged(double)),
//                  this, SLOT(Percentage2mm(double)));
//    this->disconnect(ui->WidthInPercentage, SIGNAL(valueChanged(double)),
//                  this, SLOT(Percentage2mm(double)));
//    this->disconnect(ui->xInPercentage, SIGNAL(valueChanged(double)),
//                  this, SLOT(Percentage2mm(double)));
//    this->disconnect(ui->yInPercentage, SIGNAL(valueChanged(double)),
//                  this, SLOT(Percentage2mm(double)));
//    this->disconnect(ui->HeightInMM, SIGNAL(valueChanged(double)),
//                  this, SLOT(mm2Percentage(double)));
//    this->disconnect(ui->WidthInMM, SIGNAL(valueChanged(double)),
//                  this, SLOT(mm2Percentage(double)));
//    this->disconnect(ui->xInMM, SIGNAL(valueChanged(double)),
//                  this, SLOT(mm2Percentage(double)));
//    this->disconnect(ui->yInMM, SIGNAL(valueChanged(double)),
//                  this, SLOT(mm2Percentage(double)));

//    ui->HeightInMM->setValue(
//                ui->HeightInPercentage->value() * initial_height / 100.0);
//    ui->WidthInMM->setValue(
//                ui->WidthInPercentage->value() * initial_width / 100.0);
//    ui->xInMM->setValue(
//                ui->xInPercentage->value() * page_width / 100.0);
//    ui->yInMM->setValue(
//                ui->yInPercentage->value() * page_height / 100.0);

//    this->connect(ui->HeightInMM, SIGNAL(valueChanged(double)),
//                  this, SLOT(mm2Percentage(double)));
//    this->connect(ui->WidthInMM, SIGNAL(valueChanged(double)),
//                  this, SLOT(mm2Percentage(double)));
//    this->connect(ui->xInMM, SIGNAL(valueChanged(double)),
//                  this, SLOT(mm2Percentage(double)));
//    this->connect(ui->yInMM, SIGNAL(valueChanged(double)),
//                  this, SLOT(mm2Percentage(double)));

//    this->connect(ui->HeightInPercentage, SIGNAL(valueChanged(double)),
//                  this, SLOT(Percentage2mm(double)));
//    this->connect(ui->WidthInPercentage, SIGNAL(valueChanged(double)),
//                  this, SLOT(Percentage2mm(double)));
//    this->connect(ui->xInPercentage, SIGNAL(valueChanged(double)),
//                  this, SLOT(Percentage2mm(double)));
//    this->connect(ui->yInPercentage, SIGNAL(valueChanged(double)),
//                  this, SLOT(Percentage2mm(double)));

//    qDebug() << "execute 2";
//}
