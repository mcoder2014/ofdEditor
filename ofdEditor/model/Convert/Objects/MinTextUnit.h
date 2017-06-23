#ifndef MINTEXTUNIT_H
#define MINTEXTUNIT_H

#include <QObject>
#include <QColor>
#include <QFont>
#include <QString>
#include <QVector>

#include "model_global.h"

// 中间转换的数据类型，为了求速度，全部设为公有数据.
class MODELSHARED_EXPORT MinTextUnit
        : public QObject
{
    Q_OBJECT
public:
    explicit MinTextUnit(QObject *parent = 0);



signals:

public slots:

public:
    QColor fillColor;          // 填充颜色
    QColor strokeColor;        // 描边颜色
    QFont font;                 // 字体
    double x,y;                 // 左下角坐标
    QString text;               // 内容
    double lt_x,lt_y,width,height;  // 边界
    QVector<double> delta_x;        // 偏移量

};

#endif // MINTEXTUNIT_H
