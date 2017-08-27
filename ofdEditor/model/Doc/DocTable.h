#ifndef TABLE_H
#define TABLE_H

#include "model_global.h"
#include "Doc/DocBasicTypes.h"
#include <QTextEdit>
#include <QTextBlock>
#include <QVector>
#include <QTextTable>
#include <QTextTableCell>
#include <QTextTableCellFormat>
#include <QTextTableFormat>
#include <QTextCursor>
#include <QAction>
#include <QMenu>
#include <QTextFormat>
#include <QTextCharFormat>
#include <QTextBlockFormat>

class DocBlock;
class DocPassage;
class DocPage;
class DocLayer;

///
/// \brief The DocTable class
///         这个类专门表示表格
///         为一个QTextEdit、但内部内容仅有一个QTextTable
///
class MODELSHARED_EXPORT DocTable
        :public QTextEdit
{
    Q_OBJECT
public:
    DocTable(QWidget *parent = NULL);
    DocTable(int rows, int columns);

    ~DocTable();

    DocPassage *getPassage();
    DocPage *getPage();
    DocLayer *getLayer();
    DocBlock *getBlock();

    QMenu *getMenu();

public slots:
    void setTable(int rows, int columns);        // 设置表格长和宽
    void setDefaultStyle();                      // 设置默认样式
    void setBlock(DocBlock* block);              // 设置块


private slots:

private:
    // tableCell的内容估计使用QTextBlock
    DocBlock* _block;
    int rowCount;               // 行数
    int colCount;               // 列数
    QTextTable* _table;         // 该表格

    void init();                // 通用的初始化部分
    void initMenu();            // 初始化菜单
    void initConnection();      // 初始化信号槽链接

private slots:
    void blockSizeChanged();    // 根据文字内容自动调整块的大小
signals:
    void signals_currentTable(DocTable *table); // 当前操作的表格
};

#endif // TABLE_H
