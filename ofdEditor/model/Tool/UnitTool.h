#ifndef UNITTOOL_H
#define UNITTOOL_H

/**
 * @Author Chaoqun
 * @brief  预计用来进行单位换算的工具，如毫米和像素之间的转换
 * @date   2017/05/01
 */
class UnitTool
{
public:
    UnitTool();
    static int mmToPixel(double mm);        // 将毫米单位和像素单位进行换算
};

#endif // UNITTOOL_H
