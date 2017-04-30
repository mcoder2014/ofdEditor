#ifndef DOCBLOCK_H
#define DOCBLOCK_H

#include "model_global.h"       // 导出lib使用
#include "Doc/DocBasicTypes.h"


class MODELSHARED_EXPORT DocBlock
{
public:
    DocBlock();

private:
    DocBox boundary;        // 该块的外包矩形

};

#endif // DOCBLOCK_H
