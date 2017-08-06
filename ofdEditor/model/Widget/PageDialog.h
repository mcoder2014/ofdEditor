#ifndef PAGEDIALOG_H
#define PAGEDIALOG_H

#include <QDialog>
#include <QVector>

class DocPassage;
class DocPage;


namespace Ui {
class PageDialog;
}

///
/// \brief The PageDialog class
///
class PageDialog
        : public QDialog
{
    Q_OBJECT

public:

    static PageDialog* getInstance();           // 获得单例
    static void DestoryInstance();              // 销毁实例
    void init(DocPage * current_page,
              double default_width,
              double default_height,
              bool using_working_area,
              double default_working_width,
              double default_working_height,
              double default_working_x,
              double default_working_y);             // 初始化时调用

    ~PageDialog();

private:
    explicit PageDialog(QWidget *parent = 0);   // 构造函数
    void initUI();
    void initConnect();

    static PageDialog* m_instance;

private slots:

    void on_CurrentSelectPageSize_currentIndexChanged(const QString &arg1);

    void on_DefaultSelectPageSize_currentIndexChanged(const QString &arg1);

    void on_SpecificPageCheckBox_clicked(bool checked);

    void on_CurrentSetWorkingAreaChecked_clicked(bool checked);

    void on_DefaultSetWorkingAreaChecked_clicked(bool checked);

    void on_DefaultSizeChanged(double value);   // 用来判断是否有修改过默认值

    void on_CurrentPageSize_valueChanged();

    void on_PageRangeCheckBox_toggled(bool checked);

    void on_SpecificPageCheckBox_toggled(bool checked);

    void on_CurrentPageCheckBox_toggled(bool checked);

    void on_AllPagesCheckBox_toggled(bool checked);

    void page_range_range_changed();

    void changed_page_range_changed();

    void emitInformation();

signals:
    void modifyPageSize(QVector<int> * ch_pages,
                        double _width,
                        double _height,
                        bool isUsingWorkingArea,
                        double contentWidth,
                        double contentHeight,
                        double contentX,
                        double contentY);

    void modifyDefaultPageSize(
            double default_width,
            double default_height,
            bool default_isUsingWorkingArea,
            double default_contentWidth,
            double default_contentHeight,
            double default_contentX,
            double default_contentY
            );

private:
    Ui::PageDialog *ui;
    DocPassage *passage;
    QVector<int> changed_page_numbers;
    DocPage * current_page;

    bool isDefaultPageSizeChanged;
};

#endif // PAGEDIALOG_H
