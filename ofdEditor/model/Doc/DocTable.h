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

public slots:
    setTable(int rows, int columns);        // 设置表格长和宽

private slots:

private:
    // tableCell的内容估计使用QTextBlock
    int rowCount;       // 行数
    int colCount;       // 列数
    QTextTable* _table; // 该表格
};

#endif // TABLE_H
