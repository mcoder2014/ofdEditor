#include "DocPage.h"
#include "DocLayer.h"
#include "Tool/UnitTool.h"
#include "Doc/DocBlock.h"
#include "Doc/DocTextBlock.h"
#include "Doc/DocPageScene.h"
#include "Doc/DocImage.h"
#include "Doc/DocTable.h"
#include "Doc/DocPicture.h"
#include "Doc/DocGraph.h"
#include "Doc/DocPassage.h"

#include <QPalette>
#include <QPaintEvent>
#include <QDebug>
#include <QGraphicsProxyWidget>
#include <QPointF>
#include <QPainter>
#include <QObject>

// #include "DataTypes/page/CT_PageArea.h"     // 页面大小


DocPage::DocPage(QWidget *parent)
    :QGraphicsView(parent)
{
    this->setSize(210,297);     // 默认A4纸张大小
    this->scaleFactor = 1.0;
    this->init();

}

DocPage::DocPage(double width,
                 double height, double scaleFactor, QWidget *parent)
    :QGraphicsView(parent)
{
    this->setSize(width,height);     // 设置widget大小
    this->scaleFactor = scaleFactor;
    this->setVisible(true);
    this->init();
}

DocPage::~DocPage()
{
    // area空间释放

}

DocPassage *DocPage::getPassage()
{
    return this->passage;
}

/**
 * @Author Chaoqun
 * @brief  设置文章引用
 * @param  DocPassage *passage
 * @return void
 * @date   2017/06/20
 */
void DocPage::setPassage(DocPassage *passage)
{
    this->passage = passage;    // 设置 passage
}


/**
 * @Author Chaoqun
 * @brief  调整页面的大小
 * @param  double width, double height
 * @return 返回值
 * @date   2017/05/01
 */
void DocPage::setSize(double width, double height)
{
    // 保存mm单位大小
    this->width_mm = width;
    this->height_mm = height;

    this->setFixedSize(UnitTool::mmToPixel(width),
                       UnitTool::mmToPixel(height));   // 设置页面大小
    this->setSceneRect(0,0,
                       UnitTool::mmToPixel(width),
                       UnitTool::mmToPixel(height) );
    this->setBackgroundRole(QPalette::BrightText);          // 背景颜色
    this->setAutoFillBackground(true);
}

/**
 * @Author Chaoqun
 * @brief  获得页面的大小-像素大小
 * @return QSize
 * @date   2017/05/01
 */
QSize DocPage::getSize()
{
    return QSize(UnitTool::mmToPixel(width_mm),
                 UnitTool::mmToPixel(height_mm));
}

/**
 * @Author Chaoqun
 * @brief  添加一个新的块到页面之中
 * @param  DocBlock* block  具体的块
 * @param  Layer layer      在哪层
 * @return 返回值
 * @date   2017/05/06
 */
void DocPage::addBlock(DocBlock *block, DocPage::Layer layer)
{

    qDebug() << "DocPage::addBlock excuted";
    this->docScene->addItem(block);       // 添加元素
//    qDebug() << "DocPage::addBlock excuted this->docScene->addItem(block);";

    switch (layer) {
    case Body:
        this->bodyLayer->addBlock(block);
        block->setZValue(0);
        break;
    case Foreground:
        this->foregroundLayer->addBlock(block);
        block->setZValue(1000);
        break;
    case Background:
        this->backgroundLayer->addBlock(block);
        block->setZValue(-1000);
        break;
    default:
        break;
    }


}

/**
 * @Author Chaoqun
 * @brief  将item添加到scene中
 * @param  QGraphicsItem *item
 * @return void
 * @date   2017/05/13
 */
void DocPage::addItem(QGraphicsItem *item)
{
    this->docScene->addItem(item);
}

/**
 * @Author Chaoqun
 * @brief  传递接口
 * @param  参数
 * @return 返回值
 * @date   2017/05/03
 */
QGraphicsProxyWidget *DocPage::addWidget(QWidget *widget,
                                         Qt::WindowFlags wFlags)
{
    return this->docScene->addWidget(widget,wFlags);
}

/**
 * @Author Chaoqun
 * @brief  设置下一个要插入的块的信息，用来传参，如后期有需要，
 *         可以拓展假如更多信息
 * @param  InsertBlockInfo &blockInfo
 * @return void
 * @date   2017/05/16
 */
void DocPage::setInsertBlockType(InsertBlockInfo &blockInfo)
{
    if(this->insertBlockInfo == NULL)
    {
        this->insertBlockInfo = new InsertBlockInfo(DocPage::Body,
                                                    DocPage::text);
    }

    this->insertBlockInfo->layer = blockInfo.layer;     // 层
    this->insertBlockInfo->type = blockInfo.type;       // 类型
    //    qDebug()<<"Set InsertBlockInfo successfully!";
}


/**
 * @Author Chaoqun
 * @brief  绘画事件重载
 * @param  QPaintEvent *event
 * @return void
 * @date   2017/05/02
 */
void DocPage::paintEvent(QPaintEvent *event)
{
    QGraphicsView::paintEvent(event);           // 执行原绘图操作

    // 画出动态拖拽的框
    if(this->newBlockFlag == drawMove)
    {
        QPainter painter(this->viewport());     // 坑，要画在viewport上
        painter.setPen(Qt::blue);

        QRectF rect = UnitTool::getBox(this->oldPos,this->newPos);

        painter.drawRect(rect);
        painter.end();                                  // 结束

//        qDebug()<<"Moving QPainter is Drawing";
    }

//    qDebug()<<"QPainter is Drawing";
}

/**
 * @Author Chaoqun
 * @brief  鼠标双击事件重载
 * @param  QGraphicsSceneMouseEvent *event
 * @return void
 * @date   2017/05/15
 */
