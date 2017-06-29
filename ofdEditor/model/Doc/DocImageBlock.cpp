#include "DocImageBlock.h"
#include <QPainter>
#include <QDebug>
#include <QFileDialog>
#include <QMessageBox>
#include "DocPage.h"

DocImageBlock::DocImageBlock(QWidget *parent)
    :QLabel(parent)
{
    //图片填满组件
    setScaledContents(true);
    this->setMinimumSize(5, 5);
    this->setFrameStyle(QFrame::NoFrame);
    this->setFocusPolicy(Qt::StrongFocus);

    //Initialization
    this->context_menu = new QMenu(this);
    change_image = new QAction(tr("Change Image"), NULL);
    set_image_properties = new QAction(tr("Set Image Properties"), NULL);
    remove_image = new QAction(tr("Remove Image."), NULL);
    remove_image->setShortcut(Qt::Key_Delete);

    properties_dialog = new ImagePropertiesDialog(this, parent);
    width_height_ratio_locked = false;
    width_height_ratio = 0.0;

    context_menu->addAction(this->change_image);
    context_menu->addAction(this->set_image_properties);
    context_menu->addAction(this->remove_image);

    //signal-slots
    this->connect(this->change_image, SIGNAL(triggered()),
                  this, SLOT(changeImage()));
    this->connect(this->set_image_properties, SIGNAL(triggered()),
                  this, SLOT(setImageProperties()));
    this->connect(properties_dialog,
                  SIGNAL(changeImageProperties(double,double,
                                               double,double,
                                               bool)),
                  this, SLOT(imagePropertiesChanged(double,double,
                                                    double,double,
                                                    bool)));
    this->connect(this->remove_image,
                  SIGNAL(triggered(bool)),
                  this,
                  SLOT(removeImage()));
}

/**
 * @Author Pan
 * @brief  设置DocImageBlock中的图像
 * @param  QPixmap &
 * @return void
 * @date   2017/06/23
 */
void DocImageBlock::setImage(QPixmap & pixmap)
{
    this->setPixmap(pixmap);
}

/**
 * @Author Pan
 * @brief  设置内部对代理它的DocBlock的引用
 * @param  DocBlock * _block
 * @return void
 * @date   2017/06/25
 */
void DocImageBlock::setBlock(DocBlock *_block)
{
    block = _block;
}

/**
 * @Author Pan
 * @brief  是否锁定纵横比
 * @param  none
 * @return bool
 * @date   2017/06/25
 */
bool DocImageBlock::isWidthHeightRatioLocked()
{
    return width_height_ratio_locked;
}

/**
 * @Author Pan
 * @brief  获取纵横比
 * @param  none
 * @return double
 * @date   2017/06/25
 */
double DocImageBlock::getWidthHeightRatio()
{
    if (isWidthHeightRatioLocked())
        return width_height_ratio;
}

DocBlock *DocImageBlock::getBlock()
{
    return this->block;
}

/**
 * @Author Pan
 * @brief  焦点聚焦，显示边框
 * @param  QFocusEvent
 * @return void
 * @date   2017/06/23
 */
void DocImageBlock::focusInEvent(QFocusEvent *e)
{
    //qDebug() << "focus In Event";
    emit signals_currrentImageBlock(this);
    this->setFrameShape(QFrame::Box);
    this->setLineWidth(1);
    QLabel::focusInEvent(e);
}

/**
 * @Author Pan
 * @brief  焦点移出，隐藏边框
 * @param  QFocusEvent *e
 * @return void
 * @date   2017/06/23
 */
void DocImageBlock::focusOutEvent(QFocusEvent *e)
{
    this->setFrameStyle(QFrame::NoFrame);
    QLabel::focusOutEvent(e);
}

/**
 * @Author Pan
 * @brief  右键菜单
 * @param  QContextMenuEvent *ev
 * @return void
 * @date   2017/06/24
 */
void DocImageBlock::contextMenuEvent(QContextMenuEvent *ev)
{
    context_menu->exec(ev->globalPos());
}

/**
 * @Author Pan
 * @brief  更换图片的槽函数
 * @param  none
 * @return void
 * @date   2017/06/25
 */
void DocImageBlock::changeImage()
{
    QString fileName = QFileDialog::getOpenFileName(this,
                                    tr("Open File"), QDir::currentPath());
    if (!fileName.isEmpty()) {
        QPixmap image(fileName);
        if (image.isNull()) {
            QMessageBox::information(this, tr("OFD Editor"),
                                     tr("Cannot open file %1.").arg(fileName));
            return;
        }
        this->setPixmap(image);
    }
}

/**
 * @Author Pan
 * @brief  跳出属性设置菜单，并向对话框发出信号
 * @param  none
 * @return void
 * @date   2017/06/25
 */
void DocImageBlock::setImageProperties()
{
    emit sendImageInfo(this->width(),
                       this->height(),
                       this->pos().x(),
                       this->pos().y(),
                       this->block->getPage()->width(),
                       this->block->getPage()->height(),
                       this->width_height_ratio_locked);
    properties_dialog->exec();
}

void DocImageBlock::removeImage()
{
    this->block->remove();
}

/**
 * @Author Pan
 * @brief  槽函数，根据对话框的信息改动更新自身属性
 * @param  若干参数
 * @return void
 * @date   2017/06/25
 */
void DocImageBlock::imagePropertiesChanged(double new_width,
                            double new_height,
                            double new_x,
                            double new_y,
                            bool ratio_locked)
{
    this->resize(new_width, new_height);
    this->move(new_x, new_y);
    this->width_height_ratio_locked = ratio_locked;
    this->width_height_ratio = new_width / new_height;
}
