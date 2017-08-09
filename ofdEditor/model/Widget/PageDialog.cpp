#include "PageDialog.h"
#include "ui_PageDialog.h"
#include "../Doc/DocPassage.h"
#include "../Doc/DocPage.h"
#include <QDebug>
#include "../Tool/UnitTool.h"

PageDialog* PageDialog::m_instance = NULL;



///
/// \brief PageDialog::getInstance
/// \return
///
PageDialog *PageDialog::getInstance()
{
    if(m_instance != NULL)
    {
        return m_instance;
    }
    m_instance = new PageDialog();
    return m_instance;
}

void PageDialog::DestoryInstance()
{
    m_instance = NULL;
}

///
/// \brief PageDialog::init
/// \param current_page
/// \param default_width
/// \param default_height
/// \param using_working_area
/// \param default_working_width
/// \param default_working_height
/// \param default_working_x
/// \param default_working_y
///
void PageDialog::init(
        DocPassage* passage,
        DocPage *current_page,
        double default_width,
        double default_height,
        bool using_working_area,
        double default_working_width,
        double default_working_height,
        double default_working_x,
        double default_working_y)
{

    this->passage = passage;
    this->current_page = current_page;
    this->changed_page_numbers.clear();

    ui->CurrentPageRadioButton->setChecked(true);   // 选择 当前页
    ui->CurrentPageRadioButton->click();            // 点击，发送信号

    ui->CurrentSelectPageSize->setCurrentIndex(0);  // 暂时默认置零

    if (this->current_page)
    {
        ui->CurrentPageSizeWidth->setValue(
                    toNearestInt(
                            current_page->getWidth()));
        ui->CurrentPageSizeHeight->setValue(
                    toNearestInt(
                            current_page->getHeight()));

//        //Working area to be implemented!!!!
        ui->CurrentWorkingAreaWidth->setValue(
                    toNearestInt(
                        current_page->getContentWidth()));
        ui->CurrentWorkingAreaHeight->setValue(
                    toNearestInt(
                        current_page->getContentHeight()));
        ui->CurrentWorkingAreaX->setValue(
                    toNearestInt(
                        current_page->getContentX()));
        ui->CurrentWorkingAreaY->setValue(
                    toNearestInt(
                        current_page->getContentY()));

        ui->DefaultPageSizeWidth->setValue(
                    toNearestInt(default_height));
        ui->DefaultPageSizeHeight->setValue(
                    toNearestInt(default_width));

        //Working area to be implemented!!!!
        ui->DefaultSetWorkingAreaChecked->setChecked(using_working_area);
        ui->DefaultWorkingAreaWidth->setValue(
                    toNearestInt(default_working_width));
        ui->DefaultWorkingAreaHeight->setValue(
                    toNearestInt(default_working_height));
        ui->DefaultWorkingAreaX->setValue(
                    toNearestInt(default_working_x));
        ui->DefaultWorkingAreaY->setValue(
                    toNearestInt(default_working_y));

        this->isDefaultPageSizeChanged = false;     // 是否修改了默认尺寸

        this->connect(this,
                      SIGNAL(modifyPageSize(
                                 QVector<int>*,
                                 double,
                                 double,
                                 bool,
                                 double,
                                 double,
                                 double,
                                 double)),
                      this->passage,
                      SLOT(modifyPageSize(
                               QVector<int>*,
                               double,
                               double,
                               bool,
                               double,
                               double,
                               double,
                               double)));

        this->connect(this,
                      SIGNAL(modifyDefaultPageSize(
                                 double,
                                 double,
                                 bool,
                                 double,
                                 double,
                                 double,
                                 double)),
                      this->passage,
                      SLOT(modifyDefaultPageSize(
                               double,
                               double,
                               bool,
                               double,
                               double,
                               double,
                               double)));


    }
}


PageDialog::~PageDialog()
{
    delete ui;
}


///
/// \brief PageDialog::PageDialog
/// \param parent
///
PageDialog::PageDialog(QWidget *parent)
    :QDialog(parent),ui(new Ui::PageDialog)
{
    ui->setupUi(this);
    this->initConnect();
}

