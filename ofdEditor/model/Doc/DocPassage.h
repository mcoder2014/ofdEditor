#ifndef DOCPASSAGE_H
#define DOCPASSAGE_H

#include "model_global.h"       // 导出lib使用

#include <QVector>
#include <QString>
#include <QWidget>
#include <QScrollArea>
#include <QVBoxLayout>      // 树状排布

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
    void addPassage(QVector<DocPage* >& passage);    // 添加很多界面


private:
    // 数据区
    QString version;             // OFD 版本默认 1.0
    QString docType;             // 类型默认是 OFD
    CT_DocInfo* docInfo;         // 文档元数据结构 ofd/CT_DocInfo
    //CT_CommonData* commonData; // 文档公用文档数据

    QVector<DocPage *> pages;    // 既作为数据，也作为渲染

    // 渲染区
    //QScrollArea * scrollArea;      // 可以进行进度条操作的区域
    QVBoxLayout * layout;          // 纵向排列
    QWidget * widget;               // 用widget做缓冲

    double scaleFactor;            // 表示缩放倍数


private:
    void init();                    // 初始化
    void adjustScrollBar(QScrollBar *scrollBar, double factor); // 调整滑动条




};

#endif // DOCPASSAGE_H
