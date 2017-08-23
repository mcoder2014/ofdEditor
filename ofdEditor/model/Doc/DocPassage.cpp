#include "DocPassage.h"

#include "DocPage.h"
#include "DataTypes/document/CT_DocInfo.h"
#include "DataTypes/document/ct_commondata.h"
#include "Widget/PageDialog.h"
#include "Core/GlobalSetting.h"
#include <QDebug>
#include <QDateTime>
#include <QDesktopWidget>
#include <QPalette>
#include <QApplication>
#include <QScrollBar>
#include <QMainWindow>
#include <Quuid>
#include <QDir>

///
/// \brief DocPassage::createNewPassage
/// 创建一个新的文章
/// \return 新的文章
///
DocPassage *DocPassage::createNewPassage()
{
    DocPassage* passage = new DocPassage(0);
    return passage;
}

///
/// \brief DocPassage::createEmptyPassage
///     创建一个新的文章，空的，用来进行读取已有文章
/// \return
///
DocPassage *DocPassage::createEmptyPassage()
{
    DocPassage* passage = new DocPassage();
    return passage;
}


/**
 * @Author Chaoqun
 * @brief  默认构造函数
 * @date   2017/04/30
 */
DocPassage::DocPassage(QWidget *parent)
    :QScrollArea(parent)
{
    this->undoStack=new QUndoStack(this);   // 用来来存储操作队列
    this->docInfo = new CT_DocInfo();       // 文档元信息
    this->initUI();                         // 初始化界面
    this->initDocInfo();                    // 初始化文章信息

//    this->addPage(new DocPage());       // 添加一个空白页面

    setAttribute(Qt::WA_DeleteOnClose);
    //    qDebug() << "DocPassage Constructor Finished.";
    this->isEdited = false;
}

///
/// \brief DocPassage::DocPassage
///     空的构造函数，用来创建空的文章
DocPassage::DocPassage()
    :QScrollArea(0)
{
    this->undoStack = new QUndoStack(this); // 撤销恢复队列
    this->docInfo = new CT_DocInfo();       // 新建文章信息
    this->initUI();                         // 初始化界面
    this->isEdited = false;

}

DocPassage::~DocPassage()
{

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
//        qDebug() << "DocPassage::addPage(DocPage* page) "
//                 <<  "You have add a NULL pointer";
        return;
    }

    // 先添加到 vector
    page->setSize(default_width, default_height);
//    qDebug() << "!!Page width in pixel = " <<  page->size().width()
//             << "!!Page height in pixel = " <<  page->size().height();
    page->setWorkingArea(
                default_using_working_area,
                default_working_width,
                default_working_height,
                default_working_x,
                default_working_y);

    this->pages.append(page);
    // 添加到ScrollArea
    this->layout->addWidget(page,0,Qt::AlignCenter);
    // 向layout中增加一页，并居中显示

    // 接收当前操作页面的更新
    this->connect(page, SIGNAL(signals_page_actived(DocPage*)),
                  this, SLOT(setCurrentActivedPage(DocPage*)));

    this->layout->update();             // 更新
    page->setPassage(this);             // 设置页所属的文章
    this->adjustWidgetSize();           // 调整大小

//    qDebug() << "You have added an new page";

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

///
/// \brief DocPassage::insertPage
/// \param page 页面
/// \param index    插入的位置
///
void DocPassage::insertPage(DocPage *page, int index)
{
    if(page == NULL)
    {
        qDebug() << "page is null";
        page = new DocPage();
    }

    page->setSize(default_width, default_height);
    page->setWorkingArea(
                default_using_working_area,
                default_working_width,
                default_working_height,
                default_working_x,
                default_working_y);
    int pos = -1;

    // 插入位置
    if(index >= this->pages.size())
    {
        // 插入到最后一位
        pos = this->layout->indexOf(
                    this->pages.operator [](this->pages.size()-1));
        pos ++;

        this->pages.append(page);

    }
    else if(index < 0)
    {
        // 插入到第一位
        pos = this->layout->indexOf(
                    this->pages.operator [](0));

        this->pages.insert(0, page);

    }
    else
    {
        pos = this->layout->indexOf(
                    this->pages.operator [](index));

        this->pages.insert(index, page);
    }

    this->layout->insertWidget(pos, page, 0, Qt::AlignCenter);

    this->connect(page, SIGNAL(signals_page_actived(DocPage*)),
                  this, SLOT(setCurrentActivedPage(DocPage*)));

    this->layout->update();
    page->setPassage(this);
    this->adjustWidgetSize();

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
//    page->deleteLater();                    // 移除页面

    this->pages.remove(index);              // 从数据中移除页面
//    qDebug()<<"pages size" << this->pages.size();

    this->adjustWidgetSize();               // 调整大小
    this->layout->update();                 // 更新

}

/**
 * @Author Chaoqun
 * @brief  设置文件路径
 * @param  QString filePath
 * @return void
 * @date   2017/06/26
 */
