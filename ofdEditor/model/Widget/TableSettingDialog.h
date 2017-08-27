#ifndef TABLESETTINGDIALOG_H
#define TABLESETTINGDIALOG_H

#include <QDialog>

namespace Ui {
class TableSettingDialog;
}

class TableSettingDialog : public QDialog
{
    Q_OBJECT

public:
    explicit TableSettingDialog(QWidget *parent = 0);
    ~TableSettingDialog();

private:
    Ui::TableSettingDialog *ui;
};

#endif // TABLESETTINGDIALOG_H
