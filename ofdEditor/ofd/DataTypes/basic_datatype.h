#ifndef COMMONDT_H
#define COMMONDT_H
#include <string>
#include <vector>
using std::string;
using std::vector;
//6种基本数据类型
class ST_Loc {

};

class ST_Array {

};

class ST_ID {
public:
    unsigned long id;
};

class ST_RefID {
public:
    unsigned long ref_id;
};

class ST_Pos {
public:
    double x, y;
};

class ST_Box {
public:
    double start_x, start_y, delta_x, delta_y;
};


class CT_PageArea {
public:
    ST_Box physical_box;    //物理区域
    ST_Box application_box; //显示区域
    ST_Box content_box;     //版心区域
    ST_Box bleed_box;       //出血区域
};

class CT_Object { //OFD文档中所有部件的基类
    ST_ID id;
};

class CT_GraphicUnit : CT_Object{ //图元对象
    //属性
    ST_Box boundary;
};

class CT_Color {
    //属性
    ST_Array value;
    int index;
    ST_RefID colorspace;
};
#endif // COMMONDT_H
