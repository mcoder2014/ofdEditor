#include "DocTextBlock.h"
#include "Doc/DocParagraph.h"

DocTextBlock::DocTextBlock()
{

}

DocTextBlock::~DocTextBlock()
{
    // 释放段落
   int paragraphs_length = this->paragraphs.size();
   for(int i = 0 ; i < paragraphs_length; i++)
   {
       // 挨个释放
       DocParagraph *temp = this->paragraphs.at(i);
       if(temp != NULL)
       {
           delete temp;
           (this->paragraphs)[i] = NULL;
       }
   }
   this->paragraphs.clear();

}
