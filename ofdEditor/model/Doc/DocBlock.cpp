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
//    QRectF rect = this->geometry();             // 获得该部件的大小
    //QPointF leftTop = this->pos();           // 左上角的点
//    QPointF leftTop = this->mapToScene(leftTop_);

    // 坐标系统是相对于该部件的
    QPointF leftTop(0,0);
    double x = leftTop.x();
    double y = leftTop.y();

    double width = this->size().width();
    double height = this->size().height();
    double halfWidth = width/2;
    double halfHeight = height/2;

    if(width > 50 && height > 50)
    {
        // 小于50像素，不要显示边缩放

        painter.drawRect(x-5+halfWidth,y-5,20,10);        // 顶部缩放
        painter.drawRect(x-5,y+halfHeight-5,10,20);       // 左侧缩放
        painter.drawRect(x+halfWidth-10,y+height-5,20,10);  // 底部缩放
        painter.drawRect(x+width-5,y+halfHeight-10,10,20);// 右侧缩放

    }

    painter.drawRect(x-5,y-5,10,10);                        // 左上角
    painter.drawRect(x-5 + width,y-5,10,10);                // 右上角
    painter.drawRect(x-5,y-5+height,10,10);                 // 左下角
    painter.drawRect(x-5+width,y-5+height,10,10);           // 右下角
}

/**
 * @Author Chaoqun
 * @brief  用来判断是否可以调节大小，属于哪个类型的调节大小
 * @param  QPointF point
 * @return 返回值
 * @date   2017/xx/xx
 */
DocBlock::ResizeStatus DocBlock::isResizeable(const QPointF &point)
{
    return normal;
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
//    this->grabMouse();  // 实时捕获鼠标
//    qDebug() << "DocBlock is focused!";
}

void DocBlock::focusOutEvent(QFocusEvent *event)
{
    QGraphicsProxyWidget::focusOutEvent(event);
    this->isFocused = false;
    this->update();
//    this->ungrabMouse();     // 释放鼠标
//    qDebug() << "DocBlock is out of focus";
}

void DocBlock::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsProxyWidget::mouseDoubleClickEvent(event);
}

void DocBlock::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsProxyWidget::mousePressEvent(event);
    QPointF pos = event->pos();     // 获取鼠标位置
    qDebug()<<"DocBlock Mouse Postion" << pos.x()
           << ", "<<pos.y();
//    this->setCursor(Qt::EpenHandCursor);
}

void DocBlock::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsProxyWidget::mouseMoveEvent(event);

    //this->setCursor(Qt::OpenHandCursor);

}

void DocBlock::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsProxyWidget::mouseReleaseEvent(event);
    //this->setCursor(Qt::ArrowCursor);
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

