#ifndef COMMANDS_H
#define COMMANDS_H

#include <QUndoCommand>
#include <QTextCharFormat>
#include <QTextCursor>

class DocPassage;
class DocTextBlock;
class DocPage;
class DocLayer;

class SetTextBlodCmd:
        public QUndoCommand
{
public:
    SetTextBlodCmd(DocTextBlock *docTextBlock,QTextCursor &cursor,QUndoCommand *parent=0);

    // QUndoCommand interface
public:
    void undo();
    void redo();

private:
    DocTextBlock *m_docTextBlock;
    QTextCursor m_cursor;
};

#endif // COMMANS_H
