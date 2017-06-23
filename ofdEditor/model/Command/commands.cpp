#include "commands.h"
#include<Doc/DocTextBlock.h>

SetTextBlodCmd::SetTextBlodCmd(DocTextBlock *docTextBlock, QTextCursor &cursor, QUndoCommand *parent)
{
    m_docTextBlock=docTextBlock;
    m_cursor=cursor;
}

void SetTextBlodCmd::undo()
{
    QTextCharFormat fmt;
    fmt.setFontWeight(QFont::Normal);
    m_docTextBlock->mergeFormatOnWordOrSelection(m_cursor,fmt);
}

void SetTextBlodCmd::redo()
{
    QTextCharFormat fmt;
    fmt.setFontWeight(QFont::Bold);
    m_docTextBlock->mergeFormatOnWordOrSelection(m_cursor,fmt);
}
