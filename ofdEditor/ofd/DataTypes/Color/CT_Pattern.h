#ifndef CT_PATTERN_H
#define CT_PATTERN_H

#include "../../ofd_global.h"  // 生成库文件需要

#include <string>
using std::string;
#include "../basic_datatype.h"

//// CT_PageBlock
//class CT_PageBlock{
//public:
//    ST_RefID Thumbnail;
//};

//// CellContent
//class CT_Pattern_CellContent{
//public:
//    CT_PageBlock block;
//};

// 底纹类型
class OFDSHARED_EXPORT CT_Pattern
{
public:

    double Width;               // 底纹绘制区宽度      -必选
    double Height;              // 底纹绘制区的高度    -必选
    double XStep;               // X方向底纹单元间隔    -默认值底纹宽度
    double YStep;               // Y方向底纹单元间隔    -默认值底纹高度

    string ReflectMethod;       // 描述底纹Cell的映像翻转方式，
        // 枚举值，默认为Normal
    string RelativeTo;          // Pattern 单元起始绘制位置。可取值如下
                                // Page：相对于页面坐标系的原点
                                // Object：相对于对象坐标系的原点
                                // 默认值是Object
    ST_Array CTM;               // Pattern 单元的变换矩阵，
        // 用于某些需要对Pattern 单元进行平移旋转变换的场合，默认为单位矩阵；
        // Pattern 呈现时先做XStep,YStep 排布，然后一起做CTM 处理
    //CT_PageBlock CellContent;   // 底纹单元，用底纹画刷填充目标区域时，所使用的单元对象 -必选
    //CT_Pattern_CellContent CellContent;     //底纹单元

    CT_Pattern();
};







#endif // CT_PATTERN_H
