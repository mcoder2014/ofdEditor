#include "DocBlock.h"

#include <QTimeLine>
#include <QGraphicsScene>
#include <QDebug>
#include <QCursor>
#include <QMenu>
#include <QAction>
#include <QPainter>

DocBlock::DocBlock(QGraphicsItem *parent , Qt::WindowFlags wFlags)
    :QGraphicsProxyWidget(parent,wFlags)
{
    this->boundary.set(0,0,0,0);        // 默认是一个空的包围矩形
    this->setVisible(true);
    this->setZValue(200);
    this->isFocused = false;            // 初始时设为false

    this->blockIsResizing = false;
    this->rectAdjust = this->blockNone;     // 一开始处于空状态


    this->setFlag(QGraphicsItem::ItemIsMovable, true);       // 可移动
    this->setFlag(QGraphicsItem::ItemIsSelectable, true);    // 可选择
    this->setFlag(QGraphicsItem::ItemIsFocusable, true);     // 可关注
}

/**
 * @Author Chaoqun
 * @brief  绘制控制器
 * @param  QPainter &painter
 * @return void
 * @date   2017/05/17
 */
void DocBlock::paintHandle(QPainter &painter)
{

    painter.setPen(Qt::black);

    painter.setRenderHint(QPainter::Antialiasing, true);  // 反走样

    // resize line
    qreal w = this->blockSize.width();
    qreal h = this->blockSize.height();

    // 简单的绘制调整大小的句柄
    painter.drawLine(w - 15, h-1,
                             w-1, h - 15);
    painter.drawLine(w - 10, h-1,
                             w-1, h - 10);
    painter.drawLine(w - 5, h-1,
                             w-1, h - 5);

    painter.setRenderHint(QPainter::Antialiasing, false);  // 关闭反走样

}


void DocBlock::resize(qreal w, qreal h)
{
    QGraphicsProxyWidget::resize(w,h);
    this->blockSize.setWidth(w);
    this->blockSize.setHeight(h);
}

void DocBlock::resize(const QSizeF &size)
{
    QGraphicsProxyWidget::resize(size);
    this->blockSize.setWidth(size.width());
    this->blockSize.setHeight(size.height());
}


/**
 * @Author Chaoqun
 * @brief  继承绘画函数
 * @param  参数
 * @return 返回值
 * @date   2017/05/18
 */
void DocBlock::paint(QPainter *painter,
                     const QStyleOptionGraphicsItem *option,
                     QWidget *widget)
{
    // 绘制的坐标系统是相对于该Widget的
    QGraphicsProxyWidget::paint(painter,option,widget);     // 先画原部分
    if(this->isFocused)
    {
        this->paintHandle(*painter);        // 画出调整框
    }

}

void DocBlock::focusInEvent(QFocusEvent *event)
{
    QGraphicsProxyWidget::focusInEvent(event);
    this->isFocused = true;
    this->update();     // 呼叫更新

}

void DocBlock::focusOutEvent(QFocusEvent *event)
{
    QGraphicsProxyWidget::focusOutEvent(event);
    this->isFocused = false;
    this->update();

}

void DocBlock::hoverMoveEvent(QGraphicsSceneHoverEvent *event)
{

    if (this->rectAdjust == blockResize
            || (this->currentStatus(event->pos()) == blockResize
                && isSelected()))
        this->setCursor(Qt::SizeFDiagCursor);
    else
        this->setCursor(Qt::ArrowCursor);

    QGraphicsProxyWidget::hoverMoveEvent(event);

}

void DocBlock::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsProxyWidget::mouseDoubleClickEvent(event);
}

/**
 * @Author Chaoqun
 * @brief  鼠标按下时处理函数
 * @param  QGraphicsSceneMouseEvent *event
 * @return void
 * @date   2017/05/19
 */
