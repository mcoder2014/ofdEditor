#include "PageDialog.h"
#include "ui_PageDialog.h"
#include "../Doc/DocPassage.h"
#include "../Doc/DocPage.h"
#include <QDebug>
#include "../Tool/UnitTool.h"

int toNearestInt(double n)
{
    return n + 0.5;
}

PageDialog::PageDialog(QWidget *parent, DocPassage * passage) :
    QDialog(parent),
    ui(new Ui::PageDialog),
    changed_page_numbers()
{
    ui->setupUi(this);
    this->passage = passage;

    //成员初始化
    current_page = NULL;
    //组件初始化
    if (current_page)
    {
        ui->CurrentPageSizeWidth->setValue(current_page->size().width());
        ui->CurrentPageSizeHeight->setValue(current_page->size().height());
    }
    ui->CurrentPageSizeWidth->setRange(0.0, 10000.0);
    ui->CurrentPageSizeHeight->setRange(0.0, 10000.0);
    ui->DefaultPageSizeWidth->setRange(0.0, 10000.0);
    ui->DefaultPageSizeHeight->setRange(0.0, 10000.0);
    ui->CurrentWorkingAreaWidth->setRange(0.0, 10000.0);
    ui->CurrentWorkingAreaHeight->setRange(0.0, 10000.0);
    ui->DefaultWorkingAreaWidth->setRange(0.0, 10000.0);
    ui->DefaultWorkingAreaHeight->setRange(0.0, 10000.0);
    ui->CurrentWorkingAreaX->setRange(0.0, 10000.0);
    ui->CurrentWorkingAreaY->setRange(0.0, 10000.0);
    ui->DefaultWorkingAreaX->setRange(0.0, 10000.0);
    ui->DefaultWorkingAreaY->setRange(0.0, 10000.0);

    ui->CurrentSetWorkingAreaChecked->setChecked(false);
    ui->CurrentWorkingAreaWidth->setEnabled(false);
    ui->CurrentWorkingAreaHeight->setEnabled(false);
    ui->CurrentWorkingAreaX->setEnabled(false);
    ui->CurrentWorkingAreaY->setEnabled(false);
    ui->CurrentWorkingAreaWidth->setValue(ui->CurrentPageSizeWidth->value());
    ui->CurrentWorkingAreaHeight->setValue(ui->CurrentPageSizeHeight->value());
    ui->CurrentWorkingAreaX->setValue(0.0);
    ui->CurrentWorkingAreaY->setValue(0.0);

    ui->DefaultSetWorkingAreaChecked->setChecked(false);
    ui->DefaultWorkingAreaWidth->setEnabled(false);
    ui->DefaultWorkingAreaHeight->setEnabled(false);
    ui->DefaultWorkingAreaX->setEnabled(false);
    ui->DefaultWorkingAreaY->setEnabled(false);
    ui->DefaultWorkingAreaWidth->setValue(ui->DefaultPageSizeWidth->value());
    ui->DefaultWorkingAreaHeight->setValue(ui->DefaultPageSizeHeight->value());
    ui->DefaultWorkingAreaX->setValue(0.0);
    ui->DefaultWorkingAreaY->setValue(0.0);

    ui->CurrentSelectPageSize->setEditable(true);
    ui->DefaultSelectPageSize->setEditable(true);

    ui->CurrentPageCheckBox->setChecked(true);
    ui->AllPagesCheckBox->setChecked(false);
    ui->PageRangeCheckBox->setChecked(false);
    ui->SpecificPageCheckBox->setChecked(false);

    ui->PageRangeLowerBound->setEnabled(false);
    ui->PageRangeUpperBound->setEnabled(false);
    ui->PageRangeLowerBound->setValue(1);
    ui->PageRangeUpperBound->setValue(passage->getPages().size());
    ui->SpecificPage->setEnabled(false);
    //创建信号槽
    this->connect(ui->PageRangeCheckBox,
                  SIGNAL(clicked(bool)),
                  this,
                  SLOT(on_PageRangeCheckBox_toggled(bool)));
    this->connect(ui->CurrentSelectPageSize,
                  SIGNAL(currentIndexChanged(QString)),
                  this,
                  SLOT(on_CurrentSelectPageSize_currentIndexChanged(QString)));
    this->connect(ui->DefaultSelectPageSize,
                  SIGNAL(currentIndexChanged(QString)),
                  this,
                  SLOT(on_DefaultSelectPageSize_currentIndexChanged(QString)));
    this->connect(ui->SpecificPageCheckBox,
                  SIGNAL(clicked(bool)),
                  this,
                  SLOT(on_SpecificPageCheckBox_clicked(bool)));
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
    this->connect(ui->CurrentPageCheckBox,
                  SIGNAL(toggled(bool)),
                  this,
                  SLOT(on_CurrentPageCheckBox_toggled(bool)));
    this->connect(ui->AllPagesCheckBox,
                  SIGNAL(toggled(bool)),
                  this,
                  SLOT(on_AllPagesCheckBox_toggled(bool)));
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

    this->connect(this,
                  SIGNAL(accepted()),
                  this,
                  SLOT(emitInformation()));
}

PageDialog::~PageDialog()
{
    delete ui;
}

