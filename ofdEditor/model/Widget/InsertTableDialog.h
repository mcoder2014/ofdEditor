#ifndef INSERTTABLEDIALOG_H
#define INSERTTABLEDIALOG_H

#include <QDialog>

namespace Ui {
class InsertTableDialog;
}

class InsertTableDialog : public QDialog
{
    Q_OBJECT

public:
    explicit InsertTableDialog(QWidget *parent = 0);
    ~InsertTableDialog();

private:
    Ui::InsertTableDialog *ui;
};

#endif // INSERTTABLEDIALOG_H
