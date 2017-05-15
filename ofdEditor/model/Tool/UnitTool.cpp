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
