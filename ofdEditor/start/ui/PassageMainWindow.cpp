#include "PassageMainWindow.h"

#include <QDesktopWidget>
#include <QApplication>
#include <QAction>
#include <QToolBar>
#include <QMenu>
#include <QWidget>
#include <QMenuBar>
#include <QStatusBar>
#include <QMessageBox>
#include <QColorDialog>
#include <QFileDialog>
#include <QDebug>
#include <QMdiSubWindow>
#include <QMdiArea>
#include <QPalette>


#include "Doc/DocPassage.h"
#include "ActionConnector/ActionConnector.h"

PassageMainWindow::PassageMainWindow(QWidget *parent)
    :QMainWindow(parent)
{
    init();
}

PassageMainWindow::~PassageMainWindow()
{

}

/**
 * @Author Chaoqun
 * @brief  摘要
 * @param  参数
 * @return 返回值
 * @date   2017/05/13
 */
DocPassage *PassageMainWindow::createMdiChild()
{
    DocPassage * child = new DocPassage;
    this->area->addSubWindow(child);
    child->setVisible(true);            // 设置可见
    child->showMaximized();
    return NULL;
}

/**
 * @Author Chaoqun
 * @brief  初始化窗口
 * @param  void
 * @return void
 * @date   2017/05/13
 */
void PassageMainWindow::init()
{
    this->area = new QMdiArea();
    this->setCentralWidget(this->area);

    this->connector = new ActionConnector(this);        // 新建连接器

    initAction();
    connectAction();

    QLabel * status = new QLabel();
    this->statusBar()->addWidget(status);



    this->setMinimumSize(960,720);
    this->setBackgroundRole(QPalette::Text);

}

/**
 * @Author Chaoqun
 * @brief  初始化动作
 * @param  参数
 * @return 返回值
 * @date   2017/05/13
 */
