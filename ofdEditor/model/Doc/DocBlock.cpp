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


    this->setFlag(QGraphicsItem::ItemIsMovable, true);  // 可移动
    this->setFlag(QGraphicsItem::ItemIsSelectable, true);    // 可选择
    this->setFlag(QGraphicsItem::ItemIsFocusable, true);  // 可关注
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

//    // 坐标系统是相对于该部件的
//    QPointF leftTop(0,0);
//    double x = leftTop.x();
//    double y = leftTop.y();

//    double width = this->size().width();
//    double height = this->size().height();
//    double halfWidth = width/2;
//    double halfHeight = height/2;

//    if(width > 50 && height > 50)
//    {
//        // 小于50像素，不要显示边缩放

//        painter.drawRect(x-5+halfWidth,y-5,20,10);        // 顶部缩放
//        painter.drawRect(x-5,y+halfHeight-5,10,20);       // 左侧缩放
//        painter.drawRect(x+halfWidth-10,y+height-5,20,10);  // 底部缩放
//        painter.drawRect(x+width-5,y+halfHeight-10,10,20);// 右侧缩放

//    }

//    painter.drawRect(x-5,y-5,10,10);                        // 左上角
//    painter.drawRect(x-5 + width,y-5,10,10);                // 右上角
//    painter.drawRect(x-5,y-5+height,10,10);                 // 左下角
//    painter.drawRect(x-5+width,y-5+height,10,10);           // 右下角


    painter.setPen(Qt::black);

    painter.setRenderHint(QPainter::Antialiasing, true);  // 反走样

    // resize line
    qreal w = this->blockSize.width();
    qreal h = this->blockSize.height();

    // 简单的绘制调整大小的句柄
    painter.drawLine(w - 20, h,
                             w, h - 20);
    painter.drawLine(w - 15, h,
                             w, h - 15);
    painter.drawLine(w - 10, h,
                             w, h - 10);

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

    if (blockIsResizing || (isInResizeArea(event->pos()) && isSelected()))
        setCursor(Qt::SizeFDiagCursor);
    else
        setCursor(Qt::ArrowCursor);

    QGraphicsProxyWidget::hoverMoveEvent(event);

}

void DocBlock::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsProxyWidget::mouseDoubleClickEvent(event);
}

void DocBlock::mousePressEvent(QGraphicsSceneMouseEvent *event)
{

    QPointF pos = event->pos();     // 获取鼠标位置
    qDebug()<<"DocBlock Mouse Postion" << pos.x()
           << ", "<<pos.y();

        if (event->button() == Qt::LeftButton &&
                isInResizeArea(event->pos()))
            blockIsResizing = true;
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

    if (blockIsResizing)
        {
            qreal w = event->pos().x();
            qreal h = event->pos().y();

            if (w > this->minimumWidth())
                blockSize.setWidth(w);
            if (h > this->minimumHeight())
                blockSize.setHeight(h);

            this->resize(blockSize);        // 调整大小

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
//    QGraphicsProxyWidget::mouseReleaseEvent(event);
    //this->setCursor(Qt::ArrowCursor);

    if (event->button() == Qt::LeftButton && blockIsResizing)
        blockIsResizing = false;
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
    return (pos.x() - this->size().width() + 20) >
            (this->size().height() - pos.y());
}

