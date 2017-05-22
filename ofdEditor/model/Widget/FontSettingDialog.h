#ifndef FONTSETTINGDIALOG_H
#define FONTSETTINGDIALOG_H

#include <QDialog>

namespace Ui {
class FontSettingDialog;
}

class FontSettingDialog : public QDialog
{
    Q_OBJECT

public:
    explicit FontSettingDialog(QWidget *parent = 0);
    ~FontSettingDialog();

private:
    Ui::FontSettingDialog *ui;
};

#endif // FONTSETTINGDIALOG_H
