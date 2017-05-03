#ifndef CT_LAYER_H
#define CT_LAYER_H
#include "../basic_datatype.h"
#include "CT_PageBlock.h"

#include "../../ofd_global.h"  // 生成库文件需要

class OFDSHARED_EXPORT CT_Layer : public CT_PageBlock {
    LayerType type;
    ST_RefID draw_param;
    CT_Layer() : CT_PageBlock(){
        type = Body;
    }
public:
    enum LayerType {Body, Foreground, Background};
    QString getType() {
        switch (type) {
        case Body:
            return "Body";
        case Foreground:
            return "Foreground";
        case Background:
            return "Background";
        }
    }
    ST_RefID getDrawParam() {
        return draw_param;
    }

    //从CT_PageBlock继承了几个接口
};

#endif // CT_LAYER_H
