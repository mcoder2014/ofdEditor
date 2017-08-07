#include "FindAndReplaceDock.h"
#include "ui_FindAndReplaceDock.h"
#include "../model/Doc/DocPassage.h"
#include "../model/Doc/DocPage.h"
#include "../model/Doc/DocLayer.h"
#include "../model/Doc/DocBlock.h"
#include "../model/DOc/DocTextBlock.h"
#include <QMessageBox>
#include <QDebug>
#include <QTextCursor>
#include <QScrollBar>

FindAndReplaceDock* FindAndReplaceDock::m_instance = NULL;  // 初始化静态变量

///
/// \brief FindAndReplaceDock::getInstance
///     获得实例
/// \return
///
FindAndReplaceDock *FindAndReplaceDock::getInstance()
{
    if(m_instance != NULL)
    {
        return m_instance;
    }

    m_instance = new FindAndReplaceDock();
    return m_instance;
}

///
/// \brief FindAndReplaceDock::DestoryInstance
/// 销毁实例 - 暂时交由QT处理
void FindAndReplaceDock::DestoryInstance()
{
    m_instance = NULL;
}

///
/// \brief FindAndReplaceDock::FindAndReplaceDock
/// 默认构造函数
/// \param parent
///
FindAndReplaceDock::FindAndReplaceDock(QWidget *parent) :
    QDockWidget(parent),
    ui(new Ui::FindAndReplaceDock)
{
    ui->setupUi(this);              // 初始化界面
    this->init();                   // 初始化
}

///
/// \brief FindAndReplaceDock::init
///     初始化，主要用于单例的构造函数
void FindAndReplaceDock::init()
{
    //初始化成员
    count = 0;
    ui->CountLineEdit->setText("0");
    ui->CountLineEdit->setEnabled(false);

    ui->ReplaceButton->setEnabled(false);
    ui->ReplaceAllButton->setEnabled(false);

    this->initConnect();            // 初始化信号槽链接
}

///
/// \brief FindAndReplaceDock::initConnect
/// 初始化链接
///
void FindAndReplaceDock::initConnect()
{

    this->connect(ui->FindLineEdit,
                  SIGNAL(textChanged(QString)),
                  this,
                  SLOT(on_FindLineEdit_textChanged(QString)));
//    qDebug() << "Finished constructing FindAndReplaceDock";
    this->connect(ui->CountLineEdit,
                  SIGNAL(textChanged(QString)),
                  this,
                  SLOT(on_CountLineEdit_textChanged(QString)));
    this->connect(ui->FindNextButton,
                  SIGNAL(clicked(bool)),
                  this,
                  SLOT(locateNextPlace()));
    this->connect(ui->FindPreviousButton,
                  SIGNAL(clicked(bool)),
                  this,
                  SLOT(locatePreviousPlace()));
    this->connect(ui->ReplaceButton,
                  SIGNAL(clicked(bool)),
                  this,
                  SLOT(replaceText()));
    this->connect(ui->ReplaceAllButton,
                  SIGNAL(clicked(bool)),
                  this,
                  SLOT(replaceAll()));

}


FindAndReplaceDock::~FindAndReplaceDock()
{
    delete ui;
}

/**
 * @Author Pan
 * @brief  设置当前活动的文档
 * @param  DocPassage *current_passage
 * @return void
 * @date   2017/06/27
 */
void FindAndReplaceDock::setCurrentPassage(DocPassage *current_passage)
{
    passage = current_passage;
}

/**
 * @Author Pan
 * @brief  查找文本框的内容有改变后，就重新开始Find
 * @param  const QString &arg1
 * @return void
 * @date   2017/06/27
 */
void FindAndReplaceDock::on_FindLineEdit_textChanged(const QString &arg1)
{
    blocks_found.clear();
    blocks_flag.clear();
    count = 0;

    if (arg1.length() > 0)
        findAllTargetTextBlock(arg1);
    if (blocks_found.size() == 0)
    {
        ui->ReplaceButton->setEnabled(false);
        ui->ReplaceAllButton->setEnabled(false);
    }
    else {
        ui->ReplaceButton->setEnabled(true);
        ui->ReplaceAllButton->setEnabled(true);
    }
    if (blocks_found.size() == 0)
    {
        ui->CountLineEdit->setText(QString::number(count));
    }
    else
    {
        this->current_block_index = -1;
        this->current_block_pos = -1;
        ui->CountLineEdit->setText(QString::number(count));
        locateNextPlace();
    }
}

