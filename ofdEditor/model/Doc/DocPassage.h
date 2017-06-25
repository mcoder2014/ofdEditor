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
    explicit DocPassage(QWidget* parent = 0);
    DocPassage(QWidget *parent, QString version,
               QString docType,double scaleFactor);
    ~DocPassage();

    CT_DocInfo* getDocInfo();                // 获取CT_DocInfo数据
    QString getVersion( ){return this->version;}
    QString getDocType( ){return this->docType;}
    QVector<DocPage* >& getPages();         // 获得文档中包含的所有页面
    DocPage *getPage(int index);            // 获得文档中的某一页
    QUndoStack *undoStack;                  // 撤销队列

public slots:
    void addPage(DocPage *page);                    // 添加一个新页面
    void addPassage(QVector<DocPage* >& passage);   // 添加很多界面
    void appendNewPage(DocPage *page = NULL);       // 如果为空，则代表假如空白页

    void removePage(int index);                     // 移除某一页
    void removePage(DocPage* page);                 // 移除某一页

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

    void testMessage(); // 测试信号是否走通

    void adjustWidgetSize();                    // 根据页数来自动调整widget大小

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

    // 文件信息
    QString filePath;
    QString fileName;
    QString tempPath;               // 保存成文件前的临时路径
        // 个人觉得可以用上UUID tmp/"uuid-fileName"

    // 渲染区
    QVBoxLayout * layout;           // 纵向排列
    QWidget * widget;               // 用widget做缓冲

    double scaleFactor;             // 表示缩放倍数
    double widgetWidth;             // 内容的宽度
    double widgetHeight;            // 内容的高度

    int horizontalWhite;            // 白色页面左右两边的灰色区域
    int verticalWhite;              // 白色页面上下的灰色区域

    void init();                                // 初始化
    void adjustScrollBar(QScrollBar *scrollBar,
                         double factor);        // 调整滑动条
    void adjustScrollBarRange();                //调整滑动条范围


signals:
    void signals_insertTextBlock(DocTextBlock* textBlock);      // 用来转发信号
    void signals_removeTextBlock(DocTextBlock* textBlock);      // 用来转发信号

    void signals_currentCharFormatChanged(
            QTextCharFormat& fmt);    // 当前选择的charFormat发生了变化
    void signals_currentBlockFormatChanged(
            QTextBlockFormat& fmt);  // 当前选择的block格式发生了变化
    void signals_currentTextBlock(DocTextBlock* textBlock); // 当前操作的textBlock
    void signals_currentImageBlock(DocImageBlock* imageBlock);  //当前操作的imageBlock

};

#endif // DOCPASSAGE_H
