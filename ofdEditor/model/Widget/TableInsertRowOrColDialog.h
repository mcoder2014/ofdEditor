#ifndef TABLEINSERTROWORCOLDIALOG_H
#define TABLEINSERTROWORCOLDIALOG_H

#include <QDialog>

namespace Ui {
class TableInsertRowOrColDialog;
}

class TableInsertRowOrColDialog : public QDialog
{
    Q_OBJECT

public:
    explicit TableInsertRowOrColDialog(QWidget *parent = 0);
    ~TableInsertRowOrColDialog();

private:
    Ui::TableInsertRowOrColDialog *ui;
};

#endif // TABLEINSERTROWORCOLDIALOG_H
