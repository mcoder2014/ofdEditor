#ifndef DOCPASSAGE_H
#define DOCPASSAGE_H

#include "model_global.h"       // 导出lib使用
#include <QGraphicsView>

#include <QVector>
#include <QString>
#include <QWidget>
#include <QScrollArea>
#include <QVBoxLayout>      // 树状排布
#include <QResizeEvent>
#include <QTextCharFormat>
#include <QUndoStack>
#include <QTextBlockFormat>
#include <QFile>


// 类声明
class DocPage;
class CT_DocInfo;
class CT_CommonData;
class DocBlock;
class DocTextBlock;
class DocImageBlock;
//class PageDialog;

/**
 * @Author Chaoqun
 * @brief  表述一篇文章
 * @date   2017/04/30
 */
class MODELSHARED_EXPORT DocPassage
        :public QScrollArea
{
    Q_OBJECT
public:

    static DocPassage *createNewPassage();              // 创建新的文章
    static DocPassage *createEmptyPassage();            // 创建空的文章

    explicit DocPassage(QWidget* parent);
    DocPassage();                                       // 空构造函数
//    DocPassage(QWidget *parent, QString version,
//               QString docType,double scaleFactor);     // 可能用不上了
    ~DocPassage();

    CT_DocInfo* getDocInfo();                // 获取CT_DocInfo数据
    QString getUUID();                      // 获得文章的uuid
    QString getTempSavePath();              // 获得文章保存前的临时路径
    QString getTempStorePath();             // 获得暂存资源的临时路径
    QString getVersion( ){return this->version;}
    QString getDocType( ){return this->docType;}
    QVector<DocPage* >* getPages();         // 获得文档中包含的所有页面
    DocPage *getPage(int index);            // 获得文档中的某一页
    QString getFilePath();                  // 获得文档路径
    QUndoStack *undoStack;                  // 撤销队列
    DocPage* getLastedActivedPage();        // Get last actived page

public slots:
    void addPage(DocPage *page);                    // 添加一个新页面
    void addPassage(QVector<DocPage* >& passage);   // 添加很多界面
    void appendNewPage(DocPage *page = NULL);       // 如果为空，则代表假如空白页

    void removePage(int index);                     // 移除某一页
    void removePage(DocPage* page);                 // 移除某一页
    void setFilePath(QString filePath);             // 设置文件路径

    // DocInfo
    void setDocInfo( CT_DocInfo & docInfo );

    // CommonData
    void setCommonData(CT_CommonData* commonData);  // 注意，设置的是直接的指针
    CT_CommonData* getCommonData();                 // 注意，返回的是直接的指针

    // Version
    void setVersion(QString version){this->version = version;}

    // DocType
    void setDocType(QString docType){this->docType = docType;}

    // docId
    void resetDocId();          // 重新设置DocId

//    void testMessage();       // 测试信号是否走通

    void adjustWidgetSize();// 根据页数来自动调整widget大小

    void setDefaultPageSize(
            double default_width,
            double default_height);       //设置默认的页面尺寸

    void activatePageDialog();              // 调出设置页面尺寸窗口

    // 更改指定页面的页面尺寸
    void modifyPageSize(
            QVector<int> * ch_pages,
            double _width,
            double _height,
            bool isUsingWorkingArea,
            double contentWidth,
            double contentHeight,
            double contentX,
            double contentY);

    // 修改文章的默认尺寸
    void modifyDefaultPageSize(
            double default_width,
            double default_height,
            bool default_isUsingWorkingArea,
            double default_contentWidth,
            double default_contentHeight,
            double default_contentX,
            double default_contentY
            );

    void zoomIn();                      // 缩小
    void zoomOut();                     // 放大

    void setScale(double scale);        // 设置缩放

    // Set the current actived page
    void setCurrentActivedPage(DocPage* page);


protected:
    void resizeEvent(QResizeEvent* event);
    void closeEvent(QCloseEvent *event);    // 继承，关闭前提示保存


private:
    // 数据区
    QString version;                // OFD 版本默认 1.0
    QString docType;                // 类型默认是 OFD
    CT_DocInfo* docInfo;            // 文档元数据结构 ofd/CT_DocInfo
    CT_CommonData* commonData;      // 文档公用文档数据
    QVector<DocPage *> pages;       // 既作为数据，也作为渲染
    DocPage* _lastActivedPage;      // 用来纪录最后操作过的页面

    // 文件信息
    QString filePath;
    QString fileName;
    QString tempPath;               // 保存成文件前的临时路径
        // 个人觉得可以用上UUID tmp/"uuid-fileName"

    // 渲染区
    QVBoxLayout * layout;           // 纵向排列
    QWidget * widget;               // 用widget做缓冲

    double scaleFactor;             // 表示缩放倍数
    double widgetWidth;             // 文章内部的宽度
    double widgetHeight;            // 内容的高度
    int horizontalWhite;            // 白色页面左右两边的灰色区域
    int verticalWhite;              // 白色页面上下的灰色区域

    // 默认的大小
    double default_width;                       //默认宽度
    double default_height;                      //默认高度
    bool default_using_working_area;
    double default_working_width;
    double default_working_height;
    double default_working_x;
    double default_working_y;

    void initUI();                  // 初始化文章界面相关的
    void initDocInfo();             // 初始化文档信息
    void adjustScrollBar(
            QScrollBar *scrollBar,
            double factor);        // 调整滑动条
    void adjustScrollBarRange();                //调整滑动条范围


signals:
    void signals_insertTextBlock(
            DocTextBlock* textBlock);      // 用来转发信号
    void signals_removeTextBlock(
            DocTextBlock* textBlock);      // 用来转发信号

    void signals_currentCharFormatChanged(
            QTextCharFormat fmt);    // 当前选择的charFormat发生了变化
    void signals_currentBlockFormatChanged(
            QTextBlockFormat fmt);  // 当前选择的block格式发生了变化
    void signals_currentTextBlock(
            DocTextBlock* textBlock); // 当前操作的textBlock
    void signals_currentImageBlock(
            DocImageBlock* imageBlock);  //当前操作的imageBlock

};

#endif // DOCPASSAGE_H