void PassageMainWindow::initAction()
{
    this->newFileAction = new QAction(tr("New File"));      // 新建文件
    this->newFileAction->setStatusTip(tr("Create a new ofd file"));
    this->newFileAction->setShortcut(QKeySequence::New);
    this->newFileAction->setIcon(QIcon(":/icons/source/icons/newFile.png"));

    this->openFileAtcion = new QAction(tr("Open File"));    // 打开文件
    this->openFileAtcion->setStatusTip(tr("Open an existing ofd file"));
    this->openFileAtcion->setShortcut(QKeySequence::Open);
    this->openFileAtcion->setIcon(QIcon(":/icons/source/icons/openFile.png"));

    this->saveAction = new QAction(tr("Save"));             // 保存
    this->saveAction->setStatusTip(tr("Save file"));
    this->saveAction->setShortcut(QKeySequence::Save);
    this->saveAction->setIcon(QIcon(":/icons/source/icons/save.png"));

    this->saveAsAction = new QAction(tr("Save as"));        // 另存为
    this->saveAsAction->setStatusTip(tr("Save as"));
    this->saveAsAction->setShortcut(QKeySequence::SaveAs);
    this->saveAsAction->setIcon(QIcon(":/icons/source/icons/saveAs.png"));

    this->printAction = new QAction(tr("Print"));       // 打印
    this->printAction->setStatusTip(tr("Print your document"));
    this->printAction->setIcon(QIcon(":/icons/source/icons/print.png"));

    this->undoAction = new QAction(tr("Undo"));             // 撤销操作
    this->undoAction->setStatusTip(tr("Undo your last action"));
    this->undoAction->setShortcut(QKeySequence::Undo);
    this->undoAction->setIcon(QIcon(":/icons/source/icons/undo.png"));

    this->redoAction = new QAction(tr("Redo"));             // 重新操作
    this->redoAction->setStatusTip(tr("Redo the action you undo"));
    this->redoAction->setShortcut(QKeySequence::Redo);
    this->redoAction->setIcon(QIcon(":/icons/source/icons/redo.png"));

    this->copyAction = new QAction(tr("Copy"));             // 复制文本
    this->copyAction->setStatusTip(tr("Copy the content you selected"));
    this->copyAction->setShortcut(QKeySequence::Copy);
    this->copyAction->setIcon(QIcon(":/icons/source/icons/copy.png"));

    this->cutAction = new QAction(tr("Cut"));             // 剪切
    this->cutAction->setStatusTip(tr("Cut the content you selected"));
    this->cutAction->setShortcut(QKeySequence::Cut);
    this->cutAction->setIcon(QIcon(":/icons/source/icons/cut.png"));

    this->pasteAction = new QAction(tr("Paste"));           // 粘贴
    this->pasteAction->setStatusTip(tr("Paste your pasteboard content"));
    this->pasteAction->setShortcut(QKeySequence::Paste);
    this->pasteAction->setIcon(QIcon(":/icons/source/icons/paste.png"));

    this->insertNewPageAction = new QAction("Insert New Page");     // 插入新页面
    this->insertNewPageAction->setStatusTip(tr("Insert a new Page into document"));
    this->insertNewPageAction->setIcon(QIcon(":/icons/source/icons/insertNewPage.png"));

    this->insertTextBlockAction = new QAction("Insert TextBlock");  // 插入文本框
    this->insertTextBlockAction->setStatusTip(tr("Insert a new TextBlock"));
    this->insertTextBlockAction->setIcon(QIcon(":/icons/source/icons/insertTextBlock.png"));

    this->insertImageAction = new QAction("Insert Image");           // 插入图片
    this->insertImageAction->setStatusTip(tr("Insert a image"));
    this->insertImageAction->setIcon(QIcon(":/icons/source/icons/insertImage.png"));

    this->insertTableAction = new QAction("Insert Table");          // 插入一个表格
    this->insertTableAction->setStatusTip(tr("Insert a table"));
    this->insertTableAction->setIcon(QIcon(":/icons/source/icons/insertTable.png"));

    this->textFormat = new QAction(tr("Text Format"));      // 文字格式
    this->textFormat->setStatusTip(tr("Set the selected texts' Format"));
    this->textFormat->setIcon(QIcon(":/icons/source/icons/TextFormat.png"));

    this->paragraphFormat = new QAction(tr("Paragraph Format"));    // 段落格式
    this->paragraphFormat->setStatusTip(tr("Set this paragarph format"));
    this->paragraphFormat->setIcon(QIcon(":/icons/source/icons/paragraphFormat.png"));

    this->imageFormat = new QAction(tr("Image Format"));        // 图片格式
    this->imageFormat->setStatusTip(tr("Set the Selected image's format"));
    this->imageFormat->setIcon(QIcon(":/icons/source/icons/ImageFormat.png"));

    this->tableFormat = new QAction(tr("Table Format"));    // 表格格式
    this->tableFormat->setStatusTip(tr("Set the selected table's format"));
    this->tableFormat->setIcon(QIcon(":/icons/source/icons/tableFormat.png"));

    this->aboutQtAction = new QAction(tr("about Qt"));      // 关于QT

    this->aboutAppAction = new QAction(tr("About App"));    // 关于本应用
    this->aboutAppAction->setStatusTip(tr("About this Application"));
    this->aboutAppAction->setIcon(QIcon(":/icons/source/icons/AboutQpp.png"));

    this->helpAciton = new QAction(tr("Help"));
    this->helpAciton->setStatusTip(tr("Show the help Window"));
    this->helpAciton->setIcon(QIcon(":/icons/source/icons/help.png"));

    this->filesMenu = this->menuBar()->addMenu(tr("Files"));
    this->editMenu = this->menuBar()->addMenu(tr("Edit"));
    this->formatMenu = this->menuBar()->addMenu(tr("Format"));
    this->insertMenu = this->menuBar()->addMenu(tr("Insert"));
    this->aboutMenu = this->menuBar()->addMenu(tr("About"));

    this->filesMenu->addAction(this->newFileAction);
    this->filesMenu->addAction(this->openFileAtcion);
    this->filesMenu->addAction(this->saveAction);
    this->filesMenu->addAction(this->saveAsAction);
    this->filesMenu->addAction(this->printAction);

    this->editMenu->addAction(this->undoAction);
    this->editMenu->addAction(this->redoAction);
    this->editMenu->addSeparator();     // 分割线
    this->editMenu->addAction(this->copyAction);
    this->editMenu->addAction(this->cutAction);
    this->editMenu->addAction(this->pasteAction);

    this->formatMenu->addAction(this->textFormat);
    this->formatMenu->addAction(this->paragraphFormat);
    this->formatMenu->addAction(this->imageFormat);
    this->formatMenu->addAction(this->tableFormat);

    this->insertMenu->addAction(this->insertNewPageAction);
    this->insertMenu->addAction(this->insertTextBlockAction);
    this->insertMenu->addAction(this->insertImageAction);
    this->insertMenu->addAction(this->insertTableAction);

    this->aboutMenu->addAction(this->aboutQtAction);
    this->aboutMenu->addAction(this->aboutAppAction);
    this->aboutMenu->addAction(this->helpAciton);

/*---------------------------------------------------------------------*/
    this->file_toolBar = this->addToolBar(tr("File"));
    this->edit_toolBar = this->addToolBar(tr("Edit"));
    this->format_toolBar = this->addToolBar(tr("Format"));
    this->insert_toolBar = this->addToolBar(tr("Insert"));

    this->file_toolBar->addAction(this->newFileAction);
    this->file_toolBar->addAction(this->openFileAtcion);
    this->file_toolBar->addAction(this->saveAction);

    this->edit_toolBar->addAction(this->undoAction);
    this->edit_toolBar->addAction(this->redoAction);

    this->format_toolBar->addAction(this->textFormat);
    this->format_toolBar->addAction(this->paragraphFormat);
    this->format_toolBar->addAction(this->imageFormat);
    this->format_toolBar->addAction(this->tableFormat);

    this->insert_toolBar->addAction(this->insertNewPageAction);
    this->insert_toolBar->addAction(this->insertTextBlockAction);
    this->insert_toolBar->addAction(this->insertImageAction);
    this->insert_toolBar->addAction(this->insertTableAction);

}

