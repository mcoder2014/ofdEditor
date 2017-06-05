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

private:
    // tableCell的内容估计使用QTextBlock
};

#endif // TABLE_H
