#ifndef COMMONDT_H
#define COMMONDT_H
#include <string>
#include <vector>
using std::string;
using std::vector;
#include "../ofd_global.h"   // 生成库文件时需要

#include <string>
using std::string;

//6种基本数据类型
class OFDSHARED_EXPORT ST_Loc {
public:
    string location;
};

class OFDSHARED_EXPORT ST_Array {

};

class OFDSHARED_EXPORT ST_ID {
public:
    unsigned long id;
};

class OFDSHARED_EXPORT ST_RefID {
public:
    unsigned long ref_id;
};

class OFDSHARED_EXPORT ST_Pos {
public:
    double x, y;
};

class OFDSHARED_EXPORT ST_Box {
public:
    double start_x, start_y, delta_x, delta_y;
};

class OFDSHARED_EXPORT CT_Object { //OFD文档中所有部件的基类
    ST_ID id;
};
#endif // COMMONDT_H
