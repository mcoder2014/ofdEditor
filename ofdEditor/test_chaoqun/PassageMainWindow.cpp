#include "PassageMainWindow.h"
#include "Doc/DocPassage.h"
#include "Doc/DocPage.h"

#include <QDebug>

#include <QPalette>

PassageMainWindow::PassageMainWindow(QWidget *parent) : QMainWindow(parent)
{

    this->setBackgroundRole(QPalette::Button);

}

PassageMainWindow::~PassageMainWindow()
{

}

/**
 * @Author Chaoqun
 * @brief  设置页面中的文章
 * @date   2017/05/01
 */
void PassageMainWindow::setPassage(DocPassage *passage)
{
    this->passage = passage;
    this->setCentralWidget(this->passage);

}

/**
 * @Author Chaoqun
 * @brief  摘要
 * @param  DocPage
 * @return void
 * @date   2017/05/01
 */
void PassageMainWindow::addPage(DocPage *page)
{
    this->passage->addPage(page);
}

void PassageMainWindow::resizeEvent(QResizeEvent *event)
{
    if(this->passage != NULL)
    {
        this->passage->resize(event->size().width(),
                              event->size().height());
        qDebug() << "MainWindow:resize";
    }


}
