#ifndef DOCBLOCK_H
#define DOCBLOCK_H

#include "model_global.h"       // 导出lib使用
#include "Doc/DocBasicTypes.h"
#include <QTextEdit>

class DocLayer;

/**
 * @Author Chaoqun
 * @brief  一个块
 * @param  参数
 * @return 返回值
 * @date   2017/05/03
 */
class MODELSHARED_EXPORT DocBlock
        :public QTextEdit
{
    Q_OBJECT
public:
    DocBlock(QWidget *parent = Q_NULLPTR);
    void setLayer(DocLayer * layer){this->layer = layer;}
    DocLayer * getLayer(){return this->layer;}

private:
    DocBox boundary;        // 该块的外包矩形
    DocLayer * layer;       // 该块在哪一个层之中

};

#endif // DOCBLOCK_H
