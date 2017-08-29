#ifndef RES_H
#define RES_H
#include "../basic_datatype.h"
#include "../text/CT_Font.h"
#include "../Color/CT_ColorSpace.h"
#include "../image/CT_DrawParam.h"
#include "../../ofd_global.h"  // 生成库文件需要
#include "DataTypes/image/CT_MultiMedia.h"

class OFDSHARED_EXPORT Res
{
    //属性
    ST_Loc base_loc;        //定义此资源描述文件的通用数据存储路径
    //成员
    QVector<CT_Font *>* fonts;
    QVector<CT_ColorSpace *>* colorspaces;
    QVector<CT_DrawParam *>* draw_params;
    QVector<CT_MultiMedia *>* multMedias;
    //Other resource collections to be implemented

public:
    friend class OFDParser;

    Res(QString _base_loc = "",
        QVector<CT_Font *> * _fonts = NULL,
        QVector<CT_ColorSpace *> * _colorspaces = NULL,
        QVector<CT_DrawParam *> * _draw_params = NULL)
    {
        if (!_base_loc.isNull())
            setBaseLoc(_base_loc);
        if (!_fonts)
            fonts = new QVector<CT_Font *>();
        else
            fonts = _fonts;
        if (!_colorspaces)
            colorspaces = new QVector<CT_ColorSpace *>();
        else
            colorspaces = _colorspaces;
        if (!_draw_params)
            draw_params = new QVector<CT_DrawParam *>();
        else
            draw_params = _draw_params;

        multMedias = new QVector<CT_MultiMedia *>();

    }

    ST_Loc getBaseLoc()
    {
        return base_loc;
    }

    void setBaseLoc(QString _base_loc_abs)
    {
        ST_Loc p("Resource", _base_loc_abs,_base_loc_abs);
        base_loc = p;
    }

    QVector<CT_Font *> * getFonts()
    {
        return fonts;
    }

    QVector<CT_MultiMedia *> *getMultiMedia()
    {
        return this->multMedias;
    }

    void setFonts(QVector<CT_Font *> * _fonts)
    {
        if (!_fonts)
            throw InvalidValueException("Invalid value in Fonts in Res: null pointer");
        for (int i = 0; i < fonts->size(); i++)
            delete fonts->at(i);
        delete fonts;
        fonts = _fonts;
    }

    QVector<CT_ColorSpace *> * getColorSpaces()
    {
        return colorspaces;
    }

    void setColorSpaces(QVector<CT_ColorSpace *> * _colorspaces)
    {
        if (!_colorspaces)
            throw InvalidValueException("Invalid value in ColorSpaces in Res: null pointer");
        for (int i = 0; i < colorspaces->size(); i++)
            delete colorspaces->at(i);
        delete colorspaces;
        colorspaces = _colorspaces;
    }

    QVector<CT_DrawParam *> * getDrawParams()
    {
        return draw_params;
    }

    void setDrawParams(QVector<CT_DrawParam *> * _draw_params)
    {
        if (!_draw_params)
            throw InvalidValueException("Invalid value in DrawParams in Res: null pointer");
        for (int i = 0; i < draw_params->size(); i++)
            delete draw_params->at(i);
        delete draw_params;
        draw_params = _draw_params;
    }

    ~Res()
    {
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
