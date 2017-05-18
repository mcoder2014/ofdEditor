#ifndef DOCPAGESCENE_H
#define DOCPAGESCENE_H

#include <QGraphicsScene>

class DocPageScene
        :public QGraphicsScene
{
    Q_OBJECT
public:
    DocPageScene(QObject *parent = Q_NULLPTR);      // 构造函数


};

#endif // DOCPAGESCENE_H
