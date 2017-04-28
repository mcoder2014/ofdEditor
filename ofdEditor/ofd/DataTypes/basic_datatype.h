#ifndef COMMONDT_H
#define COMMONDT_H

#include "../ofd_global.h"   // 生成库文件时需要

#include <string>
using std::string;

// 其他数据类型
//#include "CT_DrawParam.h"
//#include "Color/CT_AxialShd.h"
//#include "Color/CT_Color.h"
//#include "Color/CT_ColorSpace.h"
//#include "Color/CT_Pattern.h"
//#include "Color/CT_RadialShd.h"

//6种基本数据类型
struct OFDSHARED_EXPORT ST_Loc {
    string path;

};

struct OFDSHARED_EXPORT ST_Array {

};

struct OFDSHARED_EXPORT ST_ID {
    unsigned long id;
};

struct OFDSHARED_EXPORT ST_RefID {
    unsigned long ref_id;
};

struct OFDSHARED_EXPORT ST_Pos {
    double x, y;
};

struct OFDSHARED_EXPORT ST_Box {
    double start_x, start_y, delta_x, delta_y;
};

// 根据xsd描述重新写了
//class CT_PageArea {
//public:
//    ST_Box physical_box;
//};

// 根据xsd描述重新写了
//class CT_Color {
//    //属性
//    ST_Array value;
//    int index;
//    ST_RefID colorspace;
//};
#endif // COMMONDT_H
