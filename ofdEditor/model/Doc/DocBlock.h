#ifndef DOCBLOCK_H
#define DOCBLOCK_H

#include "model_global.h"       // 导出lib使用
#include "Doc/DocBasicTypes.h"
#include <QGraphicsItemGroup>

/**
 * @Author Chaoqun
 * @brief  一个块，应该是个组，块内可以调整，块外统一
 * @param  参数
 * @return 返回值
 * @date   2017/05/03
 */
class MODELSHARED_EXPORT DocBlock
        :QGraphicsItemGroup
{
public:
    DocBlock();

private:
    DocBox boundary;        // 该块的外包矩形

};

#endif // DOCBLOCK_H
