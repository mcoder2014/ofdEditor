#include "DocPassage.h"

#include "DocPage.h"
#include "DataTypes/document/CT_DocInfo.h"
#include "DataTypes/document/ct_commondata.h"

#include <QDebug>
#include <QDesktopWidget>
#include <QPalette>
#include <QApplication>
#include <QScrollBar>
#include <QMainWindow>
#include <Quuid>

/**
 * @Author Chaoqun
 * @brief  默认构造函数
 * @date   2017/04/30
 */
DocPassage::DocPassage(QWidget *parent)
    :docType("OFD"),version("1.0"),QScrollArea(parent)
{
    undoStack=new QUndoStack(this);
    this->scaleFactor = 1.0;            // 缩放100%
    this->docInfo = new CT_DocInfo();   // 文档元信息
//    this->commonData = new CT_CommonData();
    this->init();       // 初始化界面

//    this->addPage(new DocPage());       // 添加一个空白页面

    setAttribute(Qt::WA_DeleteOnClose);

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
    undoStack=new QUndoStack(this);
    this->version = version;
    this->docType = docType;
    this->scaleFactor = scaleFactor;
}

DocPassage::~DocPassage()
{
    // 释放根节点内容
//    if(this->docInfo != NULL)
//    {
//        delete this->docInfo;
//        this->docInfo = NULL;
//    }

    // 释放公共数据
//    if(this->commonData != NULL)
//    {
//        delete this->commonData;
//        this->commonData = NULL;
//    }

//     释放this->pages
//    int pages_length = this->pages.size();
//    for(int i = 0; i < pages_length; i++)
//    {
//        // 需要挨个释放内存空间
//        DocPage* temp = this->pages.at(i);
//        if(temp != NULL)
//        {
//            delete temp;
//            (this->pages)[i] = NULL;
//        }
//    }
//    this->pages.clear();

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
    // 添加到ScrollArea
    this->adjustWidgetSize();   // 调整大小
    this->layout->addWidget(page,0,Qt::AlignCenter);
        // 向layout中增加一页，并居中显示
    this->layout->update();             // 更新
    page->setPassage(this);             // 设置页所属的文章


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
 * @brief  在文章尾添加空白页
 * @param  DocOage* page = NULL
 * @return 返回值
 * @date   2017/05/05
 */
void DocPassage::appendNewPage(DocPage *page)
{
    if(page == NULL)
        page = new DocPage();
    this->addPage(page);            // 此处调用addPage
}

/**
 * @Author Chaoqun
 * @brief  移除指定的页
 * @param  参数
 * @return 返回值
 * @date   2017/06/20
 */
void DocPassage::removePage(int index)
{
    DocPage* page = this->pages[index];     // 获取该页面数据
    this->removePage(page);                 // 移除页面

}

/**
 * @Author Chaoqun
 * @brief  移除指定的页面
 * @param  DocPage *page
 * @return void
 * @date   2017/06/20
 */
void DocPassage::removePage(DocPage *page)
{
    int index = this->pages.indexOf(page);  // 获取到该页所在的位置
//    qDebug()<<"pages size" << this->pages.size();

    this->layout->removeWidget(page);       // 从场景中移除页面
    page->deleteLater();                    // 移除页面

    this->pages.remove(index);              // 从数据中移除页面
//    qDebug()<<"pages size" << this->pages.size();

    this->adjustWidgetSize();               // 调整大小
    this->layout->update();                 // 更新

}

/**
 * @Author Chaoqun
 * @brief  设置文档原信息
 * @param  CT_DocInfo &docInfo
 * @return void
 * @date   2017/05/23
 */
void DocPassage::setDocInfo(CT_DocInfo &docInfo)
{
    this->docInfo->copy(docInfo);
}

/**
 * @Author Chaoqun
 * @brief  测试
 * @param  参数
 * @return 返回值
 * @date   2017/06/22
 */
void DocPassage::testMessage()
{
    qDebug()<<"passage success";
}

/**
 * @Author Chaoqun
 * @brief  获得文档原信息
 * @param  void
 * @return CT_DocInfo
 * @date   2017/05/23
 */
CT_DocInfo *DocPassage::getDocInfo()
{
    return this->docInfo;
}

/**
 * @Author Chaoqun
 * @brief  获得文档中所有的页面
 * @param  void
 * @return 返回值
 * @date   2017/06/20
 */
QVector<DocPage *> &DocPassage::getPages()
{
    return this->pages;
}

/**
 * @Author Chaoqun
 * @brief  获得某一页
 * @param  int index
 * @return DocPage *
 * @date   2017/06/20
 */
DocPage *DocPassage::getPage(int index)
{
    return this->pages[index];
}

/**
 * @Author Chaoqun
 * @brief  设置CommonData,深拷贝
 * @param  CT_CommonData &commonData
 * @return void
 * @date   2017/05/23
 */
void DocPassage::setCommonData(CT_CommonData* commonData)
{
    this->commonData = commonData;
}

/**
 * @Author Chaoqun
 * @brief  获取CommonData，新建立对象
 * @param  void
 * @return CT_CommonData
 * @date   2017/05/23
 */
CT_CommonData* DocPassage::getCommonData()
{
    return this->commonData;
}

/**
 * @Author Chaoqun
 * @brief  设置 docID
 * @param  void
 * @return void
 * @date   2017/06/23
 */
void DocPassage::resetDocId()
{
    QUuid uuid = QUuid::createUuid();   // 创建uuid
    QString docId = uuid.toString();    // 转换为字符串
    qDebug() << "uuid : " << docId;

    this->docInfo->setDocID(docId);
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

/**
 * @Author Chaoqun
 * @brief  此函数可以用来实现关闭前提示保存的功能
 * @param  QCloseEvent *event
 * @return void
 * @date   2017/05/20
 */
void DocPassage::closeEvent(QCloseEvent *event)
{
    QScrollArea::closeEvent(event);
}

void DocPassage::init()
{
    this->docInfo = new CT_DocInfo();           // 新建文档 元信息
    this->resetDocId();     // 设置UUID
    this->layout = new QVBoxLayout;             // 新建布局

    // 新增widget
    this->widget = new QWidget(this);
    this->widget->setLayout(this->layout);
    this->widget->setVisible(true);
    this->widget->setBackgroundRole(QPalette::Dark);    // 背景
    this->widget->setAutoFillBackground(true);

    this->setWidget(this->widget);                      // 设置内置widget
    this->setBackgroundRole(QPalette::Dark);            // 背景

    this->horizontalWhite = 100;            // 文章两侧黑边
    this->verticalWhite = 50;               // 文章之间黑边
    this->setAlignment(Qt::AlignHCenter);   // 设置位置水平居中

    // 设置滚动条策略
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);

    adjustScrollBarRange();     // 调整进度条长度

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
                            + ((factor -1) * scrollBar->pageStep()/2)) + 1);
}

void DocPassage::adjustScrollBarRange()
{
    QSize areaSize = this->viewport()->size();  // 视窗大小
    QSize widgetSize = this->widget->size();    // 面板大小


    this->horizontalScrollBar()->setPageStep(areaSize.width());
    this->horizontalScrollBar()->setRange(0,widgetSize.width()
                                        - areaSize.width());

    this->verticalScrollBar()->setPageStep(areaSize.height());
    this->verticalScrollBar()->setRange(0,widgetSize.height()
                                        - areaSize.height());


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

//    this->widget->setMinimumSize(width, height);    // 设置内容大小
    this->widget->resize(width,height);

    // 保存计算结果
    this->widgetWidth = width;
    this->widgetHeight = height;
    this->QScrollArea::update();

    adjustScrollBarRange();     // 调整进度条长度

    // 调整滚动条位置
    adjustScrollBar(this->horizontalScrollBar(), this->scaleFactor);
    adjustScrollBar(this->verticalScrollBar(), this->scaleFactor);


    qDebug() <<"widget's Size"<<this->widget->size();
    qDebug() << "ScrollArea's Size" << this->size();

}
