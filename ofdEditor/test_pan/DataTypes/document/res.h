#ifndef RES_H
#define RES_H
#include "../basic_datatype.h"
#include "../text/CT_Font.h"
#include "../Color/CT_ColorSpace.h"
#include "../../ofd_global.h"  // 生成库文件需要

class OFDSHARED_EXPORT Res {
    //属性
    ST_Loc base_loc;        //定义此资源描述文件的通用数据存储路径
    //成员
    QVector<CT_Font *> * fonts;
    QVector<CT_ColorSpace *> * colorspaces;
    //Other resource collections to be implemented

    Res() {}
public:
    friend class OFDParser;
    ST_Loc getBaseLoc() {
        return base_loc;
    }

    QVector<CT_Font *> * getFonts() {
        return fonts;
    }

    QVector<CT_ColorSpace *> * getColorSpaces() {
        return colorspaces;
    }
};

#endif // RES_H
