#ifndef CT_CLIP_H
#define CT_CLIP_H

#include <string>
using std::string;
#include <vector>
using std::vector;
#include "basic_datatype.h"

class CT_Clip_Area{

    // 二选一
    // CT_Path Path;        // 用于裁剪的路径
    // CT_Text Text;        // 用于裁剪的文本

    ST_RefID DrawParam;     // 绘制参数引用
    ST_Array CTM;           // 针对对象坐标系，对Area下包含的Path和Text进行进一步的变换。

};


// 裁剪区结构
class CT_Clip
{
public:


    vector<CT_Clip_Area> Area;  // 剪区类型，描述了图元的裁剪区。裁剪区是各个Area的“并”。
    CT_Clip();
};

#endif // CT_CLIP_H
