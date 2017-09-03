#ifndef BLOCKSIZEANDPOSSETTINGDIALOG_H
#define BLOCKSIZEANDPOSSETTINGDIALOG_H

#include <QDialog>
#include "model_global.h"


class DocBlock;

namespace Ui {
class BlockSizeAndPosSettingDialog;
}

class MODELSHARED_EXPORT BlockSizeAndPosSettingDialog
        : public QDialog
{
    Q_OBJECT

public:

    static BlockSizeAndPosSettingDialog *getInstance();     // 获得单例
    static void DestoryInsatance();                         // 销毁单例
    void init(DocBlock* block);


    ~BlockSizeAndPosSettingDialog();

private:
    explicit BlockSizeAndPosSettingDialog(QWidget *parent = 0);
    Ui::BlockSizeAndPosSettingDialog *ui;

    static BlockSizeAndPosSettingDialog* m_instance;
    DocBlock *_block;


    void setUI(
            int width,
            int height,
            int x,
            int y);

public slots:
    void slot_finished(int value);
};

#endif // BLOCKSIZEANDPOSSETTINGDIALOG_H
