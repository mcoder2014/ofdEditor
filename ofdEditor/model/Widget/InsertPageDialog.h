#ifndef INSERTPAGEDIALOG_H
#define INSERTPAGEDIALOG_H

#include <QDialog>

namespace Ui {
class InsertPageDialog;
}

///
/// \brief The InsertPageDialog class
///     用来在页面中弹出，插入页面的类
///
class InsertPageDialog
        : public QDialog
{
    Q_OBJECT

public:
    explicit InsertPageDialog(QWidget *parent = 0);
    ~InsertPageDialog();

private:
    Ui::InsertPageDialog *ui;
};

#endif // INSERTPAGEDIALOG_H
