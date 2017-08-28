#ifndef TABLESETTINGDIALOG_H
#define TABLESETTINGDIALOG_H

#include <QDialog>
#include <QColor>
#include "model_global.h"
#include <QTextLength>
#include <QVector>


class DocTable;

namespace Ui {
class TableSettingDialog;
}

class MODELSHARED_EXPORT TableSettingDialog
        : public QDialog
{
    Q_OBJECT

public:

    static TableSettingDialog* getInstance();
    static void destroyInstance();
    static TableSettingDialog* getInstance(DocTable* table);
    void init(DocTable* table);

    ~TableSettingDialog();

private:
    explicit TableSettingDialog(QWidget *parent = 0);
    Ui::TableSettingDialog *ui;
    static TableSettingDialog* m_instance;          // instance
    DocTable* _table;                               // 存储下临时可用的表格

    int rows;
    int columns;

    double tableWidth_mm;               // 表格宽度 mm
    double tableX_mm;                   // 表格左上角位置
    double tableY_mm;
    double cellPadding;                 //单元格留白
    QVector<QTextLength> cellWidth;     // 单元格的宽度
    bool isDefaultCellWidth;            // 是否均分列宽

    QColor tableColor;                  // 表格颜色

private slots:

    void slot_selectBalanceCol();       // 选择等分
    void slot_selectCustomCol();        // 选择自定义调整列宽
    void setColor();                    // 点击按钮修改颜色


    void finished_slots( int value );   // 完成后调用

};

#endif // TABLESETTINGDIALOG_H
