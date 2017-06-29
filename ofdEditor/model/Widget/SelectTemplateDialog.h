#ifndef SELECTTEMPLATEDIALOG_H
#define SELECTTEMPLATEDIALOG_H

#include <QDialog>
#include <model_global.h>
#include <QVector>
#include <QPixmap>

class PassageMainWindow;

namespace Ui {
class SelectTemplateDialog;
}

class MODELSHARED_EXPORT SelectTemplateDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SelectTemplateDialog(QWidget *parent, PassageMainWindow * mainwindow);
    ~SelectTemplateDialog();

private:
    Ui::SelectTemplateDialog *ui;
    PassageMainWindow * mainwindow;
    int current_template_index;
    QVector<QPixmap *> icons;

private slots:
    void nextTemplate();
    void lastTemplate();
    void emitCreateTemplateSignal();


signals:
    void createTemplate(int index);
};


#endif // SELECTTEMPLATEDIALOG_H
