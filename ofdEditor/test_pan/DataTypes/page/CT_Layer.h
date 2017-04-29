#ifndef CT_LAYER_H
#define CT_LAYER_H
#include "../basic_datatype.h"
#include "CT_PageBlock.h"

#include "../../ofd_global.h"  // 生成库文件需要

class OFDSHARED_EXPORT CT_Layer : CT_PageBlock {
public:
    enum LayerType {Body, Foreground, Background};
    LayerType type;
};

#endif // CT_LAYER_H
