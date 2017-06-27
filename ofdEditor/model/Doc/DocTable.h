#ifndef TABLE_H
#define TABLE_H

#include "model_global.h"
#include "Doc/DocBasicTypes.h"
#include <QTextEdit>
#include <QTextBlock>
#include <QVector>


// 这个类专门用来表示表格
class MODELSHARED_EXPORT DocTable
        :public QTextEdit
{
    Q_OBJECT
public:
    DocTable(QWidget *parent = NULL);
    ~DocTable();

public slots:

private slots:

private:
    // tableCell的内容估计使用QTextBlock
    int rowCount;       // 行数
    int colCount;       // 列数
};

#endif // TABLE_H