/**
 * @Author Pan
 * @brief  找到所有包含目标字符串的DocTextBlock，并放到blocks_found容器中
 * @param  const QString & str
 * @return void
 * @date   2017/06/27
 */
void FindAndReplaceDock::findAllTargetTextBlock(const QString & str)
{
//    qDebug() << "Start finding all target blocks";
    for (int i = 0; i < passage->getPages()->size(); i++)
    {
        DocPage * cur_page = passage->getPages()->operator [](i);
        DocLayer * foreground_layer = cur_page->getForegroundLayer();
        DocLayer * body_layer = cur_page->getBodyLayer();
        DocLayer * background_layer = cur_page->getBackgroundLayer();
        for (int j = 0; j < foreground_layer->getBlocks()->size(); j++)
        {
            DocBlock * cur_block = foreground_layer->getBlocks()->operator [](j);
            if (cur_block->isTextBlock())
            {
                if (cur_block->getTextBlock()
                        ->getContent().contains(str))
                {
                    blocks_found.push_back(cur_block->getTextBlock());
                    blocks_flag.push_back(true);
                    QString sstr = cur_block->getTextBlock()->getContent();
                    int i = -1;
                    do {
                        i = sstr.indexOf(str, i + 1);
                        if (i != -1)
                            count++;
                    } while (i != -1);
                }
            }
        }
//        qDebug() << "Finished processing all foreground layer blocks.";
        for (int j = 0; j < body_layer->getBlocks()->size(); j++)
        {
            DocBlock * cur_block = body_layer->getBlocks()->operator [](j);
            if (cur_block->isTextBlock())
            {
//                qDebug() << "???";
                if (cur_block->getTextBlock()
                        ->getContent().contains(str))
                {
//                    qDebug() << "TextBlock Address = " << cur_block->getTextBlock();
                    blocks_found.push_back(cur_block->getTextBlock());
                    blocks_flag.push_back(true);
//                    qDebug() << "???";
                    int i = -1;
                    do {
                        i = cur_block->getTextBlock()->getContent().indexOf(str, i + 1);
//                        qDebug() << " i = " << i;
                        if (i != -1)
                            count++;
//                        qDebug() << "Count++";
                    } while (i != -1);
//                    qDebug() << "Finished processing a block";
                }
            }
        }
//        qDebug() << "Finished processing all body layer blocks.";
        for (int j = 0; j < background_layer->getBlocks()->size(); j++)
        {
            DocBlock * cur_block = body_layer->getBlocks()->operator [](j);
            if (cur_block->isTextBlock())
            {
                if (cur_block->getTextBlock()
                        ->getContent().contains(str))
                {
                    blocks_found.push_back(cur_block->getTextBlock());
                    blocks_flag.push_back(true);
                    QString sstr = cur_block->getTextBlock()->getContent();
                    int i = -1;
                    do {
                        i = sstr.indexOf(str, i + 1);
                        if (i != -1)
                            count++;
                    } while (i != -1);
                }
            }
        }
    }
    qDebug() << "Finished finding all target blocks";
    qDebug() << blocks_found.size() << "blocks found.";
}

/**
 * @Author Pan
 * @brief  聚焦、定位到当下的查找到的DocTextBlock中的文本
 * @param
 * @return void
 * @date   2017/06/27
 */
void FindAndReplaceDock::focusOnFoundPart()
{
    DocTextBlock * current_block = blocks_found[current_block_index];
    current_block->setFocus();
    QTextCursor cursor = current_block->textCursor();
    cursor.setPosition(current_block_pos);
    cursor.movePosition(QTextCursor::NextCharacter,
                        QTextCursor::KeepAnchor,
                        ui->FindLineEdit->text().length());
    current_block->setTextCursor(cursor);
    QScrollBar * scroll_bar = passage->verticalScrollBar();

    DocPassage * current_passage = current_block->getPassage();
    DocPage * current_page = current_block->getPage();
    int accumulated_height = 0;
    for (int i = 0; i < current_passage->getPages()->size(); i++)
    {
        if (current_passage->getPages()->operator [](i) == current_page)
            break;
        else
            accumulated_height +=
                    50 + current_passage->getPages()->operator [](i)->height();
    }
    scroll_bar->setSliderPosition(accumulated_height + current_block->pos().ry());
}

/**
 * @Author Pan
 * @brief  辅助函数，通过blocks_flag找到下一个包含目标字符串的块
 * @param  int cur_index
 * @return int
 * @date   2017/06/27
 */
