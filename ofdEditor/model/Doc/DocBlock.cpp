#include "DocBlock.h"

#include <QTimeLine>
#include <QGraphicsScene>
#include <QDebug>
#include <QCursor>
#include <QMenu>
#include <QAction>
#include <QPainter>
#include <QGraphicsEffect>
#include <QGraphicsOpacityEffect>

#include "Doc/DocTextBlock.h"
#include "Doc/DocLayer.h"
#include "Doc/DocPage.h"
#include "Doc/DocPassage.h"
#include "Doc/DocImageBlock.h"
#include "Doc/DocTable.h"

DocBlock::DocBlock(QGraphicsItem *parent , Qt::WindowFlags wFlags)
    :QGraphicsProxyWidget(parent,wFlags)
{
    this->boundary.set(0,0,0,0);        // 默认是一个空的包围矩形
    this->setVisible(true);
    this->setZValue(200);
    this->isFocused = false;            // 初始时设为false


    this->blockIsResizing = false;
    this->rectAdjust = blockNone;       // 一开始处于空状态

    this->setFlag(QGraphicsProxyWidget::ItemIsSelectable, true);    // 可选择
    this->setFlag(QGraphicsProxyWidget::ItemIsFocusable, true);     // 可关注
    this->setAcceptHoverEvents(true);
    this->textBlock = NULL;
    this->imageBlock = NULL;
    this->_table = NULL;
    this->isShowBox = false;

    this->initMenu();
}

///
/// \brief DocBlock::Layer
/// \return
///
DocPage::Layer DocBlock::Layer()
{
    if(this->layer != NULL)
        return this->layer->getLayer();
    else
        return DocPage::Body;
}

/**
 * @Author Chaoqun
 * @brief  获得所在的页
 * @param  void
 * @return DocPage*
 * @date   2017/06/21
 */
DocPage *DocBlock::getPage()
{
    DocLayer* layer = this->getLayer();
    return layer->getPage();
}

/**
 * @Author Chaoqun
 * @brief  获得文章
 * @param  void
 * @return DocPassage *
 * @date   2017/06/21
 */
