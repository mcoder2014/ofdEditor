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
#include "Doc/DocTextBlock.h"

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
        :public DocTextBlock
{
    Q_OBJECT
public:
    DocTable(QWidget *parent = NULL);
    DocTable(int rows, int columns);

     ~DocTable();

    QString getType();                          // 获得类型
    QMenu *getMenu();

public slots:
    void setTable(int rows, int columns);        // 设置表格长和宽
    void setDefaultStyle();                      // 设置默认样式
    void setBlock(DocBlock* block);              // 设置块

    // 插入行或列
    void insertRowBefore();
    void insertRow(
            QTextCursor cursor,
            int flag);
    void insertRowAfter();
    void insertColBefore();
    void insertColAfter();

    // 合并单元格
    void mergeCells();

    // 拆分单元格
    void splitCells();

    // 删除
    void delRow();
    void delCol();

    // 单元格设置
    void tableSetting();
    void updateStyle();     // 更新表格的样式

protected:
    void focusInEvent(QFocusEvent *e);
    void focusOutEvent(QFocusEvent *e);

private slots:
    void selectedCells(QTextCursor& cursor);

private:
    int rowCount;               // 行数
    int colCount;               // 列数
    QTextTable* _table;         // 该表格
    QVector<QTextLength> colWidth;  // 列宽设置
    bool isDefaultStyle;

    // 选择的结果
    int firstRow;
    int firstColumn;
    int rows;
    int columns;

    void init();                // 通用的初始化部分
    void initAction();          // 初始化Action
    void initMenu();            // 初始化菜单
    void initConnection();      // 初始化信号槽链接

    QMenu *menu_insertRowOrCol;     // 插入行或列
    QAction *actionInsertRowBefore; // 在当前单元格之前插入行
    QAction *actionInsertRowAfter;  // 在当前单元格之后插入行
    QAction *actionInsertColBefore; // 在当前单元格之前插入列
    QAction *actionInsertColAfter;  // 在当前单元格之后插入列

    QAction *actionMergeCells;      // 合并单元格
    QAction *actionSplitCells;      // 拆分单元格

    QAction *actionDeleteRow;       // 删除该行
    QAction *actionDeleteColumn;    // 删除该列
    QAction *actionTableSetting;    // 表格设置-对话框解决

private slots:
    void blockSizeChanged();    // 根据文字内容自动调整块的大小
signals:
    void signals_currentTable(DocTable *table); // 当前操作的表格
};

#endif // TABLE_H
