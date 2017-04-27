#ifndef CT_CGTRANSFORM_H
#define CT_CGTRANSFORM_H
#include "../basic_datatype.h"

class CT_CGTransform {  //字形变换类型
    //属性
    int CodePosition;   //TextCode中字符编码的起始位置，从0开始
    int CodeCount;  //变换关系中字符的数量，该数值必须大于等于1.
    int GlyphCount; //变换关系中字形索引的个数，该数值必须大于等于1，否则属于错误描述
};
#endif // CT_CGTRANSFORM_H
