#ifndef PROGRAMSETTINGDIALOG_H
#define PROGRAMSETTINGDIALOG_H

#include <QDialog>

namespace Ui {
class ProgramSettingDialog;
}

class ProgramSettingDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ProgramSettingDialog(QWidget *parent = 0);
    ~ProgramSettingDialog();

private:
    Ui::ProgramSettingDialog *ui;
};

#endif // PROGRAMSETTINGDIALOG_H
