#include "TableSettingDialog.h"
#include "ui_TableSettingDialog.h"
#include "Doc/DocTable.h"
#include "Doc/DocBlock.h"
#include "Tool/UnitTool.h"
#include <QDebug>
#include <QColor>
#include <QColorDialog>

TableSettingDialog * TableSettingDialog::m_instance = NULL;

TableSettingDialog::TableSettingDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TableSettingDialog)
{
    ui->setupUi(this);

    connect(ui->balanceCol, SIGNAL(clicked(bool)),
            this, SLOT(slot_selectBalanceCol()));

    connect(ui->customCol, SIGNAL(clicked(bool)),
            this, SLOT(slot_selectCustomCol()));

    connect(this, SIGNAL(finished(int)),
            this, SLOT(finished_slots(int)));

    connect(ui->table_color, SIGNAL(clicked(bool)),
            this, SLOT(setColor()));
}

///
/// \brief TableSettingDialog::slot_selectBalanceCol
///
void TableSettingDialog::slot_selectBalanceCol()
{
    this->isDefaultCellWidth = true;

    ui->setTableCellWidth->clear();
    int length = this->_table->getColumns();
    ui->setTableCellWidth->setColumnCount(length);
    ui->setTableCellWidth->horizontalHeader()->setResizeMode(
                QHeaderView::Stretch);

    for(int i =0; i < length; i++)
    {
        this->cellWidth.append(
                    QTextLength(
                        QTextLength::PercentageLength,(100.0 / length)));
    }

}

///
/// \brief TableSettingDialog::slot_selectCustomCol
///
void TableSettingDialog::slot_selectCustomCol()
{
    this->isDefaultCellWidth = false;

    ui->setTableCellWidth->clear();
    int length = this->_table->getColumns();
    this->cellWidth = this->_table->getColumnWidth();
    ui->setTableCellWidth->setColumnCount(length);
    ui->setTableCellWidth->horizontalHeader()->setResizeMode(
                QHeaderView::Interactive);
    ui->setTableCellWidth->horizontalHeader()->setResizeMode(
                length-1, QHeaderView::Stretch);

    for(int i = 0; i < length; i++)
    {
        ui->setTableCellWidth->setHorizontalHeaderItem(
                    i, new QTableWidgetItem(QString::number(i+1)));
        ui->setTableCellWidth->setColumnWidth(
                    i, ui->setTableCellWidth->viewport()->width() *this->cellWidth[i].rawValue() / 100);
    }

}

void TableSettingDialog::setColor()
{
    QColor color = QColorDialog::getColor(this->tableColor,
                                          NULL,
                                          tr("Choose table border color"));
    if(color.isValid())
    {
       this->tableColor = color;
    }
}

///
/// \brief TableSettingDialog::finished_slots
/// \param value
///
void TableSettingDialog::finished_slots(int value)
{
    this->tableWidth_mm = ui->tableWidth->value();
    this->cellPadding = ui->cellPadding->value();
    this->tableX_mm = ui->table_x->value();
    this->tableY_mm = ui->table_y->value();

    if(!this->isDefaultCellWidth)
    {
        // 如果是自定义列宽的话
        int length = this->_table->getColumns();
        this->cellWidth.clear();
        for(int i=0; i<length; i++)
        {
            this->cellWidth.append(
                        QTextLength(
                            QTextLength::PercentageLength,
                            1.0 * ui->setTableCellWidth->columnWidth(i) / ui->setTableCellWidth->width() * 100.0));
        }
    }

    if(value = QDialog::Accepted)
    {
        this->_table->setTableWidth(
                    UnitTool::mmToPixel(this->tableWidth_mm));
        this->_table->setCellPadding(
                    UnitTool::mmToPixel(this->cellPadding));
        this->_table->getBlock()->setPos(
                    UnitTool::mmToPixel(this->tableX_mm),
                    UnitTool::mmToPixel(this->tableY_mm));
        this->_table->setTableColor(this->tableColor);
        this->_table->setDefaultStyle(this->isDefaultCellWidth);

        if(!this->isDefaultCellWidth)
        {
            this->_table->setColumnWidth(this->cellWidth);
        }

        this->_table->setTableColor(this->tableColor);
    }

}

TableSettingDialog *TableSettingDialog::getInstance()
{
    if(m_instance == NULL)
    {
        m_instance = new TableSettingDialog();
    }

    return m_instance;
}

void TableSettingDialog::destroyInstance()
{
    delete m_instance;
    m_instance = NULL;
}

TableSettingDialog *TableSettingDialog::getInstance(DocTable *table)
{
    if(m_instance == NULL)
    {
        m_instance = new TableSettingDialog();
    }

    m_instance->init(table);
    return m_instance;
}

void TableSettingDialog::init(DocTable *table)
{
    this->_table = table;
    // 表格整体宽度
    this->tableWidth_mm = UnitTool::pixelToMM(
                table->getBlock()->size().rwidth());

    // 表格整体位置
    this->tableX_mm = UnitTool::pixelToMM(
                table->getBlock()->x());
    this->tableY_mm = UnitTool::pixelToMM(
                table->getBlock()->y());

    // 单元格留白
    this->cellPadding = UnitTool::pixelToMM(
                table->cellPadding());

    // 颜色
    this->tableColor = table->getTableColor();

    // 是否均分
    this->isDefaultCellWidth = table->isDefaultWidth();

    this->cellWidth.clear();
    this->cellWidth = table->getColumnWidth();


    ui->tableWidth->setValue(this->tableWidth_mm);
    ui->table_x->setValue(this->tableX_mm);
    ui->table_y->setValue(this->tableY_mm);

    ui->cellPadding->setValue(this->cellPadding);
    this->cellWidth.clear();
    if(this->isDefaultCellWidth)
    {
        ui->balanceCol->setChecked(true);
        ui->setTableCellWidth->clear();
        int length = table->getColumns();
        ui->setTableCellWidth->setColumnCount(length);
        ui->setTableCellWidth->horizontalHeader()->setResizeMode(
                    QHeaderView::Stretch);

        for(int i =0; i < length; i++)
        {
            this->cellWidth.append(
                        QTextLength(
                            QTextLength::PercentageLength,(100.0 / length)));
        }

    }
    else
    {
        ui->customCol->setChecked(true);
        ui->setTableCellWidth->clear();
        int length = table->getColumns();
        this->cellWidth = table->getColumnWidth();
        ui->setTableCellWidth->setColumnCount(length);
        ui->setTableCellWidth->horizontalHeader()->setResizeMode(
                    QHeaderView::Interactive);
        ui->setTableCellWidth->horizontalHeader()->setStretchLastSection(true);

        for(int i = 0; i < length; i++)
        {
            ui->setTableCellWidth->setHorizontalHeaderItem(
                        i, new QTableWidgetItem(QString::number(i+1)));
            ui->setTableCellWidth->setColumnWidth(
                        i, ui->setTableCellWidth->viewport()->width() *this->cellWidth[i].rawValue() / 100);
        }

    }

}

TableSettingDialog::~TableSettingDialog()
{
    delete ui;
}
