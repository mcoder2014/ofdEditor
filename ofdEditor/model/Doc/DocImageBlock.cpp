#include "DocImageBlock.h"
#include <QPainter>
#include <QDebug>

DocImageBlock::DocImageBlock(QWidget *parent)
    :QLabel(parent)
{
    //图片填满组件
    setScaledContents(true);
    this->setMinimumSize(5, 5);
    this->setFrameStyle(QFrame::NoFrame);
    this->setFocusPolicy(Qt::StrongFocus);

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
 * @brief  焦点聚焦，显示边框
 * @param  QFocusEvent
 * @return void
 * @date   2017/06/23
 */
void DocImageBlock::focusInEvent(QFocusEvent *e)
{
    //qDebug() << "focus In Event";
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
