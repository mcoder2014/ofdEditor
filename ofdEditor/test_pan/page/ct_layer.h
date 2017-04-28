#ifndef CT_LAYER_H
#define CT_LAYER_H

class CT_Layer : CT_PageBlock {
public:
    enum LayerType {Body, Foreground, Background};
    LayerType type;
};
#endif // CT_LAYER_H
