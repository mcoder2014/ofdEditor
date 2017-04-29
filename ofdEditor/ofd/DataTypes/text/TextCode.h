#ifndef TEXTCODE_H
#define TEXTCODE_H
#include "../basic_datatype.h"

#include "../../ofd_global.h"  // 生成库文件需要

class OFDSHARED_EXPORT TextCode {    //文字内容，也就是一段字符编码串。需要进行XML转义
    //属性
    double x, y;    //首字形的绘制点的横纵坐标
    ST_Array delta_x, delta_y;  //字形绘制点横纵方向偏移
    //内容
    string content; //！！！！！文本内容
};
#endif // TEXTCODE_H
