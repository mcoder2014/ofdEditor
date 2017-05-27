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

/**
 * @Author Chaoqun
 * @brief  将应用中的像素单位转换为毫米单位
 * @param  double pixel
 * @return double mm
 * @date   2017/05/17
 */
double UnitTool::pixelToMM(double pixel)
{
    QScreen* screen = QApplication::primaryScreen();    // 获取系统中默认显示器
    double dotsPerInch = screen->physicalDotsPerInch(); // 每英寸屏幕多少像素
    double inch = pixel / dotsPerInch;          // 获取像素单位对应的英寸单位
    return inch * 25.4;                 // 返回毫米单位
}

/**
 *@Author Chaoqun
 *@brief  获得由两个点构成的方形
 *@param  QPointF &point1
 *@param  QPointF &point2
 *@return QRectF
 *@date   2017/05/15
 */
QRectF UnitTool::getBox(QPointF &point1, QPointF &point2)
{
    qreal x1 = point1.x();
    qreal x2 = point2.x();

    qreal y1 = point1.y();
    qreal y2 = point2.y();

    qreal x,y,w,h;

    if(x1 < x2)
    {
        x = x1;
        w = x2 - x1;
    }
    else
    {
        x = x2;
        w = x1 - x2;
    }

    if(y1 < y2)
    {
        y = y1;
        h = y2 - y1;
    }
    else
    {
        y = y2;
        h = y1 - y2;
    }

    return QRectF(x,y,w,h);
}

/**
 * @Author Chaoqun
 * @brief  判断两个数是否相等，因为公文的误差要求是 1mm，
 *      对于边框的要求没有那么高。
 *      因此，我这里两者差的绝对值小于 0.05mm即视为相等。
 * @param  参数
 * @return 返回值
 * @date   2017/05/23
 */
bool UnitTool::equal(double a, double b)
{
    return ((a-b) > -0.05
            && (a-b) < 0.05);
}
