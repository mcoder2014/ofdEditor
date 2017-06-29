#include "SelectTemplateDialog.h"
#include "ui_SelectTemplateDialog.h"
#include <QDebug>
#
SelectTemplateDialog::SelectTemplateDialog(QWidget *parent, PassageMainWindow * mainwindow) :
    QDialog(parent),
    ui(new Ui::SelectTemplateDialog)
{
    ui->setupUi(this);
    this->mainwindow = mainwindow;
    //初始化成员
    current_template_index = 0;
    ui->CountLabel->setAlignment(Qt::AlignCenter);
    //置入模板Icon
    this->icons.push_back(new QPixmap(":/template_icon/template_icons/template_1.png"));
//    qDebug() << "Icon 1 is null: " << icons[0]->isNull();
    this->icons.push_back(new QPixmap(":/template_icon/template_icons/template_2 .png"));
    this->icons.push_back(new QPixmap(":/template_icon/template_icons/template_3.png"));
    this->icons.push_back(new QPixmap(":/template_icon/template_icons/template_4.png"));
    this->icons.push_back(new QPixmap(":/template_icon/template_icons/template_5.png"));

    ui->TemplateIcon->setPixmap(*(icons[current_template_index]));
    ui->CountLabel->setText(QString::number(current_template_index + 1)
                            + "/" + QString::number(icons.size()));

    //链接信号槽
    this->connect(ui->NextTemplateButton,
                  SIGNAL(clicked(bool)),
                  this,
                  SLOT(nextTemplate()));
    this->connect(ui->LastTemplateButton,
                  SIGNAL(clicked(bool)),
                  this,
                  SLOT(lastTemplate()));
    this->connect(ui->buttonBox,
                  SIGNAL(accepted()),
                  this,
                  SLOT(emitCreateTemplateSignal()));
}

SelectTemplateDialog::~SelectTemplateDialog()
{
    delete ui;
}


void SelectTemplateDialog::nextTemplate()
{
    if (current_template_index + 1 < icons.size())
    {
        current_template_index++;
        ui->TemplateIcon->setPixmap(*icons[current_template_index]);
        ui->CountLabel->setText(QString::number(current_template_index + 1)
                                + "/" + QString::number(icons.size()));
    }
}

void SelectTemplateDialog::lastTemplate()
{
    if (current_template_index - 1 >= 0)
    {
        current_template_index--;
        ui->TemplateIcon->setPixmap(*icons[current_template_index]);
        ui->CountLabel->setText(QString::number(current_template_index + 1)
                                + "/" + QString::number(icons.size()));
    }
}

void SelectTemplateDialog::emitCreateTemplateSignal()
{
    qDebug() << "Emit create template signal.";
    emit createTemplate(current_template_index);
}
