#ifndef COMMONDT_H
#define COMMONDT_H

//6种基本数据类型
struct ST_Loc {

};

struct ST_Array {

};

struct ST_ID {
    unsigned long id;
};

struct ST_RefID {
    unsigned long ref_id;
};

struct ST_Pos {
    double x, y;
};

struct ST_Box {
    double start_x, start_y, delta_x, delta_y;
};


class CT_PageArea {
public:
    ST_Box physical_box;    //物理区域
    ST_Box application_box; //显示区域
    ST_Box content_box;     //版心区域
    ST_Box bleed_box;       //出血区域
};


class CT_Color {
    //属性
    ST_Array value;
    int index;
    ST_RefID colorspace;
};
#endif // COMMONDT_H
