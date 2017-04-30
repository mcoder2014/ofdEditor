#include "DocPassage.h"

#include "DocPage.h"
#include "DataTypes/document/CT_DocInfo.h"
//#include "DataTypes/document/CT_CommonData.h"

/**
 * @Author Chaoqun
 * @brief  默认构造函数
 * @date   2017/04/30
 */
DocPassage::DocPassage()
    :docType("OFD"),version("1.0")
{
    this->docInfo = new CT_DocInfo();
//    this->commonData = new CT_CommonData();

}

DocPassage::~DocPassage()
{
    // 释放根节点内容
    if(this->docInfo != NULL)
    {
        delete this->docInfo;
        this->docInfo = NULL;
    }

    // 释放公共数据
//    if(this->commonData != NULL)
//    {
//        delete this->commonData;
//        this->commonData = NULL;
//    }

    // 释放this->pages
    int pages_length = this->pages.size();
    for(int i = 0; i < pages_length; i++)
    {
        // 需要挨个释放内存空间
        DocPage* temp = this->pages.at(i);
        if(temp != NULL)
        {
            delete temp;
            temp = NULL;
        }
    }
    this->pages.clear();

}
