#include "DocLayer.h"
#include "Doc/DocBlock.h"       // 块
#include "Doc/DocTable.h"       // 表格
#include "Doc/DocDrawParam.h"


DocLayer::DocLayer()
{

}

DocLayer::~DocLayer()
{

    // 释放块
    int blocks_length = this->blocks.size();
    for(int i = 0; i < blocks_length; i++)
    {
        //挨个释放内存空间
        DocBlock* temp = this->blocks.at(i);
        if(temp != NULL)
        {
            delete temp;
            (this->blocks)[i] = NULL;
        }
    }
    this->blocks.clear();

    // 释放表格
    int tables_length = this->tables.size();
    for(int i = 0; i < tables_length; i++)
    {
        // 挨个释放表格的空间
        DocTable* temp = this->tables.at(i);
        if(temp != NULL)
        {
            delete temp;
            (this->tables)[i]=NULL;
        }
    }
    this->tables.clear();

    // 释放绘制模式
    if(this->drawParam != NULL)
    {
        delete this->drawParam;
        this->drawParam = NULL;
    }

}

/**
 * @Author Chaoqun
 * @brief  修改本层的 zValue的值
 * @param  qreal z
 * @return void
 * @date   2017/05/03
 */
void DocLayer::setZValue(qreal z)
{
    this->zValue = z;

    // 遍历所有块，将他们的值全部修改为 z

}
