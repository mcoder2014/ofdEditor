#include "DocBlock.h"

#include <QTimeLine>
#include <QGraphicsScene>
#include <QDebug>
#include <QCursor>
#include <QMenu>
#include <QAction>

DocBlock::DocBlock(QGraphicsItem *parent , Qt::WindowFlags wFlags)
    :QGraphicsProxyWidget(parent,wFlags)
{
    this->boundary.set(0,0,0,0);        // 默认是一个空的包围矩形
    this->setVisible(true);
    this->setZValue(200);

}

void DocBlock::focusInEvent(QFocusEvent *event)
{
    QGraphicsProxyWidget::focusInEvent(event);
}

void DocBlock::focusOutEvent(QFocusEvent *event)
{
    QGraphicsProxyWidget::focusOutEvent(event);
}

void DocBlock::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsProxyWidget::mouseDoubleClickEvent(event);
}

void DocBlock::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsProxyWidget::mousePressEvent(event);

    qDebug() << "被鼠标点了一下 "<<(event->pos()).x()
             << "," << event->pos().y();

    QGraphicsScene * scene = this->scene();
    qDebug() << scene->width() << ", "
             << scene->height() ;

    //this->setCursor(Qt::ClosedHandCursor);
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

