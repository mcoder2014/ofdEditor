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

// 类声明
class DocPage;
class CT_DocInfo;
class CT_CommonData;

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

    void addPage(DocPage *page);                    // 添加一个新页面
    void addPassage(QVector<DocPage* >& passage);   // 添加很多界面
    void appendNewPage(DocPage *page = NULL);       // 如果为空，则代表假如空白页

    void setDocInfo( CT_DocInfo & docInfo );
    CT_DocInfo getDocInfo();

    void setVersion(QString version){this->version = version;}
    QString getVersion( ){return this->version;}

    void setDocType(QString docType){this->docType = docType;}
    QString getDocType( ){return this->docType;}


protected:
    void resizeEvent(QResizeEvent* event);

    void closeEvent(QCloseEvent *event);    // 继承，关闭前提示保存


private:
    // 数据区
    QString version;             // OFD 版本默认 1.0
    QString docType;             // 类型默认是 OFD
    CT_DocInfo* docInfo;         // 文档元数据结构 ofd/CT_DocInfo
    //CT_CommonData* commonData; // 文档公用文档数据

    QVector<DocPage *> pages;    // 既作为数据，也作为渲染

    // 渲染区
    QVBoxLayout * layout;           // 纵向排列
    QWidget * widget;               // 用widget做缓冲

    double scaleFactor;             // 表示缩放倍数
    double widgetWidth;             // 内容的宽度
    double widgetHeight;            // 内容的高度

    int horizontalWhite;            // 白色页面左右两边的灰色区域
    int verticalWhite;              // 白色页面上下的灰色区域


private:
    void init();                    // 初始化
    void adjustScrollBar(QScrollBar *scrollBar,
                         double factor); // 调整滑动条
    void adjustScrollBarRange();    //调整滑动条范围
    void adjustWidgetSize();        // 根据页数来自动调整widget大小




};

#endif // DOCPASSAGE_H
