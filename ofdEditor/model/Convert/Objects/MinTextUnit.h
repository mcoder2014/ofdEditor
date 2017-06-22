#ifndef MINTEXTUNIT_H
#define MINTEXTUNIT_H

#include <QObject>
#include <QColor>
#include "model_global.h"

class MODELSHARED_EXPORT MinTextUnit
        : public QObject
{
    Q_OBJECT
public:
    explicit MinTextUnit(QObject *parent = 0);

signals:

public slots:

private:
    QColor fill_color;          // 填充颜色
    QColor stroke_color;        // 描边颜色

};

#endif // MINTEXTUNIT_H
