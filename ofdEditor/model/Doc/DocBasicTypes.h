#ifndef DOCBASICTYPES_H
#define DOCBASICTYPES_H

#include "model_global.h"

/**
 * @Author Chaoqun
 * @brief  功能同 ST_Box
 * @date   2017/04/30
 */
class MODELSHARED_EXPORT DocBox
{
public:
    double x;
    double y;
    double width;
    double height;

    void set(double x, double y,double width, double height)
    {
        this->x = x;
        this->y = y;
        this->width = width;
        this->height = height;
    }

};



#endif // DOCBASICTYPES_H
