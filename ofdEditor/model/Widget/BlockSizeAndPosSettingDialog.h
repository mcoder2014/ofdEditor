#ifndef BLOCKSIZEANDPOSSETTINGDIALOG_H
#define BLOCKSIZEANDPOSSETTINGDIALOG_H

#include <QDialog>

namespace Ui {
class BlockSizeAndPosSettingDialog;
}

class BlockSizeAndPosSettingDialog : public QDialog
{
    Q_OBJECT

public:
    explicit BlockSizeAndPosSettingDialog(QWidget *parent = 0);
    ~BlockSizeAndPosSettingDialog();

private:
    Ui::BlockSizeAndPosSettingDialog *ui;
};

#endif // BLOCKSIZEANDPOSSETTINGDIALOG_H