int FindAndReplaceDock::findNextAvaibleBlockIndex(int cur_index)
{
    int i = cur_index;
    while (((++i) %= blocks_flag.size()) != cur_index)
    {
        if (blocks_flag[i] == true)
            return i;
    }
    return -1;
}

/**
 * @Author Pan
 * @brief  定位到上一处位置
 * @param
 * @return void
 * @date   2017/06/27
 */
void FindAndReplaceDock::locatePreviousPlace()
{
    //To be implemented
}

/**
 * @Author Pan
 * @brief  定位到下一处位置
 * @param
 * @return void
 * @date   2017/06/27
 */
void FindAndReplaceDock::locateNextPlace()
{
    if (blocks_found.size() > 0) {
        qDebug() << "locateNextPlace() called.";
    //    qDebug() << "Current_block_index = " << current_block_index;
    //    qDebug() << "Current_block_pos = " << current_block_pos;
        DocTextBlock * cur_text_block;
        QString str = ui->FindLineEdit->text();
        QString sstr;
        if (current_block_index == -1 && current_block_pos == -1) {
            current_block_index = 0;
            cur_text_block = blocks_found[0];
            sstr = cur_text_block->getContent();
            current_block_pos = sstr.indexOf(str, 0);
//            qDebug() << "In init str = " << str << " sstr = " << sstr;
        }
        else
        {
            qDebug() << "Output block_flags: ";
            for (int i = 0; i < blocks_flag.size(); i++)
                qDebug() << "blocks_flag at " << i << " = " << blocks_flag[i];
            qDebug() << "Current_block_index = " << current_block_index;
            qDebug() << "Current_block_pos = " << current_block_pos;
            qDebug() << "Next place position = " << sstr.indexOf(str, current_block_pos + 1);
            qDebug() << "str = " << str << " sstr = " << sstr;
            cur_text_block = blocks_found[current_block_index];
            sstr = cur_text_block->getContent();
            if (sstr.indexOf(str, current_block_pos + 1) != -1) {
                current_block_pos = sstr.indexOf(str, current_block_pos + 1);
            }
            else {
                qDebug() << "Current_block_index = " << current_block_index;
                qDebug() << "Next available Index = "
                         << findNextAvaibleBlockIndex(current_block_index);
                if ((current_block_index =
                        findNextAvaibleBlockIndex(current_block_index)) != -1) {

                    cur_text_block = blocks_found[current_block_index];
                    sstr = cur_text_block->getContent();
                    current_block_pos = sstr.indexOf(str, 0);
                } else {
                    QMessageBox::information(passage, "Message", "No text found.", "OK");
                    return;
                }
            }
        }
        focusOnFoundPart();
    } else {
        QMessageBox::information(passage, "Message", "No text found.", "OK");
        return;
    }
}

/**
 * @Author Pan
 * @brief  用ReplaceWithLineEdit中的文本替换当前选中的文本
 * @param
 * @return void
 * @date   2017/06/27
 */
void FindAndReplaceDock::replaceText()
{
    if (blocks_found.size() > 0)
    {
        if (current_block_index >= 0 && current_block_pos >= 0)
        {
            DocTextBlock * cur_block = blocks_found[current_block_index];
            QTextCursor cursor = cur_block->textCursor();
            cursor.removeSelectedText();
            cursor.insertText(ui->ReplaceWithLineEdit->text());
            if (!cur_block->getContent().contains(ui->FindLineEdit->text()))
                blocks_flag[current_block_index] = false;
            locateNextPlace();
        }
    }
    else return;
}

/**
 * @Author Pan
 * @brief  替换所有查找到的文本
 * @param
 * @return void
 * @date   2017/06/27
 */
void FindAndReplaceDock::replaceAll()
{
   while (findNextAvaibleBlockIndex(0) >= 0) {
       replaceText();
   }
}

/**
 * @Author Pan
 * @brief  按钮的效果的槽函数
 * @param  const QString &arg1
 * @return void
 * @date   2017/06/27
 */
void FindAndReplaceDock::on_CountLineEdit_textChanged(const QString &arg1)
{
    if (arg1.toInt() == 0)
    {
        ui->FindPreviousButton->setEnabled(false);
        ui->FindNextButton->setEnabled(false);
    }
    else
    {
        ui->FindPreviousButton->setEnabled(true);
        ui->FindNextButton->setEnabled(true);
    }
}



