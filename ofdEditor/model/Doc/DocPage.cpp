#include "DocPage.h"
#include "DocLayer.h"
#include "Tool/UnitTool.h"
#include "Doc/DocBlock.h"

#include <QPalette>
#include <QPaintEvent>
#include <QDebug>

// #include "DataTypes/page/CT_PageArea.h"     // 页面大小

DocPage::DocPage(QWidget *parent)
    :QGraphicsView(parent)
{
    this->setSize(210,297);
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
    qDebug() << "DocPage::addBlock excuted this->docScene->addItem(block);";

    switch (layer) {
    case Body:
        this->bodyLayer->addBlock(block);
        break;
    case Foreground:
        this->foregroundLayer->addBlock(block);
        break;
    case Background:
        this->backgroundLayer->addBlock(block);
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
 * @brief  摘要
 * @param  参数
 * @return 返回值
 * @date   2017/05/02
 */
void DocPage::paintEvent(QPaintEvent *event)
{

}


/**
 * @Author Chaoqun
 * @brief  初始化DocPage
 * @date   2017/05/06
 */
void DocPage::init()
{
    this->docScene = new QGraphicsScene(); // 新建
    this->setScene(this->docScene);        // 设置场景

    this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);



    // 新建三个层
    this->foregroundLayer = new DocLayer(Foreground);
    this->bodyLayer = new DocLayer(DocPage::Body);
    this->backgroundLayer = new DocLayer(Background);


}