///
/// \brief PageDialog::initConnect
/// 初始化 信号槽
///
void PageDialog::initConnect()
{
    //创建信号槽

    // 四种模式的选择
    // 当前页
    this->connect(ui->CurrentPageRadioButton,
                  SIGNAL(toggled(bool)),
                  this,
                  SLOT(onCurrentPageRadioButtonToggled(bool)));
    // 所有页面
    this->connect(ui->AllPagesRadioButton,
                  SIGNAL(toggled(bool)),
                  this,
                  SLOT(onAllPagesRadioButtonToggled(bool)));
    // 页面范围
    this->connect(ui->PageRangeRadioButton,
                  SIGNAL(toggled(bool)),
                  this,
                  SLOT(onPageRangeRadioButtonToggled(bool)));
    // 特殊页面
    this->connect(ui->SpecificPageRadioButton,
                  SIGNAL(toggled(bool)),
                  this,
                  SLOT(onSpecificPageRadioButtonToggled(bool)));


    this->connect(ui->CurrentSetWorkingAreaChecked,
                  SIGNAL(clicked(bool)),
                  this,
                  SLOT(on_CurrentSetWorkingAreaChecked_clicked(bool)));

    this->connect(ui->DefaultSetWorkingAreaChecked,
                  SIGNAL(clicked(bool)),
                  this,
                  SLOT(on_DefaultSetWorkingAreaChecked_clicked(bool)));

    this->connect(ui->CurrentPageSizeHeight,
                  SIGNAL(editingFinished()),
                  this,
                  SLOT(on_CurrentPageSize_valueChanged()));

    this->connect(ui->CurrentPageSizeWidth,
                  SIGNAL(editingFinished()),
                  this,
                  SLOT(on_CurrentPageSize_valueChanged()));

    this->connect(ui->OkAndCancel,
                  SIGNAL(accepted()),
                  this,
                  SLOT(accept()));

    this->connect(ui->OkAndCancel,
                  SIGNAL(rejected()),
                  this,
                  SLOT(reject()));

    this->connect(ui->PageRangeLowerBound,
                  SIGNAL(valueChanged(int)),
                  this,
                  SLOT(page_range_range_changed()));

    this->connect(ui->PageRangeUpperBound,
                  SIGNAL(valueChanged(int)),
                  this,
                  SLOT(page_range_range_changed()));

    this->connect(ui->PageRangeLowerBound,
                  SIGNAL(valueChanged(int)),
                  this,
                  SLOT(changed_page_range_changed()));

    this->connect(ui->PageRangeUpperBound,
                  SIGNAL(valueChanged(int)),
                  this,
                  SLOT(changed_page_range_changed()));

    // 选择确定后的操作
    this->connect(this, SIGNAL(finished(int)),
                  this,SLOT(finished_slots(int)));

    // 判断是否修改过默认值
    this->connect(ui->DefaultPageSizeWidth,
                  SIGNAL(valueChanged(double)),
                  this,
                  SLOT(on_DefaultSizeChanged(double)));

    this->connect(ui->DefaultPageSizeHeight,
                  SIGNAL(valueChanged(double)),
                  this,
                  SLOT(on_DefaultSizeChanged(double)));

    this->connect(ui->DefaultWorkingAreaHeight,
                  SIGNAL(valueChanged(double)),
                  this,
                  SLOT(on_DefaultSizeChanged(double)));

    this->connect(ui->DefaultWorkingAreaWidth,
                  SIGNAL(valueChanged(double)),
                  this,
                  SLOT(on_DefaultSizeChanged(double)));

    this->connect(ui->DefaultWorkingAreaX,
                  SIGNAL(valueChanged(double)),
                  this,
                  SLOT(on_DefaultSizeChanged(double)));

    this->connect(ui->DefaultWorkingAreaY,
                  SIGNAL(valueChanged(double)),
                  this,
                  SLOT(on_DefaultSizeChanged(double)));

}

///
/// \brief PageDialog::caculatePages
/// 计算选择了哪些页面
///
void PageDialog::caculatePages()
{
    if(ui->CurrentPageRadioButton->isChecked())
    {
        // 当前页
        int i;
        if (current_page)
        {
            for (i = 0; i < passage->getPages()->size(); i++)
                if (passage->getPages()->operator [](i) == current_page)
                    break;
        }
        else i = 1;
        changed_page_numbers.push_back(i+1);
    }
    else if(ui->AllPagesRadioButton->isChecked())
    {
        // 所有页面
        for (int i = 0; i < passage->getPages()->size(); i++)
            changed_page_numbers.push_back(i + 1);
    }
    else if(ui->PageRangeRadioButton->isChecked())
    {
        // 页面范围
       int start = ui->PageRangeLowerBound->value();    // 开始
       int end = ui->PageRangeUpperBound->value();      // 截止
        for (int j = start ; j <= end; j++)
            changed_page_numbers.push_back(j);
    }
    else if(ui->SpecificPageRadioButton->isChecked())
    {
        // 特殊页面

    }
}

