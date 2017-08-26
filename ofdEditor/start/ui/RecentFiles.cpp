#include "RecentFiles.h"
#include "ui_RecentFiles.h"
#include "Settings/RecentFileList.h"
#include "Settings/RecentFileItem.h"
#include "ui/PassageMainWindow.h"

#include <QTableWidgetItem>

RecentFiles* RecentFiles::m_instance = NULL;        // 初始化静态变量

///
/// \brief RecentFiles::getInstance
/// \return
///
RecentFiles *RecentFiles::getInstance()
{
    if(m_instance == NULL)
    {
        m_instance = new RecentFiles();
    }

    m_instance->init();
    return m_instance;
}

///
/// \brief RecentFiles::destroyInstance
///
void RecentFiles::destroyInstance()
{
    m_instance = NULL;
}

///
/// \brief RecentFiles::init
///     每次使用时的初始化函数，用来重新初始化界面
///
void RecentFiles::init()
{
    RecentFileList* recentlist = RecentFileList::getInstance(); // 获得最近文件
    recentlist->load();             // 更新最近文件列表

    this->clear();

    ui->recent_table->horizontalHeader()->setVisible(true);

    int length = recentlist->size();    // 获得有效的列表长度
    for(int i = length-1; i >= 0; i--)
    {
        this->appendItem(recentlist->item(i));
    }

}

RecentFiles::RecentFiles(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RecentFiles)
{
    ui->setupUi(this);
    // 最后一行自动适应宽度
    ui->recent_table->horizontalHeader()->setStretchLastSection(true);
    // 设置每次可以选取一行
    ui->recent_table->setSelectionBehavior(QAbstractItemView::SelectRows);
    // 设置只可以单选
    ui->recent_table->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->recent_table->horizontalHeader()->setVisible(true);

    // 双击打开文件
    this->connect(ui->recent_table, SIGNAL(cellDoubleClicked(int,int)),
                  this, SLOT(selectOpenFile(int,int)));

    // 绑定打开文件信号
    PassageMainWindow* mainWindow = PassageMainWindow::getInstance();
    this->connect(this, SIGNAL(openFile(QString)),
                  mainWindow, SLOT(openFile(QString)));

    // 清空最近记录
    connect(ui->btn_cleanRecent, SIGNAL(released()),
            this, SLOT(cleanRecentRecord()));

}

RecentFiles::~RecentFiles()
{
    delete ui;
}

///
/// \brief RecentFiles::appendItem
///     追加一项
/// \param item
///
void RecentFiles::appendItem(RecentFileItem *item)
{
    int rows = ui->recent_table->rowCount();    // 共有多少行
    ui->recent_table->insertRow(rows);

    QTableWidgetItem* fileName = new QTableWidgetItem();
    QTableWidgetItem* author = new QTableWidgetItem();
    QTableWidgetItem* recentOpenTime = new QTableWidgetItem();
    QTableWidgetItem* recentEditTime = new QTableWidgetItem();
    QTableWidgetItem* filePath = new QTableWidgetItem();

    fileName->setText(item->getFileName());
    author->setText(item->getAuthor());
    recentOpenTime->setText(item->getRecentOpenTime_str());
    recentEditTime->setText(item->getRecentEditTime_str());
    filePath->setText(item->getFilePath());

    ui->recent_table->setItem(rows,0,fileName);
    ui->recent_table->setItem(rows,1,author);
    ui->recent_table->setItem(rows,2,recentOpenTime);
    ui->recent_table->setItem(rows,3,recentEditTime);
    ui->recent_table->setItem(rows,4,filePath);

}

///
/// \brief RecentFiles::clear
///     清空内容
///
void RecentFiles::clear()
{
//    ui->recent_table->clearContents();      // 清空内容
    int length = ui->recent_table->rowCount();
    for(int i = 0; i < length; i++)
    {
        ui->recent_table->removeRow(0);
    }
}

///
/// \brief RecentFiles::selectOpenFile
///     双击打开文件
/// \param row
/// \param column
///
void RecentFiles::selectOpenFile(int row, int column)
{
    QTableWidgetItem* filePath = ui->recent_table->item(row,4);
    QString path = filePath->text();        // 获得路径信息

    emit this->openFile(path);              // 发出打开文件信号

}


///
/// \brief RecentFiles::cleanRecentRecord
///     与按钮绑定
void RecentFiles::cleanRecentRecord()
{
    RecentFileList * list = RecentFileList::getInstance();
    list->clear();
    list->save();
    this->clear();
}
