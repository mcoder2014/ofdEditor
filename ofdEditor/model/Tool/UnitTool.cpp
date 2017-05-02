#include "UnitTool.h"
#include <QScreen>
#include <QApplication>

UnitTool::UnitTool()
{

}


/**
 * @Author Chaoqun
 * @brief  将文件格式中的毫米单位转换为电脑上的像素单位
 * @param  double mm
 * @return int
 * @date   2017/05/01
 */
int UnitTool::mmToPixel(double mm)
{
    double inch = mm * 0.0393700787;        // 转换成英寸
    QScreen* screen = QApplication::primaryScreen();
    double dotsPerInch = screen->physicalDotsPerInch(); // 每英寸多少像素

    return (int)(inch * dotsPerInch);       // 返回像素大小
}
