#ifndef DOCGRAPH_H
#define DOCGRAPH_H

#include "model_global.h"       // 导出lib使用
#include "Doc/DocBasicTypes.h"
#include "Doc/DocImage.h"

class MODELSHARED_EXPORT DocGraph
        :public DocImage
{
    Q_OBJECT
public:
    DocGraph(QObject *parent = NULL);
    ~DocGraph();
};

#endif // DOCGRAPH_H