void DocPage::mouseDoubleClickEvent(QMouseEvent *event)
{
    QGraphicsView::mouseDoubleClickEvent(event);
}

/**
 * @Author Chaoqun
 * @brief  鼠标点击事件重载
 * @param  QGraphicsSceneMouseEvent *event
 * @return void
 * @date   2017/05/15
 */
void DocPage::mousePressEvent(QMouseEvent *event)
{

    // 如果是加入新块状态
    if(this->newBlockFlag == draw)
    {
        this->newBlockFlag = drawMove;        // 进入移动状态

        this->oldPos = this->mapToScene(event->pos());  // 记录鼠标
        this->newPos = this->mapToScene(event->pos());  // 记录鼠标
//        qDebug() << " QMouseEvent * pos: x:" << event->x()
//                 << " y:" << event->y()
//                 << "After convert pos x: " << this->oldPos.x()
//                 << "y: "<< this->oldPos.y();
    }
    else if(this->newBlockFlag == none)
    {
        // 先找出鼠标是否有点击
        this->oldPos = this->mapToScene(event->pos());
        QList<QGraphicsItem *> items = this->scene()->items(this->oldPos);
        if( items.size()> 0)
        {
            QGraphicsItem* tempItem = items[0];

            // 不清楚为什么，造型函数在这里编译出错，因此我采取了强制类型转换
//            DocBlock * block = qobject_cast<DocBlock *>(tempItem);
            DocBlock * block = (DocBlock *)tempItem;

            if(block != NULL)
            {
                 this->activeBlock = block;         // 存下 block

                 QPointF tempPoint = this->mapToScene(this->oldPos.rx(),
                                                      this->oldPos.ry());
//                 qDebug() <<"temp Point"<<tempPoint.rx()
//                         << ","<<tempPoint.ry();

                 if(block->currentStatus(
                             block->mapFromScene(tempPoint))
                         == DocBlock::blockMove)
                 {
//                     qDebug()<<" Accepted Signal is blockMove";
                     this->newBlockFlag = blockMove;
                 }
                 else
                 {
//                     qDebug()<<"not Move";
                 }
            }
            else
            {
//                qDebug() << "block == NULL";
            }
        }
        else
        {
//            qDebug() << "items.size:" <<items.size();
        }



    }

    QGraphicsView::mousePressEvent(event);
    this->viewport()->update();
}

/**
 * @Author Chaoqun
 * @brief  鼠标移动事件重载
 * @param  QGraphicsSceneMouseEvent *event
 * @return void
 * @date   2017/05/15
 */
void DocPage::mouseMoveEvent(QMouseEvent *event)
{

    QGraphicsView::mouseMoveEvent(event);

    if(this->newBlockFlag == drawMove)
    {
        this->newPos = this->mapToScene(event->x(),event->y());
//        this->update();             // 这个接口用不了不在文档里说明！！！

        this->viewport()->update();   // 调用刷新
    }
    else if(this->newBlockFlag == blockMove)
    {
        this->newPos = this->mapToScene(event->pos());
        if(this->activeBlock == NULL)
        {
            this->newBlockFlag = none;
            this->viewport()->update();   // 调用刷新
            return;     // 终止函数
        }

        QPointF point = this->newPos - this->oldPos;
        this->activeBlock->moveBy(point.rx(),point.ry());
        this->oldPos = this->mapToScene(event->pos());

        this->viewport()->update();   // 调用刷新
    }

}

/**
 * @Author Chaoqun
 * @brief  鼠标释放事件重载
 * @param  QGraphicsSceneMouseEvent *event
 * @return void
 * @date   2017/05/15
 */
void DocPage::mouseReleaseEvent(QMouseEvent *event)
{

    if(this->newBlockFlag == drawMove)
    {
        // 如果是释放

        QPointF newPos = this->mapToScene(event->x(),event->y());

        // 插入新块
        DocBlock * newBlock = new DocBlock();
        if(this->insertBlockInfo->type == text)
        {
            newBlock->setWidget(new DocTextBlock());    // 插入文字框
        }
        else if(this->insertBlockInfo->type == image)
        {
//            newBlock->setWidget(new DocImage());      // 插入图片框
        }
        else if(this->insertBlockInfo->type == table)
        {
            newBlock->setWidget(new DocTable());        // 插入表格框
        }

        // 设置位置大小
        QRectF rect = UnitTool::getBox(this->oldPos,newPos);
        newBlock->setPos(rect.x(),rect.y());
        newBlock->resize(rect.width(),rect.height());

        this->addBlock(newBlock,this->insertBlockInfo->layer);  // 插入到页中

    }

    if(this->newBlockFlag != none)
    {
        this->newBlockFlag = none;
        this->viewport()->setCursor(Qt::ArrowCursor);   // 恢复鼠标
    }

    this->viewport()->update();     // 释放时，刷新一下

    QGraphicsView::mouseReleaseEvent(event);
}


/**
 * @Author Chaoqun
 * @brief  初始化DocPage
 * @date   2017/05/06
 */
void DocPage::init()
{
    this->setWindowFlags(Qt::Widget);

    this->docScene = new DocPageScene(); // 新建
    this->setScene(this->docScene);        // 设置场景

    this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);



    // 新建三个层
    this->foregroundLayer = new DocLayer(Foreground);
    this->foregroundLayer->setPage(this);

    this->bodyLayer = new DocLayer(DocPage::Body);
    this->bodyLayer->setPage(this);

    this->backgroundLayer = new DocLayer(Background);
    this->backgroundLayer->setPage(this);

//    this->setBackgroundRole(QPalette::Dark);
    this->insertBlockInfo = NULL;
}
