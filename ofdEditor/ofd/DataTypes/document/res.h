#ifndef RES_H
#define RES_H
#include "../basic_datatype.h"
#include "../text/CT_Font.h"
#include "../Color/CT_ColorSpace.h"
#include "../image/CT_DrawParam.h"
#include "../../ofd_global.h"  // 生成库文件需要

class OFDSHARED_EXPORT Res {
    //属性
    ST_Loc base_loc;        //定义此资源描述文件的通用数据存储路径
    //成员
    QVector<CT_Font *> * fonts;
    QVector<CT_ColorSpace *> * colorspaces;
    QVector<CT_DrawParam *> * draw_params;
    //Other resource collections to be implemented

    Res() {
        fonts = new QVector<CT_Font *>();
        colorspaces = new QVector<CT_ColorSpace *>();
        draw_params = new QVector<CT_DrawParam *>();
    }
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

    QVector<CT_DrawParam *> * getDrawParams() {
        return draw_params;
    }

    ~Res() {
        for (int i = 0; i < fonts->size(); i++)
            delete fonts->at(i);
        for (int i = 0; i < colorspaces->size(); i++)
            delete colorspaces->at(i);
        for (int i = 0; i < draw_params->size(); i++)
            delete draw_params->at(i);
        delete fonts;
        delete colorspaces;
        delete draw_params;
    }
};

#endif // RES_H
