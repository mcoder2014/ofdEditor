#include "ActionConnector.h"
#include "Doc/DocPassage.h"
#include "ui/PassageMainWindow.h"
#include "Doc/DocPage.h"
#include "DataTypes/document/ct_docinfo.h"
#include "Widget/DocInfoDialog.h"

#include <QDebug>
#include <QMetaObject>


ActionConnector::ActionConnector()
{
    init();
}

ActionConnector::ActionConnector(PassageMainWindow *mainWindow)
{
    this->mainWindow = mainWindow;
    init();
}

void ActionConnector::showAttribute()
{
    if(this->passage == NULL)
    {
        qDebug() << "show attribute";
        return;
    }


    CT_DocInfo * docInfo = this->passage->getDocInfo();
    DocInfoDialog* dialog = new DocInfoDialog(docInfo,this->mainWindow);  // 设置窗口
    dialog->exec();                  // 运行
    qDebug() << "show Attribute";

}

void ActionConnector::setMainWindow(PassageMainWindow *mainWindow)
{
    this->mainWindow = mainWindow;
    init();
}


/**
 * @Author Chaoqun
 * @brief  为页面增加一个空白页，默认加在最后一页
 * @param  参数
 * @return 返回值
 * @date   2017/05/15
 */
void ActionConnector::addNewPage()
{

    if(this->passage == NULL)
        return;
    this->passage->appendNewPage();     // 在队尾增加一页

}

/**
 *@Author Chaoqun
 *@brief  增加一个新块
 *@param  参数
 *@return 返回值
 *@date   2017/05/15
 */
void ActionConnector::addNewBlock(InsertBlockInfo& blockInfo)
{
    if(this->passage == NULL)
    {
        return;
    }

//    this->updateActivePassage();    // 更新文章
    DocPage * page = qobject_cast<DocPage *>(this->passage->focusWidget());
    if(page == NULL)
    {
        qDebug() <<"No Focus Widget";
        return;
    }
    else
    {
        qDebug() << "get Focus Sucess";
        page->setBlockFlag(DocPage::draw);      // 进入绘画状态

//        InsertBlockInfo blockInfo(type,layer);  // 设置插入文本框信息
        page->setInsertBlockType(blockInfo);    // 设置插入文本框信息

        page->viewport()->setCursor(Qt::CrossCursor);       // 将鼠标设置为加号形状

    }
}

void ActionConnector::addTextBlock()
{
    if(this->passage == NULL)
        return;
    InsertBlockInfo blockInfo(this->defaultLayer,DocPage::text);  // 设置插入文本框信息
    this->addNewBlock(blockInfo);
}

void ActionConnector::addImageBlock()
{
    if(this->passage == NULL)
        return;
    InsertBlockInfo blockInfo(this->defaultLayer,DocPage::image);  // 设置插入文本框信息
    this->addNewBlock(blockInfo);
}

void ActionConnector::addTableBlock()
{
    if(this->passage == NULL)
        return;
    InsertBlockInfo blockInfo(this->defaultLayer,DocPage::table);  // 设置插入文本框信息
    this->addNewBlock(blockInfo);
}

void ActionConnector::undo()
{
    qDebug() <<"undo";
    this->passage->undoStack->undo();
}

void ActionConnector::redo()
{
    qDebug() <<"redo";
    this->passage->undoStack->redo();
}




/**
 * @Author Chaoqun
 * @brief  更新当前的活跃窗口
 * @param  参数
 * @return 返回值
 * @date   2017/06/23
 */
void ActionConnector::updateActivePassage(QMdiSubWindow *window)
{
    if(window == NULL)
    {
        qDebug() << "updateActivePassage NULL"
                 << "there's no actived window";
        this->passage = NULL;
        return;
    }
    DocPassage* passage = qobject_cast<DocPassage*>(window->widget());  // 获得文档
    if(passage == NULL)
    {
        qDebug()<< "The active MdiWindow may not DocPassage";
        this->passage = NULL;
    }
    else
    {
        this->passage = passage;
    }
}

void ActionConnector::init()
{
    this->defaultLayer = DocPage::Body;
    this->passage = NULL;   // 初始化为空
}
