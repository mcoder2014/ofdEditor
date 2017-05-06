#include "DocBlock.h"

#include <QTimeLine>
#include <QGraphicsScene>
#include <QDebug>

DocBlock::DocBlock(QGraphicsItem *parent)
    :QGraphicsProxyWidget(parent,Qt::Widget)
{
    this->boundary.set(0,0,0,0);        // 默认是一个空的包围矩形
    this->setVisible(true);
    this->setZValue(200);

}

void DocBlock::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    qDebug() << "被鼠标点了一下 ";
}

