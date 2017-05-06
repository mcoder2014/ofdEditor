#include "DocTextBlock.h"
#include "Doc/DocParagraph.h"

#include <QTextCursor>
#include <QPalette>

DocTextBlock::DocTextBlock(QWidget *parent)
    :QTextEdit(parent)
{
    QTextCursor cursor(this->textCursor());
    cursor.insertText(tr("testsesetstsetestes"));

    this->setBackgroundRole(QPalette::Dark);
}

DocTextBlock::~DocTextBlock()
{

}