void DocBlock::mousePressEvent(QGraphicsSceneMouseEvent *event)
{

    QPointF pos = event->pos();     // 获取鼠标位置
    qDebug()<<"DocBlock Mouse Postion" << pos.x()
           << ", "<<pos.y();

//        if (event->button() == Qt::LeftButton &&
//                isInResizeArea(event->pos()))
    if (event->button() == Qt::LeftButton )
    {
        // 如果按下的是鼠标左键，检测是否是可以修改大小或位置的状态
        this->rectAdjust = this->currentStatus(event->pos());
        if(this->rectAdjust == blockMove)
            this->setAcceptDrops(true);
    }
     else
         QGraphicsProxyWidget::mousePressEvent(event);

}

/**
 * @Author Chaoqun
 * @brief  鼠标移动
 * @param  QGraphicsSceneMouseEvent *event
 * @return 返回值
 * @date   2017/05/18
 */
void DocBlock::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{

    if (this->rectAdjust == blockResize)
    {
        qreal w = event->pos().x();
        qreal h = event->pos().y();

        if (w > this->minimumWidth())
            blockSize.setWidth(w);
        if (h > this->minimumHeight())
            blockSize.setHeight(h);

        this->resize(blockSize);        // 调整大小

    }
    else if(this->rectAdjust == blockMove)
    {
//        QPointF newPos = event->pos();
//        newPos = newPos - this->blockOldPos + this->pos();  // 计算平移后坐标
//        newPos.setX(newPos.x() - this->blockOldPos.x() + this->pos().x());
//        newPos.setY(newPos.y() - this->blockOldPos.y() + this->pos().y());
//        this->setPos(newPos);    // 设置坐标
//        this->setX(event->pos().x() - this->blockOldPos.x() + this->x());
//        this->setY(event->pos().y() - this->blockOldPos.y() + this->y());

//        this->blockOldPos = event->pos();
    }
    else
    {
        QGraphicsProxyWidget::mouseMoveEvent(event);    // 调用基类的函数
    }

}

/**
 * @Author Chaoqun
 * @brief  继承鼠标释放函数
 * @param  QGraphicsSceneMouseEvent *event
 * @return void
 * @date   2017/05/18
 */
void DocBlock::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{

//    if (event->button() == Qt::LeftButton && blockIsResizing)
    if (event->button() == Qt::LeftButton &&
            this->rectAdjust != blockNone)
    {
        this->rectAdjust = blockNone;
        this->setAcceptDrops(false);
    }
    else
        QGraphicsProxyWidget::mouseReleaseEvent(event);
}

/**
 * @Author Chaoqun
 * @brief  可以实现右键菜单
 * @param  参数
 * @return 返回值
 * @date   2017/xx/xx
 */
void DocBlock::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
//    QMenu menu;
//    QAction *removeAction = menu.addAction("Remove");
//    QAction *markAction = menu.addAction("Mark");
    QGraphicsProxyWidget::contextMenuEvent(event);
}

/**
 * @Author Chaoqun
 * @brief  检查鼠标是否在重置大小区域
 * @param  const QPointF &pos
 * @return bool
 * @date   2017/05/18
 */
bool DocBlock::isInResizeArea(const QPointF &pos)
{
    return (pos.x() - this->size().width() + 15) >
            (this->size().height() - pos.y());
}

/**
 * @Author Chaoqun
 * @brief  检测当前状态是否可以移动块
 * @param  const QPointF &pos
 * @return RectAdjustStatus
 * @date   2017/05/19
 */
DocBlock::RectAdjustStatus DocBlock::currentStatus(const QPointF &pos)
{
    if((pos.x() - this->size().width() + 15) >
            (this->size().height() - pos.y()))
        return blockResize;

    // 画出可以移动的边缘
    qreal moveMargin = 5;   // 边缘多少像素内可以移动
    QRectF left(0,0,
                moveMargin,this->blockSize.height());
    QRectF right(this->blockSize.width()-moveMargin,0,
                 moveMargin,this->blockSize.height());
    QRectF top(0,0,
               this->blockSize.width(),moveMargin);
    QRectF bottom(0,this->blockSize.height() - moveMargin,
              this->blockSize.width(),moveMargin);

    if(left.contains(pos)
            || right.contains(pos)
            || top.contains(pos)
            || bottom.contains(pos))
        return blockMove;


    // 如果未得出结果，则可以移动
    return blockNone;
}