void PageDialog::on_CurrentSelectPageSize_currentIndexChanged(const QString &arg1)
{
    if (arg1 == tr("A4"))
    {
        ui->CurrentPageSizeWidth->setValue(210);
        ui->CurrentPageSizeHeight->setValue(297);
    } else if (arg1 == tr("A3"))
    {
        ui->CurrentPageSizeWidth->setValue(297);
        ui->CurrentPageSizeHeight->setValue(420);
    } else if (arg1 == tr("A5"))
    {
        ui->CurrentPageSizeWidth->setValue(148);
        ui->CurrentPageSizeHeight->setValue(210);
    } else if (arg1 == tr("Letter"))
    {
        ui->CurrentPageSizeWidth->setValue(216);
        ui->CurrentPageSizeHeight->setValue(279);
    } else if (arg1 == tr("16K"))
    {
        ui->CurrentPageSizeWidth->setValue(184);
        ui->CurrentPageSizeHeight->setValue(260);
    } else if (arg1 == tr("32K"))
    {
        ui->CurrentPageSizeWidth->setValue(130);
        ui->CurrentPageSizeHeight->setValue(184);
    }
}

void PageDialog::on_DefaultSelectPageSize_currentIndexChanged(const QString &arg1)
{
    if (arg1 == tr("A4"))
    {
        ui->DefaultPageSizeWidth->setValue(210);
        ui->DefaultPageSizeHeight->setValue(297);
    } else if (arg1 == tr("A3"))
    {
        ui->DefaultPageSizeWidth->setValue(297);
        ui->DefaultPageSizeHeight->setValue(420);
    } else if (arg1 == tr("A5"))
    {
        ui->DefaultPageSizeWidth->setValue(148);
        ui->DefaultPageSizeHeight->setValue(210);
    } else if (arg1 == tr("Letter"))
    {
        ui->DefaultPageSizeWidth->setValue(216);
        ui->DefaultPageSizeHeight->setValue(279);
    } else if (arg1 == tr("16K"))
    {
        ui->DefaultPageSizeWidth->setValue(184);
        ui->DefaultPageSizeHeight->setValue(260);
    } else if (arg1 == tr("32K"))
    {
        ui->DefaultPageSizeWidth->setValue(130);
        ui->DefaultPageSizeHeight->setValue(184);
    }
    this->isDefaultPageSizeChanged = true;
}

void PageDialog::on_SpecificPageCheckBox_clicked(bool checked)
{
    if (checked)
    {
        ui->SpecificPage->setEnabled(true);
//        !!!!!!!!!!!!!!!!!!!!!!
    }
    else
    {
        ui->SpecificPage->setText("");
        ui->SpecificPage->setEnabled(false);
    }
}

void PageDialog::on_CurrentSetWorkingAreaChecked_clicked(bool checked)
{
    if (checked)
    {
        ui->CurrentWorkingAreaWidth->setEnabled(true);
        ui->CurrentWorkingAreaHeight->setEnabled(true);
        ui->CurrentWorkingAreaX->setEnabled(true);
        ui->CurrentWorkingAreaY->setEnabled(true);
    }
    else
    {
        ui->CurrentWorkingAreaWidth->setEnabled(false);
        ui->CurrentWorkingAreaHeight->setEnabled(false);
        ui->CurrentWorkingAreaX->setEnabled(false);
        ui->CurrentWorkingAreaY->setEnabled(false);
        ui->CurrentWorkingAreaWidth->setValue(ui->CurrentPageSizeWidth->value());
        ui->CurrentWorkingAreaHeight->setValue(ui->CurrentPageSizeHeight->value());
        ui->CurrentWorkingAreaX->setValue(0.0);
        ui->CurrentWorkingAreaY->setValue(0.0);
    }
}

void PageDialog::on_DefaultSetWorkingAreaChecked_clicked(bool checked)
{
    if (checked)
    {
        ui->DefaultWorkingAreaWidth->setEnabled(true);
        ui->DefaultWorkingAreaHeight->setEnabled(true);
        ui->CurrentWorkingAreaX->setEnabled(true);
        ui->CurrentWorkingAreaY->setEnabled(true);
    }
    else
    {
        ui->DefaultWorkingAreaWidth->setEnabled(false);
        ui->DefaultWorkingAreaHeight->setEnabled(false);
        ui->DefaultWorkingAreaX->setEnabled(false);
        ui->DefaultWorkingAreaY->setEnabled(false);
        ui->DefaultWorkingAreaWidth->setValue(ui->DefaultPageSizeWidth->value());
        ui->DefaultWorkingAreaHeight->setValue(ui->DefaultPageSizeHeight->value());
        ui->DefaultWorkingAreaX->setValue(0.0);
        ui->DefaultWorkingAreaY->setValue(0.0);
    }

    this->isDefaultPageSizeChanged = true;
}

void PageDialog::on_DefaultSizeChanged(double value)
{
    this->isDefaultPageSizeChanged = true;
}

void PageDialog::on_CurrentPageSize_valueChanged()
{
//    qDebug() << "Called";
    ui->CurrentSelectPageSize->setCurrentIndex(ui->CurrentSelectPageSize->findText(tr("Customized")));
}

