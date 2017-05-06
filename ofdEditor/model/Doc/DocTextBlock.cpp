#include "DocTextBlock.h"
#include "Doc/DocParagraph.h"

#include <QTextCursor>

DocTextBlock::DocTextBlock(QWidget *parent)
    :DocBlock(parent)
{
//    QTextCursor cursor(this->textCursor());
//    cursor.insertText(tr("test text"));
//    this->setFixedSize(300,300);
}

DocTextBlock::~DocTextBlock()
{

}
