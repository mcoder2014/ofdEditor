#include "CT_Path.h"

/**
 * @Author Chaoqun
 * @brief  默认构造函数
 * @date   2017/04/28
 */
CT_Path::CT_Path()
    :CT_GraphicUnit(),Rule("NonZero")
{
    this->Stroke = true;
    this->Fill = false;
}
