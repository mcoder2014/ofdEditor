#ifndef DOCPAGE_H
#define DOCPAGE_H

#include "model_global.h"       // 导出lib使用

#include <QVector>
#include <QWidget>
#include <QGraphicsView>
#include <QGraphicsScene>

// 类声明
class DocLayer;
//class CT_PageArea;



/**
 * @Author Chaoqun
 * @brief  用来表示文章中的某一页
 * @date   2017/04/30
 */
class MODELSHARED_EXPORT DocPage
        :public QGraphicsView
{
    Q_OBJECT
public:
    explicit DocPage(QWidget * parent = 0);
    DocPage(double width,
            double height, double scaleFactor,QWidget * parent = 0);
    ~DocPage();

    void setSize(double width, double height);    // 设置页面大小
    QSize getSize();                              // 获得页面像素大小
    double getWidth(){return width_mm;}           // 返回毫米单位宽度
    double getHeight(){return height_mm;}         // 返回毫米单位高度

protected:
    void paintEvent(QPaintEvent *event);

private:
    QGraphicsScene* scene;               // 场景
    QVector<DocLayer *> layers;          // 一个文档具有很多层

    // 还应该有模板页
    //CT_PageArea* area;                  // 页面大小描述

    double width_mm;          // 页面的宽      --单位 mm
    double height_mm;         // 页面的高

    double scaleFactor;            // 表示缩放倍数

};

#endif // DOCPAGE_H
