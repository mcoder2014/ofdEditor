#include "CT_GraphicUnit.h"

#include "Color/CT_Color.h"
#include "CT_Clip.h"
#include "CT_Action.h"

CT_GraphicUnit::CT_GraphicUnit()
    :Cap("Butt"),Join("Miter")
{

    this->Visible = true;
    this->LineWidth = 0.353;
    this->MiterLimit = 3.528;
    this->DashOffset = 0;
    this->Alpha = 255;

}

CT_GraphicUnit::~CT_GraphicUnit()
{

}
