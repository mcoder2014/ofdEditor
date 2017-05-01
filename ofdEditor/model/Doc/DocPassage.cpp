#include "DocPassage.h"

#include "DocPage.h"
#include "DataTypes/document/CT_DocInfo.h"

#include <QDebug>
#include <QScrollBar>

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
    this->layout->addWidget(page);      // 向layout中增加一个
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

void DocPassage::init()
{
    // 更新渲染区域
    //this->scrollArea = new QScrollArea(this);
    this->layout = new QVBoxLayout;             // 新建布局

    //this->scrollArea->setLayout(this->layout);  // 应用布局
//    this->setLayout(this->layout);
    // 将scrollArea加入到场景中
//    QVBoxLayout* temp_layout = new QVBoxLayout;
//    temp_layout->addWidget(this->scrollArea);
//    this->setLayout(temp_layout);

    this->widget = new QWidget(this);
    this->widget->setLayout(this->layout);
    this->setWidget(this->widget);
    this->widget->setVisible(true);
    this->widget->setFixedSize(1300,600);

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
    scrollBar->setValue(int(factor * scrollBar->value()
                           + ((factor -1) * scrollBar->pageStep()/2)));
}
