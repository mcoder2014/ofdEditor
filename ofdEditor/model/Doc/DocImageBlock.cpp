#include "DocImageBlock.h"
#include <QPainter>
#include <QDebug>
#include <QFileDialog>
#include <QMessageBox>
#include "DocPage.h"
#include "Tool/UnitTool.h"

DocImageBlock::DocImageBlock(QWidget *parent)
    :QLabel(parent)
{
    //图片填满组件
    setScaledContents(true);
    this->setMinimumSize(5, 5);
    this->setFrameStyle(QFrame::NoFrame);
    this->setFocusPolicy(Qt::StrongFocus);

    //Initialization
//    this->properties_dialog = new ImagePropertiesDialog(this, parent);
    width_height_ratio_locked = true;
    width_height_ratio = 0.0;

    this->initMenu();   // 初始化右键菜单

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

    this->realWidth = UnitTool::pixelToMM(
                pixmap.width());                // 保存实际宽度
    this->realHeight = UnitTool::pixelToMM(
                pixmap.height());               // 保存实际高度
    this->width_height_ratio = this->realWidth / this->realHeight;

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
    else return -1;
}

DocPassage *DocImageBlock::getPassage()
{
    DocBlock *block = this->block;
    if(block == NULL)
        return NULL;

    return block->getPassage();

}

///
/// \brief DocImageBlock::getPage
///     获得图片框所属的页
/// \return
///
DocPage *DocImageBlock::getPage()
{
    DocBlock *block = this->block;
    if(block == NULL)
        return NULL;

    return block->getPage();
}

///
/// \brief DocImageBlock::getLayer
///     获得图片框所属的层
/// \return
///
DocLayer *DocImageBlock::getLayer()
{
    DocBlock *block = this->block;
    if(block == NULL)
        return NULL;

    return block->getLayer();
}

///
/// \brief DocImageBlock::getBlock
/// 获得图片框所属的块
/// \return
///
DocBlock *DocImageBlock::getBlock()
{
    return this->block;
}

QString DocImageBlock::getType()
{
    return tr("DocImageBlock");
}

///
/// \brief DocImageBlock::getMenu
///     获得图片块的右键菜单
/// \return
///
QMenu *DocImageBlock::getMenu()
{
    this->context_menu->clear();
    this->context_menu->setTitle(this->getType());
    this->context_menu->addAction(this->change_image);
    this->context_menu->addAction(this->set_image_properties);

    return this->context_menu;
}

double DocImageBlock::getRealWidth()
{
    return this->realWidth;
}

double DocImageBlock::getRealHeight()
{
    return this->realHeight;
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

///**
// * @Author Pan
// * @brief  右键菜单
// * @param  QContextMenuEvent *ev
// * @return void
// * @date   2017/06/24
// */
//void DocImageBlock::contextMenuEvent(QContextMenuEvent *ev)
//{
//    context_menu->exec(ev->globalPos());
//}

///
/// \brief DocImageBlock::initMenu
///     初始化右键菜单
///
void DocImageBlock::initMenu()
{
    this->context_menu = new QMenu(tr("DocImageBlock"));       // 新建菜单

    this->change_image = new QAction(tr("Change Image"), this);
    this->set_image_properties = new QAction(tr("Property"), this);

    this->context_menu->addAction(this->change_image);
    this->context_menu->addAction(this->set_image_properties);

    //signal-slots
    this->connect(this->change_image, SIGNAL(triggered()),
                  this, SLOT(changeImage()));

    this->connect(this->set_image_properties, SIGNAL(triggered()),
                  this, SLOT(setImageProperties()));

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
    if (!fileName.isEmpty())
    {
        QPixmap image(fileName);
        if (image.isNull())
        {
            QMessageBox::information(this, tr("OFD Editor"),
                                     tr("Cannot open file %1.").arg(fileName));
            return;
        }
        this->setImage(image);

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
    ImagePropertiesDialog* dialog = ImagePropertiesDialog::getInstance();
    dialog->init(this);
    dialog->exec();
}

/**
 * @Author Pan
 * @brief  槽函数，根据对话框的信息改动更新自身属性
 * @param  若干参数
 * @return void
 * @date   2017/06/25
 */
void DocImageBlock::imagePropertiesChanged(
        double new_width,
        double new_height,
        double new_x,
        double new_y,
        bool ratio_locked)
{
    this->block->resize(new_width, new_height);
    this->block->setPos(new_x, new_y);
    this->width_height_ratio_locked = ratio_locked;
    this->width_height_ratio = new_width / new_height;
}