void PageDialog::updateInformation(DocPage *current_page,
                                   double default_width,
                                   double default_height,
                                   bool using_working_area,
                                   double default_working_width,
                                   double default_working_height,
                                   double default_working_x,
                                   double default_working_y)
{
    this->current_page = current_page;
    if (current_page)
    {
//        qDebug() << "Current_page width in pixel = " << current_page->size().width();
//        qDebug() << "Current_page height in pixel = " << current_page->size().height();
        ui->CurrentPageSizeWidth->setValue(toNearestInt(UnitTool::pixelToMM(current_page->size().width())));
        ui->CurrentPageSizeHeight->setValue(toNearestInt(UnitTool::pixelToMM(current_page->size().height())));
        //Working area to be implemented!!!!
        ui->CurrentWorkingAreaWidth->setValue(toNearestInt(current_page->working_area_width));
        ui->CurrentWorkingAreaHeight->setValue(toNearestInt(current_page->working_area_height));
        ui->CurrentWorkingAreaX->setValue(toNearestInt(current_page->working_area_x));
        ui->CurrentWorkingAreaY->setValue(toNearestInt(current_page->working_area_y));

        ui->DefaultPageSizeWidth->setValue(toNearestInt(default_height));
        ui->DefaultPageSizeHeight->setValue(toNearestInt(default_width));
        //Working area to be implemented!!!!
        ui->DefaultSetWorkingAreaChecked->setChecked(using_working_area);
        ui->DefaultWorkingAreaWidth->setValue(toNearestInt(default_working_width));
        ui->DefaultWorkingAreaHeight->setValue(toNearestInt(default_working_height));
        ui->DefaultWorkingAreaX->setValue(toNearestInt(default_working_x));
        ui->DefaultWorkingAreaY->setValue(toNearestInt(default_working_y));

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
}

void PageDialog::on_CurrentPageSize_valueChanged()
{
//    qDebug() << "Called";
    ui->CurrentSelectPageSize->setCurrentIndex(ui->CurrentSelectPageSize->findText(tr("Customized")));
}




void PageDialog::on_PageRangeCheckBox_toggled(bool checked)
{
    if (checked)
    {
        ui->PageRangeLowerBound->setEnabled(true);
        ui->PageRangeUpperBound->setEnabled(true);
        int i;
        if (current_page)
        {
            for (i = 0; i < passage->getPages().size(); i++)
                if (passage->getPages()[i] == current_page)
                    break;
        }
        else i = 1;
        ui->PageRangeLowerBound->setValue(i);
        ui->PageRangeUpperBound->setValue(passage->getPages().size());
        ui->PageRangeLowerBound->setRange(1, ui->PageRangeUpperBound->value());
        ui->PageRangeUpperBound->setRange(ui->PageRangeLowerBound->value(), passage->getPages().size());
        for (int j = i; j <= passage->getPages().size(); j++)
            changed_page_numbers.push_back(j);
    }
    else
    {
        ui->PageRangeLowerBound->setEnabled(false);
        ui->PageRangeUpperBound->setEnabled(false);

    }
}

void PageDialog::on_SpecificPageCheckBox_toggled(bool checked)
{
    if (checked)
    {
        ui->SpecificPage->setEnabled(true);
    }
    else
    {
        ui->SpecificPage->setEnabled(false);
    }
}

void PageDialog::on_CurrentPageCheckBox_toggled(bool checked)
{
    if (checked)
    {
        changed_page_numbers.clear();
        int i;
        if (current_page)
        {
            for (i = 0; i < passage->getPages().size(); i++)
                if (passage->getPages()[i] == current_page)
                    break;
        }
        else i = 1;
        changed_page_numbers.push_back(i);
    }
}

void PageDialog::on_AllPagesCheckBox_toggled(bool checked)
{
    if (checked)
    {
        changed_page_numbers.clear();
        int i;
        for (i = 0; i < passage->getPages().size(); i++)
            changed_page_numbers.push_back(i + 1);
    }
}

void PageDialog::page_range_range_changed()
{
    ui->PageRangeLowerBound->setRange(1, ui->PageRangeUpperBound->value());
    ui->PageRangeUpperBound->setRange(ui->PageRangeLowerBound->value(), passage->getPages().size());
}

void PageDialog::changed_page_range_changed()
{
    changed_page_numbers.clear();
    for (int j = ui->PageRangeLowerBound->value(); j <= ui->PageRangeUpperBound->value(); j++)
        changed_page_numbers.push_back(j);
}

void PageDialog::emitInformation()
{
    passage->updatePageSizeInformation(changed_page_numbers,
                                       ui->CurrentPageSizeWidth->value(),
                                       ui->CurrentPageSizeHeight->value(),
                                       ui->CurrentSetWorkingAreaChecked->isChecked(),
                                       ui->CurrentWorkingAreaWidth->value(),
                                       ui->CurrentWorkingAreaHeight->value(),
                                       ui->CurrentWorkingAreaX->value(),
                                       ui->CurrentWorkingAreaY->value(),
                                       ui->DefaultPageSizeWidth->value(),
                                       ui->DefaultPageSizeHeight->value(),
                                       ui->DefaultSetWorkingAreaChecked->isChecked(),
                                       ui->DefaultWorkingAreaWidth->value(),
                                       ui->DefaultWorkingAreaHeight->value(),
                                       ui->DefaultWorkingAreaX->value(),
                                       ui->DefaultWorkingAreaY->value());
}

