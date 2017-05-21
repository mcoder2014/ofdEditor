#ifndef PARAGRAPHFORMATDIALOG_H
#define PARAGRAPHFORMATDIALOG_H

#include "model_global.h"
#include <QDialog>

namespace MODELSHARED_EXPORT Ui {
class MODELSHARED_EXPORT ParagraphFormatDialog;
}

class MODELSHARED_EXPORT ParagraphFormatDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ParagraphFormatDialog(QWidget *parent = 0);
    ~ParagraphFormatDialog();

private:
    Ui::ParagraphFormatDialog *ui;
};

#endif // PARAGRAPHFORMATDIALOG_H
