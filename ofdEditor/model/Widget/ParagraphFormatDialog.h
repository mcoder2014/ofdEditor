#ifndef PARAGRAPHFORMATDIALOG_H
#define PARAGRAPHFORMATDIALOG_H

#include <QDialog>

namespace Ui {
class ParagraphFormatDialog;
}

class ParagraphFormatDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ParagraphFormatDialog(QWidget *parent = 0);
    ~ParagraphFormatDialog();

private:
    Ui::ParagraphFormatDialog *ui;
};

#endif // PARAGRAPHFORMATDIALOG_H
