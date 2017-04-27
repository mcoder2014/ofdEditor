#ifndef DOCUMENTCLASSES_H
#define DOCUMENTCLASSES_H
#include "basic_datatype.h"
#include "text_object_classes.h"
#include <vector>

class CT_PageBlock {
public:
    std::vector<CT_Text> text_objects;
};

class CT_Layer : CT_PageBlock {
public:
    enum LayerType {Body, Foreground, Background};
    LayerType type;
};

class Page {
public:
    CT_PageArea area;
    //内容Contents
    CT_Layer layer;
};





#endif // DOCUMENTCLASSES_H