/**
 * @Author Chaoqun
 * @brief  为QActions链接响应事件
 * @param  参数
 * @return 返回值
 * @date   2017/05/13
 */
void PassageMainWindow::connectAction()
{
    connect(this->newFileAction, &QAction::triggered,
            this,&PassageMainWindow::createMdiChild);   // 新建窗口

    connect(this->insertNewPageAction, &QAction::triggered,
            this->connector, &ActionConnector::addNewPage);    // 在文章尾部加入新的一页

//    connect(this->insertTextBlockAction, &QAction::triggered,
//            this->connector, &ActionConnector::addNewBlock);    // 插入新块

    connect(this->insertTextBlockAction, &QAction::triggered,
            this->connector, &ActionConnector::addTextBlock);   // 插入文本框

    connect(this->insertImageAction, &QAction::triggered,
            this->connector, &ActionConnector::addImageBlock);  // 插入图片

    connect(this->insertTableAction, &QAction::triggered,
            this->connector, &ActionConnector::addTableBlock);  // 插入表格
}

/**
 * @Author Chaoqun
 * @brief  解除QAction的所有事件响应
 * @param  void
 * @return 返回值
 * @date   2017/05/13
 */
void PassageMainWindow::disconnectAction()
{

}

/**
 * @Author Chaoqun
 * @brief  获取激活的窗口
 * @param  void
 * @return DocPassage *
 * @date   2017/05/13
 */
DocPassage *PassageMainWindow::activeMdiChild()
{
    if (QMdiSubWindow *activeSubWindow = this->area->activeSubWindow())
        return qobject_cast<DocPassage *>(activeSubWindow->widget());
    return 0;
}
