#include "DocPage.h"
#include"DocLayer.h"

// #include "DataTypes/page/CT_PageArea.h"     // 页面大小

DocPage::DocPage()
{


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
