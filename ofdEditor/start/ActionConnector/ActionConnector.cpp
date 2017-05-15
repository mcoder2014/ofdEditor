#include "ActionConnector.h"
#include "Doc/DocPassage.h"
#include "ui/PassageMainWindow.h"
#include "Doc/DocPage.h"

#include <QDebug>
#include <QMetaObject>


ActionConnector::ActionConnector()
{

}

ActionConnector::ActionConnector(PassageMainWindow *mainWindow)
{
    this->mainWindow = mainWindow;
}

void ActionConnector::setMainWindow(PassageMainWindow *mainWindow)
{
    this->mainWindow = mainWindow;
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
    this->updateActivePassage();        // 更新选择的文章。
    this->passage->appendNewPage();     // 在队尾增加一页

}

/**
 *@Author Chaoqun
 *@brief  增加一个新块
 *@param  参数
 *@return 返回值
 *@date   2017/05/15
 */
void ActionConnector::addNewBlock()
{
    this->updateActivePassage();    // 更新文章
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
        page->setCursor(Qt::CrossCursor);       // 将鼠标设置为加号形状

    }
}


/**
 * @Author Chaoqun
 * @brief  更新，确保当前操作的对象是活动窗口
 * @param  参数
 * @return 返回值
 * @date   2017/05/15
 */
void ActionConnector::updateActivePassage()
{
    DocPassage* temp = this->mainWindow->activeMdiChild();
    if(temp == NULL)
    {
        qDebug()<< " You haven't select any passage. "
                  <<"void ActionConnector::updateActivePassage()";
        this->passage = NULL;
    }
    else
    {
        this->passage = temp;
        // 待CT_DocType配置好了，这里可以输出文档名称
    }
}