///
/// \brief PageDialog::onCurrentPageRadioButtonToggled
///         选择只选择当前页
/// \param checked
///
void PageDialog::onCurrentPageRadioButtonToggled(bool checked)
{
    if(checked)
    {
        // 页码范围
        ui->PageRangeLowerBound->setEnabled(false);
        ui->PageRangeUpperBound->setEnabled(false);

        // 特定
        ui->SpecificPage->setEnabled(false);
        ui->SpecificPage->setText("");
    }
}

///
/// \brief PageDialog::onAllPagesRadioButtonToggled
/// \param checked
///
void PageDialog::onAllPagesRadioButtonToggled(bool checked)
{
    if(checked)
    {
        // 页码范围
        ui->PageRangeLowerBound->setEnabled(false);
        ui->PageRangeUpperBound->setEnabled(false);

        // 特定
        ui->SpecificPage->setEnabled(false);
        ui->SpecificPage->setText("");
    }
}

///
/// \brief PageDialog::onPageRangeRadioButtonToggled
/// \param checked
///
void PageDialog::onPageRangeRadioButtonToggled(bool checked)
{
    if(checked)
    {
        // 页码范围
        ui->PageRangeLowerBound->setEnabled(true);
        ui->PageRangeUpperBound->setEnabled(true);

        int i;
        if (current_page)
        {
            for (i = 0; i < passage->getPages()->size(); i++)
                if (passage->getPages()->operator [](i) == current_page)
                    break;
        }
        else i = 1;

        ui->PageRangeLowerBound->setValue(i);
        ui->PageRangeUpperBound->setValue(
                    passage->getPages()->size());
        ui->PageRangeLowerBound->setRange(
                    1,
                    ui->PageRangeUpperBound->value());
        ui->PageRangeUpperBound->setRange(
                    ui->PageRangeLowerBound->value(),
                    passage->getPages()->size());

        // 特定
        ui->SpecificPage->setEnabled(false);
        ui->SpecificPage->setText("");
    }
}

void PageDialog::onSpecificPageRadioButtonToggled(bool checked)
{
    if(checked)
    {
        // 页码范围
        ui->PageRangeLowerBound->setEnabled(false);
        ui->PageRangeUpperBound->setEnabled(false);

        // 特定
        ui->SpecificPage->setEnabled(true);
    }
}

void PageDialog::page_range_range_changed()
{
    ui->PageRangeLowerBound->setRange(
                1,
                ui->PageRangeUpperBound->value());
    ui->PageRangeUpperBound->setRange(
                ui->PageRangeLowerBound->value(),
                passage->getPages()->size());
}

void PageDialog::changed_page_range_changed()
{
    changed_page_numbers.clear();
    for (int j = ui->PageRangeLowerBound->value(); j <= ui->PageRangeUpperBound->value(); j++)
        changed_page_numbers.push_back(j);
}

///
/// \brief PageDialog::emitInformation
///
void PageDialog::emitInformation()
{


}

///
/// \brief PageDialog::finished_slots
/// \param value
///
void PageDialog::finished_slots(int value)
{
    if(value == QDialog::Accepted)
    {
        // 如果用户选择了确定
        this->caculatePages();      // 计算选择的页数
        emit this->modifyPageSize(
                    &changed_page_numbers,
                    ui->CurrentPageSizeWidth->value(),
                    ui->CurrentPageSizeHeight->value(),
                    ui->CurrentSetWorkingAreaChecked->isChecked(),
                    ui->CurrentWorkingAreaWidth->value(),
                    ui->CurrentWorkingAreaHeight->value(),
                    ui->CurrentWorkingAreaX->value(),
                    ui->CurrentWorkingAreaY->value());

        if(isDefaultPageSizeChanged)
        {
            emit this->modifyDefaultPageSize(
                        ui->DefaultPageSizeWidth->value(),
                        ui->DefaultPageSizeHeight->value(),
                        ui->DefaultSetWorkingAreaChecked->isChecked(),
                        ui->DefaultWorkingAreaWidth->value(),
                        ui->DefaultWorkingAreaHeight->value(),
                        ui->DefaultWorkingAreaX->value(),
                        ui->DefaultWorkingAreaY->value());
        }
    }

    // 断开链接
    this->disconnect(this,
                  SIGNAL(modifyPageSize(
                             QVector<int>*,
                             double,
                             double,
                             bool,
                             double,
                             double,
                             double,
                             double)),
                  this->passage,
                  SLOT(modifyPageSize(
                           QVector<int>*,
                           double,
                           double,
                           bool,
                           double,
                           double,
                           double,
                           double)));

    this->disconnect(this,
                  SIGNAL(modifyDefaultPageSize(
                             double,
                             double,
                             bool,
                             double,
                             double,
                             double,
                             double)),
                  this->passage,
                  SLOT(modifyDefaultPageSize(
                           double,
                           double,
                           bool,
                           double,
                           double,
                           double,
                           double)));
}

