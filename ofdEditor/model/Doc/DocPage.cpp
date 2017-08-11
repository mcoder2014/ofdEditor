#include "DocPage.h"
#include "DocLayer.h"
#include "Tool/UnitTool.h"
#include "Doc/DocBlock.h"
#include "Doc/DocTextBlock.h"
#include "Doc/DocPageScene.h"
#include "Doc/DocImageBlock.h"
#include "Doc/DocTable.h"
#include "Doc/DocPicture.h"
#include "Doc/DocGraph.h"
#include "Doc/DocPassage.h"
#include "DocImageBlock.h"
#include <QPalette>
#include <QPaintEvent>
#include <QDebug>
#include <QGraphicsProxyWidget>
#include <QPointF>
#include <QPainter>
#include <QObject>
#include <QLabel>
#include <cmath>
#include <QList>

#include <QFileDialog>
#include <QMessageBox>
#include <QPixmap>
#include "DocImageBlock.h"

// #include "DataTypes/page/CT_PageArea.h"     // 页面大小



DocPage::DocPage(QWidget *parent)
    : QGraphicsView(parent)
{
    //this->setSize(210,297);     // 默认A4纸张大小
    this->scaleFactor = 1.0;
    this->init();

//    this->setMouseTracking(true);
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

///
/// \brief DocPage::hasBlock
///     判断该页面中是否包含某个块
/// \param block
/// \return
///
bool DocPage::hasBlock(DocBlock *block)
{
    bool flag = false;
    if(this->foregroundLayer->hasBlock(block))
    {
        flag = true;
    }
    if(this->bodyLayer->hasBlock(block))
    {
        flag = true;
    }
    if(this->backgroundLayer->hasBlock(block))
    {
        flag = true;
    }
}

/**
 * @Author Chaoqun
 * @brief  获得前景层
 * @param  void
 * @return DocLayer*
 * @date   2017/06/24
 */
DocLayer *DocPage::getForegroundLayer()
{
    return this->foregroundLayer;
}

/**
 * @Author Chaoqun
 * @brief  获得正文层
 * @param  void
 * @return DocLayer*
 * @date   2017/06/24
 */
DocLayer *DocPage::getBodyLayer()
{
    return this->bodyLayer;
}

/**
 * @Author Chaoqun
 * @brief  获得背景层
 * @param  参数
 * @return DocLayer*
 * @date   2017/06/24
 */
DocLayer *DocPage::getBackgroundLayer()
{
    return this->backgroundLayer;
}

///
/// \brief DocPage::getLayer
///     获得层
/// \param layer
/// \return
///
DocLayer *DocPage::getLayer(DocPage::Layer layer)
{
    if(layer == Foreground)
    {
        return this->foregroundLayer;
    }
    else if(layer == Body)
    {
        return this->bodyLayer;
    }
    else if(layer == Background)
    {
        return this->backgroundLayer;
    }
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

//    qDebug() << "DocPage::addBlock excuted";
    this->docScene->addItem(block);       // 添加元素
//    qDebug() << "DocPage::addBlock excuted this->docScene->addItem(block);";

    // 插入DocTextBlock
    if(block->isTextBlock())
    {
        DocTextBlock* textBlock = block->getTextBlock();            // 获得TextBlock
        DocPassage* passage = this->getPassage();                   // 获得passage
        emit this->signals_insertTextBlock(textBlock);              // 发射信号

        // 转发信号给passage
        connect(textBlock,SIGNAL(signals_currentBlockFormatChanged(QTextBlockFormat)),
                passage,SIGNAL(signals_currentBlockFormatChanged(QTextBlockFormat)));
        connect(textBlock,SIGNAL(signals_currentCharFormatChanged(QTextCharFormat)),
                passage,SIGNAL(signals_currentCharFormatChanged(QTextCharFormat)));
        connect(textBlock,SIGNAL(signals_currentTextBlock(DocTextBlock*)),
                passage,SIGNAL(signals_currentTextBlock(DocTextBlock*)));
    } else if (block->isImageBlock())
    {
        DocImageBlock * imageBlock = block->getImageBlock();
        DocPassage * passage = this->getPassage();
        //emit this->signals_insertImageBlock();

        //转发给passage
        this->connect(imageBlock, SIGNAL(signals_currrentImageBlock(DocImageBlock*)),
                      passage, SIGNAL(signals_currentImageBlock(DocImageBlock*)));
    }
//    qDebug()<< "connect";

    // 分到层
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

///
/// \brief DocPage::changeBlockLayer
/// \param block
/// \param layer
///
void DocPage::changeBlockLayer(DocBlock *block, DocPage::Layer layer)
{
    if(!this->hasBlock(block))
    {
        // 如果本页中不包含此块
        return;
        qDebug() << "this page does not has this block.";
    }

    DocLayer *doclayer = block->getLayer();     // 获得层
    if(doclayer == NULL)
    {
        return;
        qDebug() << "This block has no layer";
    }

    DocLayer* goalLayer = this->getLayer(layer);    // 目标层

    doclayer->removeBlock(block);   // 从原有层中删除
    goalLayer->addBlock(block);     // 加入到目标层中

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
 * @brief  从passage中移除本页内容
 * @param  参数
 * @return 返回值
 * @date   2017/06/23
 */
void DocPage::remove()
{
    DocPassage * passage = this->getPassage();
    passage->removePage(this);
}

// 设置页面的显示大小
void DocPage::setScale(double scale)
{
    // 设置纸张大小

    this->setFixedSize(scale * this->width(),
                       scale * this->height());

//    qDebug() << "Page Size after scale:" << this->width()
//             << " " << this->height();

    // 缩放
    this->scale(scale,scale);
    this->viewport()->update();
    this->update();
}

///
/// \brief DocPage::setWorkingArea
///     设置工作区域
/// \param isUsingWorkingArea
/// \param contentWidth
/// \param contentHeight
/// \param contentX
/// \param contentY
///
void DocPage::setWorkingArea(
        bool isUsingWorkingArea,
        double contentWidth,
        double contentHeight,
        double contentX,
        double contentY)
{
    this->has_working_area = isUsingWorkingArea;
    this->working_area_width = contentWidth;
    this->working_area_height = contentHeight;
    this->working_area_x = contentX;
    this->working_area_y = contentY;
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

        QRect rect = UnitTool::getBox(
                    mapFromScene(this->oldPos),
                    mapFromScene(this->newPos));

        painter.drawRect(rect);
        painter.end();                                  // 结束

//        qDebug()<<"Moving QPainter is Drawing";
    }

    //    qDebug()<<"QPainter is Drawing";
}

///
/// \brief DocPage::contextMenuEvent
///     实现右键菜单
/// \param event
///
void DocPage::contextMenuEvent(QContextMenuEvent *event)
{
    // event->globalPos() 是相对于屏幕的
    // event->pos() 相对于页面左上角
    QMenu* menu = new QMenu(this->passage); // 让passage作为父亲

    QPoint pos = event->pos();      // 获得相对于页面的
    QList<QGraphicsItem*> items = this->items(pos);     // 获得某鼠标下的所有块
//    qDebug() << "There are"
//             << items.size()
//             << " items at position "
//             << event->pos()
//             << " scene:"
//             << this->mapToScene(event->pos());

    for(int i = 0; i < items.size(); i++)
    {
        // 将qgraphicsItem造型成为DocBlock
        if(items[i]->type() == DocBlock::Type)
        {
            // 向下造型
            DocBlock* block = qgraphicsitem_cast<DocBlock*>(items[i]);
            if(block != NULL)
            {
                menu->addMenu(block->getMenu());
            }
            else
            {
                qDebug() << "is not a Docblock class";
            }
        }

    }

    menu->exec(event->globalPos());         // 执行菜单

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
//    qDebug() << "mouse Press";

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
//            DocBlock * block = (DocBlock *)tempItem;
            DocBlock* block = qgraphicsitem_cast<DocBlock *>(tempItem);
            if(block != NULL)
            {
                 this->activeBlock = block;         // 存下 block

                 QPointF tempPoint = this->mapToScene(this->oldPos.rx(),
                                                      this->oldPos.ry());

                 if(block->cursor().shape() == Qt::SizeAllCursor)
                 {
//                     qDebug()<<" the cursor'shape is size all ";
                     this->newBlockFlag = blockMove;
                 }
                 else if (block->currentStatus(
                              block->mapFromScene(tempPoint))
                          == DocBlock::blockResize
                          && block->isImageBlock())
                 {
                     this->newBlockFlag = DocPage::blockResize;
                     this->oldPos = activeBlock->pos();
//                     qDebug() << "oh yeah";
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
//        qDebug() << "Move x:" << point.rx()
//                 << " y:" << point.ry();
        this->activeBlock->moveBy(point.rx(),point.ry());
        this->oldPos = this->mapToScene(event->pos());

        this->viewport()->update();   // 调用刷新
    }
    else if (this->newBlockFlag == DocPage::blockResize&&
             activeBlock != NULL
             && activeBlock->isImageBlock())
    {
////        qDebug() << "hahaha";
        DocImageBlock * image_block = activeBlock->getImageBlock();
        this->newPos = this->mapToScene(event->pos());
        QPointF point = this->newPos - this->oldPos;
        if (!image_block->isWidthHeightRatioLocked())
            // 如果纵横比未锁定
            this->activeBlock->resize(point.rx(),point.ry());
        else
        {
            // 如果纵横比锁定
            double ratio = image_block->getWidthHeightRatio();
            if(point.rx() < point.ry() * ratio)
                this->activeBlock->resize(point.rx(), point.rx() / ratio);
            else
                this->activeBlock->resize(point.ry() * ratio, point.ry());
        }
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

///
/// \brief DocPage::focusInEvent
///     When this page been focused ,we will emit a message
/// \param event
///
void DocPage::focusInEvent(QFocusEvent *event)
{
    QGraphicsView::focusInEvent(event);
    emit signals_page_actived(this);
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
    this->foregroundLayer->setZValue(1000);

    this->bodyLayer = new DocLayer(DocPage::Body);
    this->bodyLayer->setPage(this);
    this->bodyLayer->setZValue(0);

    this->backgroundLayer = new DocLayer(Background);
    this->backgroundLayer->setPage(this);
    this->backgroundLayer->setZValue(-1000);

//    this->setBackgroundRole(QPalette::Dark);
    this->insertBlockInfo = NULL;
    this->activeBlock = NULL;

    has_working_area = false;
    working_area_width = this->size().width();
    working_area_height = this->size().height();
    working_area_x = working_area_y = 0;

    this->initMenu();           // 初始化菜单
}

///
/// \brief DocPage::initMenu
///
void DocPage::initMenu()
{
    this->menu_insert =  new QMenu(tr("Insert"));

    // 插入文本框
    this->action_insertTextBlock = new QAction(tr("TextBlock"), NULL);

    // 插入图片
    this->action_insertImageBlock = new QAction(tr("ImageBlock"), NULL);

    // 插入表格
    this->action_insertTable = new QAction(tr("Table"), NULL);

    // 插入页
    this->action_insertPage = new QAction(tr("Page"), NULL);

    //删除本页
    this->action_deletePage = new QAction(tr("This Page"), NULL);

    // 页面设置
    this->action_pageSetting = new QAction(tr("Page Setting"), NULL);
}

///
/// \brief DocPage::getMenu
///     根绝获得的物体生成菜单
/// \param items
/// \return
///
QMenu *DocPage::getMenu(QList<QGraphicsItem *> &items)
{

}

void DocPage::addImage()
{
//    qDebug() << "Open a QDialog and load an img";
    //打开对话框，选取一个图片文件
    QString fileName = QFileDialog::getOpenFileName(this,
                                    tr("Open File"), QDir::homePath());
    if (!fileName.isEmpty()) {
        QPixmap image(fileName);
        if (image.isNull()) {
            QMessageBox::information(this, tr("OFD Editor"),
                                     tr("Cannot open file %1.").arg(fileName));
            return;
        }

        //新建一个DocBlock
        DocBlock * newBlock = new DocBlock();
        //新建一个图片框
        DocImageBlock * new_image_block = new DocImageBlock();
        newBlock->setWidget(new_image_block);
        new_image_block->setImage(image);               // 设置图片
        double page_width = this->width(), page_height = this->height();

//        qDebug() << "Page Width: " << this->width();
//        qDebug() << "Page Height: " << this->height();
//        qDebug() << "Image Width: " << image.width();
//        qDebug() << "Image Height: " << image.height();

        double ratio;
        if (image.width() > page_width || image.height() > page_height)
        {
            ratio = std::min(
                        page_width / image.width(),
                        page_height / image.height());
            ratio *= 0.8;
        }
        else ratio = 1.0;

//        qDebug() << "Ratio = " << ratio;
        newBlock->setPos(
                    (page_width - image.width() * ratio) / 2,
                    (page_height - image.height() * ratio) / 2);

        newBlock->resize(
                    image.width() * ratio,
                    image.height() * ratio);

        this->addBlock(newBlock,this->insertBlockInfo->layer);
    }
}