DocPassage *DocBlock::getPassage()
{
    DocPage* page = this->getPage();
    if(page == NULL)
        return NULL;
    return page->getPassage();
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

/**
 * @Author Chaoqun
 * @brief  调整大小
 * @param  qreal x,qreal y
 * @return void
 * @date   2017/05/19
 */
void DocBlock::resize(qreal w, qreal h)
{
    QGraphicsProxyWidget::resize(w,h);
    this->blockSize.setWidth(w);
    this->blockSize.setHeight(h);

    emit this->signal_resize(this->x(),this->y(),w,h);
}

/**
 * @Author Chaoqun
 * @brief  重新调整大小
 * @param  参数
 * @return 返回值
 * @date   2017/05/19
 */
void DocBlock::resize(const QSizeF &size)
{
    QGraphicsProxyWidget::resize(size);
    this->blockSize.setWidth(size.width());
    this->blockSize.setHeight(size.height());
    emit this->signal_resize(this->x(),this->y(),
                           size.width(),size.height());
}

/**
 * @Author Chaoqun
 * @brief  设置Z坐标的同时存下记录
 * @param  参数
 * @return 返回值
 * @date   2017/05/27
 */
void DocBlock::setZValue(qreal z)
{
//    qDebug()<<"setZValue:" << z;
    QGraphicsProxyWidget::setZValue(z);
    this->realZValue = z;                   // 设置本地记录
}

/**
 *@Author Chaoqun
 *@brief  槽函数，设置Block坐标位置
 *@param  qreal x
 *@param  qreal y
 *@return 返回值
 *@date   2017/06/20
 */
void DocBlock::setPos(qreal x, qreal y)
{
    QGraphicsProxyWidget::setPos(x,y);
}

/**
 *@Author Chaoqun
 *@brief  从场景中移除本框
 *@param  void
 *@return void
 *@date   2017/06/20
 */
void DocBlock::remove()
{
    QGraphicsScene *scene = this->scene();      // 查找到本块所在的场景
    scene->removeItem(this);                    // 从场景中移除该组件

    if(this->layer)
    {
        this->layer->removeBlock(this);
    }

    emit this->signals_blockRemoved(this);      // 发出信号
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
    if(this->isFocused && this->rectAdjust != blockMove)
    {
        this->paintHandle(*painter);        // 画出调整框
    }

    if(this->isShowBox)
    {
        // 如果需要花边框的话，为什么要在这里画呢
        // 因为如果需要绘制边框，直接用系统的显示边框会影响block内的尺寸

        painter->setPen(Qt::black);

        // resize line
        qreal w = this->blockSize.width();
        qreal h = this->blockSize.height();

        painter->drawRect(0,0,
                          w,h);

    }

}

/**
 * @Author Chaoqun
 * @brief  被聚焦后的操作
 * @param  QFocusEvent *event
 * @return void
 * @date   2017/05/27
 */
void DocBlock::focusInEvent(QFocusEvent *event)
{
    if(!this->getPage()->getEditable())
        return;

    QGraphicsProxyWidget::focusInEvent(event);
    this->isFocused = true;

    QGraphicsProxyWidget::setZValue(2000);          // 临时呼叫到最高层
    this->update();     // 呼叫更新

}

/**
 * @Author Chaoqun
 * @brief  聚焦移出事件响应
 * @param  QFocusEvent *event
 * @return void
 * @date   2017/05/27
 */
void DocBlock::focusOutEvent(QFocusEvent *event)
{
    if(!this->getPage()->getEditable())
        return;
    QGraphicsProxyWidget::focusOutEvent(event);
    this->isFocused = false;
    QGraphicsProxyWidget::setZValue(this->realZValue);    // 还原到真实的Z坐标
    this->update();

}

/**
 * @Author Chaoqun
 * @brief  设置鼠标悬浮操作，主要负责改变鼠标样式
 * @param  QGraphicsSceneHoverEvent *event
 * @return void
 * @date   2017/05/19
 */
void DocBlock::hoverMoveEvent(QGraphicsSceneHoverEvent *event)
{
//    qDebug() << "Hovering.";

    if (this->rectAdjust == blockResize
            || (this->currentStatus(event->pos()) == blockResize
                && this->isFocused))
    {
        this->setCursor(Qt::SizeFDiagCursor);           // 设置为角落缩放
    }
    else if(this->rectAdjust == blockMove
            || (this->currentStatus(event->pos()) == blockMove
                && this->isFocused))
    {
        this->setCursor(Qt::SizeAllCursor);             // 设置为移动样式
    }
    else if(this->rectAdjust == blockNone)
    {
            if(this->isTextBlock())
            {
                // 如果是文本框
                this->setCursor(Qt::IBeamCursor);
            }
    }

        QGraphicsProxyWidget::hoverMoveEvent(event);    // 调用父类的调整函数

}

void DocBlock::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{

//    this->clearFocus();
//    this->setFocus();

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
//    qDebug() << "???";
    if (this->rectAdjust == blockResize)
    {
//        qDebug() << "???";
        qreal w = event->pos().x();
        qreal h = event->pos().y();

        if (w > this->minimumWidth())
            blockSize.setWidth(w);
        if (h > this->minimumHeight())
            blockSize.setHeight(h);

//        qDebug() << "Block Size Width = " << blockSize.width();
//        qDebug() << "Block Size Height = " << blockSize.height();
        this->resize(blockSize);        // 调整大小
        this->setCursor(Qt::SizeFDiagCursor);
    }
        QGraphicsProxyWidget::mouseMoveEvent(event);    // 调用基类的函数

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

    if(event->button() == Qt::LeftButton)
    {
        this->rectAdjust = this->currentStatus(event->pos());
    }

    QGraphicsProxyWidget::mouseReleaseEvent(event);
}

/**
 * @Author Chaoqun
 * @brief  设置内容
 * @param  QWidget *widget
 * @return void
 * @date   2017/06/20
 */
void DocBlock::setWidget(QWidget *widget)
{
    QGraphicsProxyWidget::setWidget(widget);
}

/**
 * @Author Chaoqun
 * @brief  多台的函数,方便在加入DocTextBlock时建立一些信号槽
 * @param  QWidget *widget
 * @return 返回值
 * @date   2017/06/20
 */
void DocBlock::setWidget(DocTextBlock *textBlock)
{
    // 建立connect
//    connect(textBlock,SIGNAL(signals_remove(DocTextBlock*)),
//            this,SLOT(remove()));                // 和块做移除连接
//    connect(textBlock,SIGNAL(signals_setZValue(qreal)),
//            this,SLOT(setZValue(qreal)));       // 建立设置Z值的信号连接

    textBlock->setBlock(this);                  // 给DocTextBlock设置引用
    this->textBlock = textBlock;                // 给DocBlock设置引用

    QGraphicsProxyWidget::setWidget(textBlock);
    qDebug() <<"set widget text";
}


/**
 * @Author Pan
 * @brief  em.....
 * @param  DocImageBlock * imageBlock
 * @return void
 * @date   2017/06/24
 */
void DocBlock::setWidget(DocImageBlock * imageBlock)
{
    imageBlock->setBlock(this);
    this->imageBlock = imageBlock;
    QGraphicsProxyWidget::setWidget(imageBlock);
}

///
/// \brief DocBlock::setWidget
///     设置对象为表格
/// \param table
///
void DocBlock::setWidget(DocTable *table)
{
    table->setBlock(this);
    this->_table = table;
    QGraphicsProxyWidget::setWidget(table);
    qDebug() << "set Widget table";
}

///
/// \brief DocBlock::sizeAndPositionDialog
///     大小位置调整窗口
///
void DocBlock::sizeAndPositionDialog()
{

}

///
/// \brief DocBlock::setShowBoundaryBox
///     设置是否显示包围的边框
/// \param flag
///
void DocBlock::setShowBoundaryBox(bool flag)
{
    this->isShowBox = flag;
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


void DocBlock::initMenu()
{
    // 移动到前景层
    this->action_foreground = new QAction(tr("Foreground"), NULL);
    this->action_foreground->setCheckable(true);
    this->connect(this->action_foreground, SIGNAL(triggered(bool)),
                  this, SLOT(moveToForeground()));

    // 移动到正文层
    this->action_body = new QAction(tr("Body"), NULL);
    this->action_body->setCheckable(true);
    this->connect(this->action_body, SIGNAL(triggered(bool)),
                  this, SLOT(moveToBody()));

    // 移动到背景层
    this->action_background = new QAction(tr("Background"), NULL);
    this->action_background->setCheckable(true);

    this->connect(this->action_background, SIGNAL(triggered(bool)),
                  this, SLOT(moveToBackground()));

    // 删除
    this->action_delete = new QAction(tr("Delete"), NULL);

    this->connect(this->action_delete, SIGNAL(triggered(bool)),
                  this, SLOT(remove()));

    // 大小和位置调整
    this->action_geometry = new QAction(tr("size and position"), NULL);

    this->connect(this->action_geometry, SIGNAL(triggered(bool)),
                  this, SLOT(sizeAndPositionDialog()));
}

///
/// \brief DocBlock::moveToForeground
///
void DocBlock::moveToForeground()
{
    if(this->Layer() != DocPage::Foreground)
    {
        this->getPage()->changeBlockLayer(this, DocPage::Foreground);
    }
}

///
/// \brief DocBlock::moveToBody
///
void DocBlock::moveToBody()
{
    if(this->Layer() != DocPage::Body)
    {
        this->getPage()->changeBlockLayer(this, DocPage::Body);
    }
}

///
/// \brief DocBlock::moveToBackground
///
void DocBlock::moveToBackground()
{
    if(this->Layer() != DocPage::Background)
    {
        this->getPage()->changeBlockLayer(this, DocPage::Background);
    }
}

/**
 * @Author Chaoqun
 * @brief  检测当前状态是否可以移动块
 * @param  const QPointF &pos
 * @return RectAdjustStatus
 * @date   2017/05/19
 */
DocBlock::RectAdjustStatus DocBlock::currentStatus( QPointF pos)
{
    if (isTextBlock() || isTableBlock())
    {
        if((pos.x() - this->size().width() + 15) >
                (this->size().height() - pos.y()))
        {
            return blockResize;
        }
        // 画出可以移动的边缘
        qreal moveMargin = 10;   // 边缘多少像素内可以移动
        QRectF left(-5,0,
                    moveMargin,this->blockSize.height());
        QRectF right(this->blockSize.width()-moveMargin+5,0,
                     moveMargin,this->blockSize.height());
        QRectF top(0,-5,
                   this->blockSize.width(),moveMargin);
        QRectF bottom(0,this->blockSize.height() - moveMargin+5,
                  this->blockSize.width(),moveMargin);

        if(left.contains(pos)
                || right.contains(pos)
                || top.contains(pos)
                || bottom.contains(pos))
        {
            return blockMove;
        }

        // 如果未得出结果，则默认无操作
        return blockNone;
    }
    else if (isImageBlock())
    {
//        qDebug() << "Is Image Block.";
        QRectF rec(0, 0,
                   this->blockSize.width(),
                   this->blockSize.height());
        if((pos.x() - this->size().width() + 15) >
                (this->size().height() - pos.y()))
        {
            //qDebug() << "Mouse resizing.";
            return blockResize;
        } else if (rec.contains(pos))
        {
//            qDebug() << "Is moving Image Block";
            return blockMove;
        }

    }
}

///
/// \brief DocBlock::getMenu
///     获得菜单
/// \return
///
QMenu *DocBlock::getMenu()
{
    QMenu * menu = new QMenu();     // 新建菜单
    QMenu * menu_layer = new QMenu(tr("Layer"));    // 子菜单-层
    menu_layer->addAction(this->action_foreground);
    menu_layer->addAction(this->action_body);
    menu_layer->addAction(this->action_background);

    // 设置提醒状态
    if(this->Layer() == DocPage::Foreground)
    {
        this->action_foreground->setChecked(true);
        this->action_body->setChecked(false);
        this->action_background->setChecked(false);
    }
    else if(this->Layer() == DocPage::Body)
    {
        this->action_foreground->setChecked(false);
        this->action_body->setChecked(true);
        this->action_background->setChecked(false);
    }
    else if(this->Layer() == DocPage::Background)
    {
        this->action_foreground->setChecked(false);
        this->action_body->setChecked(false);
        this->action_background->setChecked(true);
    }

    if(this->isTextBlock())
    {
        // 文本框
        menu = this->textBlock->getMenu();
        menu->setTitle(this->textBlock->getType());

        menu->addSeparator();
        menu->addAction(this->action_geometry);
        menu->addMenu(menu_layer);
        menu->addAction(this->action_delete);

    }
    else if(this->isTableBlock())
    {
        // 这个是表格
        menu = this->_table->getMenu();
        menu->setTitle(this->_table->getType());

        menu->addSeparator();
        menu->addAction(this->action_geometry);
        menu->addMenu(menu_layer);
        menu->addAction(this->action_delete);
    }
    else if(this->isImageBlock())
    {
        // 如果是图片
        menu = this->imageBlock->getMenu();
        menu->setTitle(this->imageBlock->getType());

        menu->addSeparator();
        menu->addMenu(menu_layer);
        menu->addAction(this->action_delete);
    }

    return menu;
}

/**
 * @Author Chaoqun
 * @brief  判断是其中装的是否是DocTextBlock
 * @param  void
 * @return bool
 * @date   2017/06/23
 */
bool DocBlock::isTextBlock()
{
    if(this->textBlock == NULL)
    {
        return false;
    }
    else
    {
        return true;
    }
}

/**
 * @Author Chaoqun
 * @brief  获得DocTextBlock
 * @param  void
 * @return DocTextBlock*
 * @date   2017/06/23
 */
DocTextBlock *DocBlock::getTextBlock()
{
    if (isTextBlock())
        return this->textBlock;
    return NULL;
}

/**
 * @Author Pan
 * @brief  判断是其中装的是否是DocImageBlock
 * @param  void
 * @return bool
 * @date   2017/06/23
 */
bool DocBlock::isImageBlock()
{
    return imageBlock != NULL;
}

/**
 * @Author Pan
 * @brief  获得DocImageBlock
 * @param  void
 * @return void
 * @date   2017/06/23
 */
DocImageBlock * DocBlock::getImageBlock()
{
    if (isImageBlock())
        return imageBlock;
    return NULL;
}

bool DocBlock::isTableBlock()
{
    return this->_table != NULL;
}

DocTable *DocBlock::getTableBlock()
{
    if(this->isTableBlock())
        return this->_table;
    return NULL;
}
