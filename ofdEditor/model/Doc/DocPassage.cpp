#include "DocPassage.h"

#include "DocPage.h"
#include "DataTypes/document/CT_DocInfo.h"

#include <QDebug>
#include <QDesktopWidget>
#include <QPalette>
#include <QApplication>
#include <QScrollBar>
#include <QMainWindow>

//#include "DataTypes/document/CT_CommonData.h"

/**
 * @Author Chaoqun
 * @brief  默认构造函数
 * @date   2017/04/30
 */
DocPassage::DocPassage(QWidget *parent)
    :docType("OFD"),version("1.0"),QScrollArea(parent)
{
    this->scaleFactor = 1.0;            // 缩放100%
    this->docInfo = new CT_DocInfo();
//    this->commonData = new CT_CommonData();
    this->init();       // 初始化界面

    //this->addPage(new DocPage());       // 添加一个空白页面

}

/**
 * @Author Chaoqun
 * @brief  含参构造函数
 * @date   2017/05/01
 */
DocPassage::DocPassage(QWidget *parent,
                       QString version, QString docType, double scaleFactor)
    :QScrollArea(parent)
{
    this->version = version;
    this->docType = docType;
    this->scaleFactor = scaleFactor;
}

DocPassage::~DocPassage()
{
    // 释放根节点内容
    if(this->docInfo != NULL)
    {
        delete this->docInfo;
        this->docInfo = NULL;
    }

    // 释放公共数据
//    if(this->commonData != NULL)
//    {
//        delete this->commonData;
//        this->commonData = NULL;
//    }

    // 释放this->pages
    int pages_length = this->pages.size();
    for(int i = 0; i < pages_length; i++)
    {
        // 需要挨个释放内存空间
        DocPage* temp = this->pages.at(i);
        if(temp != NULL)
        {
            delete temp;
            (this->pages)[i] = NULL;
        }
    }
    this->pages.clear();

}

/**
 * @Author Chaoqun
 * @brief  向该面板中，增加很多区域
 * @param  参数
 * @return 返回值
 * @date   2017/05/01
 */
void DocPassage::addPage(DocPage *page)
{
    if(page == NULL)
    {
        qDebug() << "DocPassage::addPage(DocPage* page) "
                 <<  "You have add a NULL pointer";
        return;
    }

    // 先添加到 vector
    this->pages.append(page);

    // 再添加到ScrollArea

    this->adjustWidgetSize();   // 调整大小

    this->layout->addWidget(page,0,Qt::AlignCenter);
        // 向layout中增加一页，并居中显示
    this->layout->update();             // 更新

    qDebug() << "You have added an new page";

}

/**
 * @Author Chaoqun
 * @brief  向场景中增加很多页面
 * @param  QVector<DocPage *>& passage
 * @return void
 * @date   2017/05/01
 */
void DocPassage::addPassage(QVector<DocPage *>& passage)
{
    int passage_length = passage.size();
    for(int i = 0; i < passage_length; i++)
    {
        this->addPage(passage[i]);
    }
}

/**
 * @Author Chaoqun
 * @brief  设置重置响应事件，窗口大小发生调整，将会调用这个函数u
 * @param  QResizeEvent *event
 * @return void
 * @date   2017/05/02
 */
void DocPassage::resizeEvent(QResizeEvent *event)
{
    this->adjustWidgetSize();       // 调整整体大小
    qDebug() << "DocPassage::resizeEvent Runs";
}

void DocPassage::init()
{
    this->layout = new QVBoxLayout;             // 新建布局

    // 新增widget
    this->widget = new QWidget(this);
    this->widget->setLayout(this->layout);
    this->widget->setVisible(true);
    this->widget->setBackgroundRole(QPalette::Dark);    // 背景
    this->widget->setAutoFillBackground(true);

    this->setWidget(this->widget);                      // 设置内置widget
    this->widget->show();

    this->horizontalWhite = 100;            // 文章两侧黑边
    this->verticalWhite = 50;               // 文章之间黑边

    // 设置滚动条策略
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);

    // 设置滚动条位置
    adjustScrollBar(this->horizontalScrollBar(), 1);
    adjustScrollBar(this->verticalScrollBar(),1);

}

/**
 * @Author Chaoqun
 * @brief  调整滑动条
 * @param  QScrollBar *scrollBar
 * @param  Double factor
 * @return 返回值
 * @date   2017/05/01
 */
void DocPassage::adjustScrollBar(QScrollBar *scrollBar, double factor)
{
    // 设置滚动条位置
    scrollBar->setValue(int(factor * scrollBar->value()
                            + ((factor -1) * scrollBar->pageStep()/2)));
}

/**
 * @Author Chaoqun
 * @brief  根据每一页的大小，调整显示区域面积
 * @return void
 * @date   2017/05/01
 */
void DocPassage::adjustWidgetSize()
{
    // 计算页面大小
    int width = 0;
    int height = 0;

    int length = this->pages.size();
    for(int i = 0; i <length; i++)
    {
        if(width < this->pages[i]->getSize().width())
        {
           width =  this->pages[i]->getSize().width();
        }

        height += verticalWhite + this->pages[i]->getSize().height();
    }

    height += verticalWhite;
    width += 2*horizontalWhite;

    this->widget->setMinimumSize(width, height);    // 设置内容大小

    // 保存计算结果
    this->widgetWidth = width;
    this->widgetHeight = height;

    // 调整滚动条位置
    adjustScrollBar(this->horizontalScrollBar(), this->scaleFactor);
    adjustScrollBar(this->verticalScrollBar(), this->scaleFactor);

}
