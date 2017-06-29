#ifndef INSERTTABLEDIALOG_H
#define INSERTTABLEDIALOG_H
#include "model_global.h"

#include <QDialog>

namespace Ui {
class InsertTableDialog;
}

class  MODELSHARED_EXPORT InsertTableDialog : public QDialog
{
    Q_OBJECT

public:
    explicit InsertTableDialog(QWidget *parent = 0);
    ~InsertTableDialog();

private:
    Ui::InsertTableDialog *ui;
};

#endif // INSERTTABLEDIALOG_H
