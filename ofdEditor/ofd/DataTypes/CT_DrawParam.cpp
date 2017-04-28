#include "CT_DrawParam.h"

#include "Color/CT_Color.h"

CT_DrawParam::CT_DrawParam()
    :Join("Miter"),
      Cap("Butt")

{
    this->LineWidth = 0.353;
    this->MiterLimit = 3.528;
    // this->FilleColor
    // this->StrokeColor
}

CT_DrawParam::~CT_DrawParam()
{

}
