#ifndef DOCINFODIALOG_H
#define DOCINFODIALOG_H

#include <QDialog>

namespace Ui {
class DocInfoDialog;
}

class DocInfoDialog : public QDialog
{
    Q_OBJECT

public:
    explicit DocInfoDialog(QWidget *parent = 0);
    ~DocInfoDialog();

private:
    Ui::DocInfoDialog *ui;
};

#endif // DOCINFODIALOG_H
