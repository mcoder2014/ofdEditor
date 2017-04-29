#ifndef OFD_H
#define OFD_H
#include "../basic_datatype.h"

#include "../../ofd_global.h"  // 生成库文件需要

class OFDSHARED_EXPORT OFD { //主入口文件的根元素
    //属性
    string version; //版本号
    string doc_type;    //文档子集类型
};

#endif // OFD_H
