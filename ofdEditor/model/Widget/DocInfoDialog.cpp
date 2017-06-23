#include "DocInfoDialog.h"
#include "ui_DocInfoDialog.h"

#include "DataTypes/document/ct_docinfo.h"
#include <QUuid>
#include <QDebug>

DocInfoDialog::DocInfoDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DocInfoDialog)
{
    ui->setupUi(this);
}

DocInfoDialog::DocInfoDialog(CT_DocInfo *docInfo,QWidget *parent = 0):
    QDialog(parent), ui(new Ui::DocInfoDialog)
{
    ui->setupUi(this);
    this->docInfo = docInfo;
    this->init();
    this->initUi();
}

DocInfoDialog::~DocInfoDialog()
{
    delete ui;
}

/**
 * @Author Chaoqun
 * @brief  初始化
 * @param  void
 * @return void
 * @date   2017/06/23
 */
void DocInfoDialog::init()
{
    connect(ui->author, SIGNAL(textChanged(QString)),
            this,SLOT(editAuthor(QString)));            // 作者
    connect(ui->title, SIGNAL(textChanged(QString)),
            this, SLOT(editTitle(QString)));            // 题目
    connect(ui->subject, SIGNAL(textChanged(QString)),
            this, SLOT(editSubject(QString)));          // 主题
    connect(ui->abstract_2, SIGNAL(textChanged(QString)),
            this, SLOT(editAbstract(QString)));         // 摘要
    connect(ui->fileType, SIGNAL(textChanged(QString)),
            this,SLOT(editFileType(QString)));          // 文档类型
    connect(ui->fileCover, SIGNAL(textChanged(QString)),
            this,SLOT(editFileCover(QString)));         // 文档封面
    connect(ui->btn_uuid, SIGNAL(pressed()),
            this, SLOT(resetDocId()));                   // 重置UUID

}

/**
 * @Author Chaoqun
 * @brief  初始化UI界面
 * @param  void
 * @return void
 * @date   2017/06/23
 */
void DocInfoDialog::initUi()
{
    // 文件名

    // 文件id
    ui->docId->setText(this->docInfo->getDocID());

    // 标题
    ui->title->setText(this->docInfo->getTitle());

    // 作者
    ui->author->setText(this->docInfo->getAuthor());

    // 主题
    ui->subject->setText(this->docInfo->getSubject());

    // 摘要
    ui->abstract_2->setText(this->docInfo->getAbstract());

    // 文档类型
    ui->fileType->setText(this->docInfo->getDocUsage());

    // 文档封面
    ui->fileCover->setText(this->docInfo->getCover());

    // 创建日期
    ui->creationDate->setText(this->docInfo->getCreationDate());

    // 最后修改时间
    ui->modDate->setText(this->docInfo->getModDate());

    // 创建者应用程序
    ui->createApp->setText(this->docInfo->getCreator());

    // 应用程序版本
    ui->creatorVersion->setText(this->docInfo->getCreatorVersion());

}


void DocInfoDialog::editTitle(const QString &text)
{
    this->docInfo->setTitle(text);  // 设置标题
}

void DocInfoDialog::editAuthor(const QString &text)
{
    this->docInfo->setAuthor(text);
}

void DocInfoDialog::editSubject(const QString &text)
{
    this->docInfo->setSubject(text);
}

void DocInfoDialog::editAbstract(const QString &text)
{
    this->docInfo->setAbstract(text);
}

void DocInfoDialog::editFileType(const QString &text)
{
    this->docInfo->setDocUsage(text);
}

void DocInfoDialog::editFileCover(const QString &text)
{
    this->docInfo->setCover(text);
}

void DocInfoDialog::resetDocId()
{
    QUuid uuid = QUuid::createUuid();   // 创建一个新的uuid
    QString docId = uuid.toString();
    qDebug() << "new uuid: " << docId;

    this->docInfo->setDocID(docId);
    ui->docId->setText(docId);
}
