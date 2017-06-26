#ifndef PAGEDIALOG_H
#define PAGEDIALOG_H

#include <QDialog>
#include <QVector>

class DocPassage;
class DocPage;


namespace Ui {
class PageDialog;
}

class PageDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PageDialog(QWidget *parent = 0, DocPassage* passage = NULL);
    ~PageDialog();
    void updateInformation(DocPage * current_page,
                           double default_width,
                           double default_height,
                           bool using_working_area,
                           double default_working_width,
                           double default_working_height,
                           double default_working_x,
                           double default_working_y);

private slots:

    void on_CurrentSelectPageSize_currentIndexChanged(const QString &arg1);

    void on_DefaultSelectPageSize_currentIndexChanged(const QString &arg1);

    void on_SpecificPageCheckBox_clicked(bool checked);

    void on_CurrentSetWorkingAreaChecked_clicked(bool checked);

    void on_DefaultSetWorkingAreaChecked_clicked(bool checked);

    void on_CurrentPageSize_valueChanged();


    void on_PageRangeCheckBox_toggled(bool checked);

    void on_SpecificPageCheckBox_toggled(bool checked);


    void on_CurrentPageCheckBox_toggled(bool checked);

    void on_AllPagesCheckBox_toggled(bool checked);

    void page_range_range_changed();

    void changed_page_range_changed();

    void emitInformation();
private:
    Ui::PageDialog *ui;
    DocPassage *passage;
    QVector<int> changed_page_numbers;
    DocPage * current_page;
};

#endif // PAGEDIALOG_H
