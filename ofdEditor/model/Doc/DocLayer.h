#ifndef DOCLAYER_H
#define DOCLAYER_H

#include "model_global.h"       // 导出lib使用

#include <QVector>

class DocBlock;     // 块
class DocTable;     // 表格
class DocDrawParam; // 默认绘画模式

/**
 * @Author Chaoqun
 * @brief  一层
 * @date   2017/04/30
 */
class MODELSHARED_EXPORT DocLayer
{
public:
    DocLayer();
    ~DocLayer();
private:
    QVector<DocBlock* > blocks;         // 块    -文字块图形块的父类
    QVector<DocTable* > tables;         // 表格
    DocDrawParam* drawParam;            // 该层的默认绘画模式

    enum LayerType{Body,ForeGround,BackGroune};
    LayerType type;                     // 表示该层属于那一层上的

};

#endif // DOCLAYER_H
