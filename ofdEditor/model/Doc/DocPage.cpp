#include "DocPage.h"
#include "DocLayer.h"
#include "Tool/UnitTool.h"

#include <QPalette>

// #include "DataTypes/page/CT_PageArea.h"     // 页面大小

DocPage::DocPage(QWidget *parent)
    :QWidget(parent)
{
    this->setSize(210,297);

    this->scaleFactor = 1.0;



}

DocPage::DocPage(double width,
                 double height, double scaleFactor, QWidget *parent)
    :QWidget(parent)
{
    this->setSize(width,height);     // 设置widget大小
    this->scaleFactor = scaleFactor;
    this->setVisible(true);
}

DocPage::~DocPage()
{
    // area空间释放

    // 层空间释放
    int layers_length = layers.size();
    for(int i = 0; i < layers_length; i++)
        // 挨个释放空间
    {
        DocLayer* temp = this->layers.at(i);
        if(temp != NULL)
        {
            delete temp;
            (this->layers)[i] = NULL;
        }
    }
    this->layers.clear();   // 清空层

}


/**
 * @Author Chaoqun
 * @brief  调整页面的大小
 * @param  double width, double height
 * @return 返回值
 * @date   2017/05/01
 */
void DocPage::setSize(double width, double height)
{
    // 保存mm单位大小
    this->width_mm = width;
    this->height_mm = height;

    this->setFixedSize(UnitTool::mmToPixel(width),
                       UnitTool::mmToPixel(height));   // 设置页面大小
    this->setBackgroundRole(QPalette::BrightText);          // 背景颜色
    this->setAutoFillBackground(true);
}

/**
 * @Author Chaoqun
 * @brief  获得页面的大小-像素大小
 * @return QSize
 * @date   2017/05/01
 */
QSize DocPage::getSize()
{
    return QSize(UnitTool::mmToPixel(width_mm),
                 UnitTool::mmToPixel(height_mm));
}
