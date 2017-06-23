#include "DocLayer.h"
#include "Doc/DocBlock.h"       // 块
#include "Doc/DocTable.h"       // 表格
#include "Doc/DocDrawParam.h"
#include "Doc/DocPage.h"
#include "Doc/DocPassage.h"

DocLayer::DocLayer()
{

}

DocLayer::DocLayer(DocPage::Layer layer)
{
    this->type = layer;
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

/**
 * @Author Chaoqun
 * @brief  向该层添加一个block
 * @param  DocBlock * block
 * @return void
 * @date   2017/05/06
 */
void DocLayer::addBlock(DocBlock *block)
{
    this->blocks.append(block);     // 追加到队尾
    block->setZValue(this->zValue); // 设置Z值
    block->setLayer(this);          // 设置引用
}

/**
 * @Author Chaoqun
 * @brief  移除块
 * @param  DocBlock * block
 * @return void
 * @date   2017/05/06
 */
void DocLayer::removeBlock(DocBlock *block)
{
    // 判断参数是否正确
    if(block == NULL)
        return;
    // 判断是否包含本块
    if(this->blocks.contains(block))
    {
        int index = this->blocks.indexOf(block);
        this->blocks.remove(index);
    }
}

/**
 * @Author Chaoqun
 * @brief  设置页
 * @param  DocPage * page
 * @return void
 * @date   2017/06/21
 */
void DocLayer::setPage(DocPage *page)
{
    this->parent = page;
}

/**
 * @Author Chaoqun
 * @brief  获得本层的所有blocks
 * @return QVector<DocBlock *>
 * @date   2017/05/06
 */
QVector<DocBlock *>* DocLayer::getBlocks()
{
    QVector<DocBlock* > *vector = new QVector<DocBlock* >();
    int length = this->blocks.size();

    for(int i = 0; i<length; i++)
    {
        vector->push_back((this->blocks)[i]);
    }

    return vector;
}

/**
 * @Author Chaoqun
 * @brief  获得页
 * @param  void
 * @return DocPage*
 * @date   2017/06/21
 */
DocPage *DocLayer::getPage()
{
    return this->parent;
}

/**
 * @Author Chaoqun
 * @brief  获得文章
 * @param  void
 * @return DocPassage *
 * @date   2017/06/21
 */
DocPassage *DocLayer::getPassage()
{
    DocPage *page = this->getPage();
    if(page == NULL)
        return NULL;
    return page->getPassage();
}