void DocPassage::setFilePath(QString filePath)
{
    this->filePath = filePath;
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
    this->setWindowTitle(this->docInfo->getTitle());        // 设置小窗口标题
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
    qDebug()<<"passage success ";
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
 * @brief  获得文章的uuid
 * @param  void
 * @return QString&
 * @date   2017/06/xx
 */
QString DocPassage::getUUID()
{
    return this->docInfo->getDocID();
}

/**
 * @Author Chaoqun
 * @brief  获得用来存储文档时的临时路径
 * @param  void
 * @return QString
 * @date   2017/06/26
 */
QString DocPassage::getTempSavePath()
{
    QString temp = QDir::tempPath() + "/";      // 获得系统临时路径
    QString uuid = this->getUUID();             // 获得uuid
    QString end = "-source";
    return temp + uuid + end;       // 获得临时存放路径
}

/**
 * @Author Chaoqun
 * @brief  临时存储路劲
 * @param  void
 * @return QString
 * @date   2017/06/26
 */
QString DocPassage::getTempStorePath()
{
    QString temp = QDir::tempPath() + "/";      // 获得系统临时路径
    QString uuid = this->getUUID();             // 获得uuid
    QString end = "-file";
    return temp + uuid + end;       // 获得临时存放路径
}

/**
 * @Author Chaoqun
 * @brief  获得文档中所有的页面
 * @param  void
 * @return 返回值
 * @date   2017/06/20
 */
QVector<DocPage *> *DocPassage::getPages()
{
    return &this->pages;
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
 * @brief  摘要
 * @param  参数
 * @return 返回值
 * @date   2017/06/26
 */
QString DocPassage::getFilePath()
{
    // 获得文件路径
    return this->filePath;
}

///
/// \brief DocPassage::getLastedActivedPage
/// 获得最近更新的页面
/// \return
///
DocPage *DocPassage::getLastedActivedPage()
{
    return _lastActivedPage;
}

///
/// \brief DocPassage::getLastedActivedPageIndex
/// \return
///
int DocPassage::getLastedActivedPageIndex()
{
    return this->pages.indexOf(this->_lastActivedPage);
}

///
/// \brief DocPassage::getPageIndex
/// \param page
/// \return
///
int DocPassage::getPageIndex(DocPage *page)
{
    int index = this->pages.indexOf(page);  // 查找页的序号
    return index;
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

    // 去掉字符串的链接符号  {0142d46f-60b5-47cf-8310-50008cc7cb3a}
    // 0142d46f60b547cf831050008cc7cb3a
    docId.remove(docId.length()-1, 1);
    docId.remove(docId.length() -13, 1);
    docId.remove(docId.length() -17,1);
    docId.remove(docId.length() -21, 1);
    docId.remove(docId.length() - 25,1);
    docId.remove(0,1);
//    qDebug() << "uuid : " << docId;

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
    QScrollArea::resizeEvent(event);
//    qDebug() << "DocPassage::resizeEvent Runs";
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
    qDebug() << "passage close request";
}

///
/// \brief DocPassage::initUI
///     与界面有关的初始化
///
void DocPassage::initUI()
{

    this->marginWhite = 50;                   // 边缘留下50像素的留白
    this->spacingWhite = 50;                  // 纸张之间留下的留白

    this->layout = new QVBoxLayout;                     // 新建布局
    this->layout->setMargin(this->marginWhite);         // 边缘留至少50的空白
    this->layout->setSpacing(this->spacingWhite);       // 纸张之间留下50像素的留白
    this->layout->setAlignment(Qt::AlignHCenter);       // 纸张自动居中排列

    // 新增widget
    this->widget = new QWidget();                       // 中间文档显示区域
    this->widget->setLayout(this->layout);
    this->widget->setVisible(true);
    this->widget->setBackgroundRole(QPalette::Dark);    // widget 背景
    this->widget->setAutoFillBackground(true);
    this->setWidgetResizable(true);                     // 设置ScrollArea 可以影响到内部纸张
    this->setWidget(this->widget);                      // 设置内置widget
    this->setBackgroundRole(QPalette::Dark);            // ScrollArea 的背景
    this->setAlignment(Qt::AlignHCenter);               // ScrollArea 设置位置水平居中

    this->scaleFactor = 1.0;                            // 放大缩小比例

    // 设置滚动条策略
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);

    adjustScrollBarRange();                             // 调整进度条长度

    // 设置滚动条位置
    adjustScrollBar(this->horizontalScrollBar(), 1);
    adjustScrollBar(this->verticalScrollBar(),1);

//    qDebug() << "Finished Initializing DocPassage...";
    GlobalSetting* global = GlobalSetting::getInstance();
    default_width = global->getPhysicalWidth();                //默认宽度
    default_height = global->getPhysicalHeight();              //默认高度
    default_using_working_area = false;
    default_working_width = global->getContentWidth();
    default_working_height = global->getContentHeight();
    default_working_x = global->getContentX();
    default_working_y = global->getContentY();

}

///
/// \brief DocPassage::initDocInfo
/// 用来初始化文档相关的信息
///
void DocPassage::initDocInfo()
{
    this->docInfo = new CT_DocInfo();           // 新建文档 元信息
    this->resetDocId();                         // 设置UUID

    GlobalSetting* globalSetting = GlobalSetting::getInstance();
    this->docInfo->setCreator(globalSetting->getCreator());
    this->docInfo->setCreatorVersion(globalSetting->getCreatorVersion());

    this->docType = globalSetting->getDocType();
    this->version = globalSetting->getDocVersion();

    QDateTime time = QDateTime::currentDateTime();  // 获得系统当前时间
    this->docInfo->setCreationDate( time.toString("yyyy-MM-dd"));

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
    scrollBar->update();
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

    this->horizontalScrollBar()->update();
    this->verticalScrollBar()->update();

}

/**
 * @Author Chaoqun
 * @brief  根据每一页的大小，调整显示区域面积
 * @return void
 * @date   2017/05/01
 */
void DocPassage::adjustWidgetSize()
{

    this->update();
    this->viewport()->update();
    this->QScrollArea::update();

    adjustScrollBarRange();     // 调整进度条长度

    // 调整滚动条位置
    adjustScrollBar(this->horizontalScrollBar(), this->scaleFactor);
    adjustScrollBar(this->verticalScrollBar(), this->scaleFactor);

}

void DocPassage::setDefaultPageSize(double default_width, double default_height)
{
    this->default_width = default_width;
    this->default_height = default_height;
}

void DocPassage::activatePageDialog()
{

    if( this->_lastActivedPage!= NULL)
    {

        PageDialog* page_dialog = PageDialog::getInstance();

        page_dialog->init(
                    this,
                    this->_lastActivedPage,
                    default_width,
                    default_height,
                    default_using_working_area,
                    default_working_width,
                    default_working_height,
                    default_working_x,
                    default_working_y);

        page_dialog->exec();
    }
    else
    {
        qDebug() << "You should have clicked at least one page";
    }

}

///
/// \brief DocPassage::modifyPageSize
///     修改指定页面的尺寸
/// \param choosed_pages
/// \param _width
/// \param _height
/// \param isUsingWorkingArea
/// \param contentWidth
/// \param contentHeight
/// \param contentX
/// \param contentY
///
void DocPassage::modifyPageSize(
        QVector<int> *ch_pages,
        double _width,
        double _height,
        bool isUsingWorkingArea,
        double contentWidth,
        double contentHeight,
        double contentX,
        double contentY)
{

    qDebug() << "modifyPageSize";
    DocPage * temp_page;                // 临时变量使用的页面
    for (int i = 0; i < ch_pages->size(); i++) {
        temp_page = this->pages[ch_pages->operator [](i) - 1];
        temp_page->setSize(_width, _height);
        temp_page->setWorkingArea(isUsingWorkingArea,
                                  contentWidth,
                                  contentHeight,
                                  contentX,
                                  contentY);
    }

    this->adjustWidgetSize();   // 更新尺寸
}

///
/// \brief DocPassage::modifyDefaultPageSize
///     修改默认新增加页面时的尺寸
/// \param default_width
/// \param default_height
/// \param default_isUsingWorkingArea
/// \param default_contentWidth
/// \param default_contentHeight
/// \param default_contentX
/// \param default_contentY
///
void DocPassage::modifyDefaultPageSize(
        double default_width,
        double default_height,
        bool default_isUsingWorkingArea,
        double default_contentWidth,
        double default_contentHeight,
        double default_contentX,
        double default_contentY)
{
    this->default_width = default_width;
    this->default_height = default_height;
    this->default_using_working_area = default_isUsingWorkingArea;
    this->default_working_width = default_contentWidth;
    this->default_working_height = default_contentHeight;
    this->default_working_x = default_contentX;
    this->default_working_y = default_contentY;
}

/**
 * @Author Chaoqun
 * @brief  放大页面
 * @param  参数
 * @return 返回值
 * @date   2017/06/27
 */
void DocPassage::zoomIn()
{
    this->setScale(this->scaleFactor * 1.25);
}

/**
 * @Author Chaoqun
 * @brief  缩小
 * @param  参数
 * @return 返回值
 * @date   2017/06/27
 */
void DocPassage::zoomOut()
{
    this->setScale(this->scaleFactor * 0.8);
}


/**
 * @Author Chaoqun
 * @brief  设置文章的缩放
 * @param  double scale
 * @return void
 * @date   2017/06/27
 */
void DocPassage::setScale(double scale)
{
    // 重置DocPage的尺寸
    double scale_temp = scale / this->scaleFactor;      // 获得需要缩小的增量

    // 对每一页进行缩放
    int page_length = this->pages.size();

    for(int i =0 ; i< page_length; i++)
    {
        pages[i]->setScale(scale_temp);         // 进行缩放
    }

    // 调整页面的尺寸
    adjustWidgetSize();

    this->scaleFactor = scale;

}

///
/// \brief DocPassage::setCurrentActivedPage
/// 设置当前活跃的页面
/// 因为假设调用此函数的均为此文档下的页面，因此未作检测
/// 通过信号槽调用
/// \param page
///
void DocPassage::setCurrentActivedPage(DocPage *page)
{
    this->_lastActivedPage = page;
}
