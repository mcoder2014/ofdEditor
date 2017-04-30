#include "DocPage.h"
#include"DocLayer.h"

DocPage::DocPage()
{

}

DocPage::~DocPage()
{

    int layers_length = layers.size();
    for(int i = 0; i < layers_length; i++)
        // 挨个释放空间
    {
        DocLayer* temp = this->layers.at(i);
        if(temp != NULL)
        {
            delete temp;
            temp = NULL;
        }
    }
    this->layers.clear();   // 清空层

}
