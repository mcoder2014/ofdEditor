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
#include <QTextBlockFormat>
#include <QTextCharFormat>


#include "Doc/DocPassage.h"
#include "ActionConnector/ActionConnector.h"
#include "Loaders/ZipTool.h"                // 压缩文件工具
#include "ofd_parser.h"
#include "DataTypes/document/ofd.h"
#include "Convert/OFD_DocConvertor.h"       // OFD 转 Doc 工具
#include "Doc/DocPage.h"
#include "Doc/DocTextBlock.h"

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
    qDebug()<<"execute";

    DocPassage * child = new DocPassage(this);
    child->addPage(new DocPage());      // 添加一个空白页

    this->addDocPassage(child);         // 加入到本视区

    return child;
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
    this->newFileAction = new QAction(tr("New File"),NULL);      // 新建文件
    this->newFileAction->setStatusTip(tr("Create a new ofd file"));
    this->newFileAction->setShortcut(QKeySequence::New);
    this->newFileAction->setIcon(QIcon(":/icons/source/icons/newFile.png"));

    this->openFileAtcion = new QAction(tr("Open File"),NULL);    // 打开文件
    this->openFileAtcion->setStatusTip(tr("Open an existing ofd file"));
    this->openFileAtcion->setShortcut(QKeySequence::Open);
    this->openFileAtcion->setIcon(QIcon(":/icons/source/icons/openFile.png"));

    this->saveAction = new QAction(tr("Save"),NULL);             // 保存
    this->saveAction->setStatusTip(tr("Save file"));
    this->saveAction->setShortcut(QKeySequence::Save);
    this->saveAction->setIcon(QIcon(":/icons/source/icons/save.png"));

    this->saveAsAction = new QAction(tr("Save as"),NULL);        // 另存为
    this->saveAsAction->setStatusTip(tr("Save as"));
    this->saveAsAction->setShortcut(QKeySequence::SaveAs);
    this->saveAsAction->setIcon(QIcon(":/icons/source/icons/saveAs.png"));

    this->printAction = new QAction(tr("Print"),NULL);       // 打印
    this->printAction->setStatusTip(tr("Print your document"));
    this->printAction->setIcon(QIcon(":/icons/source/icons/print.png"));

    this->attributeAction = new QAction(tr("Attribute"),NULL);      // 文档属性
    this->attributeAction->setStatusTip(tr("Show you the attribute of the actived passage"));

    this->undoAction = new QAction(tr("Undo"),NULL);             // 撤销操作
    this->undoAction->setStatusTip(tr("Undo your last action"));
    this->undoAction->setShortcut(QKeySequence::Undo);
    this->undoAction->setIcon(QIcon(":/icons/source/icons/undo.png"));

    this->redoAction = new QAction(tr("Redo"),NULL);             // 重新操作
    this->redoAction->setStatusTip(tr("Redo the action you undo"));
    this->redoAction->setShortcut(QKeySequence::Redo);
    this->redoAction->setIcon(QIcon(":/icons/source/icons/redo.png"));

    this->copyAction = new QAction(tr("Copy"),NULL);             // 复制文本
    this->copyAction->setStatusTip(tr("Copy the content you selected"));
    this->copyAction->setShortcut(QKeySequence::Copy);
    this->copyAction->setIcon(QIcon(":/icons/source/icons/copy.png"));

    this->cutAction = new QAction(tr("Cut"),NULL);             // 剪切
    this->cutAction->setStatusTip(tr("Cut the content you selected"));
    this->cutAction->setShortcut(QKeySequence::Cut);
    this->cutAction->setIcon(QIcon(":/icons/source/icons/cut.png"));

    this->pasteAction = new QAction(tr("Paste"),NULL);           // 粘贴
    this->pasteAction->setStatusTip(tr("Paste your pasteboard content"));
    this->pasteAction->setShortcut(QKeySequence::Paste);
    this->pasteAction->setIcon(QIcon(":/icons/source/icons/paste.png"));

    this->insertNewPageAction = new QAction(tr("Insert New Page"),NULL);     // 插入新页面
    this->insertNewPageAction->setStatusTip(tr("Insert a new Page into document"));
    this->insertNewPageAction->setIcon(QIcon(":/icons/source/icons/insertNewPage.png"));

    this->insertTextBlockAction = new QAction(tr("Insert TextBlock"),NULL);  // 插入文本框
    this->insertTextBlockAction->setStatusTip(tr("Insert a new TextBlock"));
    this->insertTextBlockAction->setIcon(QIcon(":/icons/source/icons/insertTextBlock.png"));

    this->insertImageAction = new QAction(tr("Insert Image"),NULL);           // 插入图片
    this->insertImageAction->setStatusTip(tr("Insert a image"));
    this->insertImageAction->setIcon(QIcon(":/icons/source/icons/insertImage.png"));

    this->insertTableAction = new QAction(tr("Insert Table"),NULL);          // 插入一个表格
    this->insertTableAction->setStatusTip(tr("Insert a table"));
    this->insertTableAction->setIcon(QIcon(":/icons/source/icons/insertTable.png"));

    this->textFormat = new QAction(tr("Text Format"),NULL);      // 文字格式
    this->textFormat->setStatusTip(tr("Set the selected texts' Format"));
    this->textFormat->setIcon(QIcon(":/icons/source/icons/TextFormat.png"));

    this->paragraphFormat = new QAction(tr("Paragraph Format"),NULL);    // 段落格式
    this->paragraphFormat->setStatusTip(tr("Set this paragarph format"));
    this->paragraphFormat->setIcon(QIcon(":/icons/source/icons/paragraphFormat.png"));

    this->imageFormat = new QAction(tr("Image Format"),NULL);        // 图片格式
    this->imageFormat->setStatusTip(tr("Set the Selected image's format"));
    this->imageFormat->setIcon(QIcon(":/icons/source/icons/ImageFormat.png"));

    this->tableFormat = new QAction(tr("Table Format"),NULL);    // 表格格式
    this->tableFormat->setStatusTip(tr("Set the selected table's format"));
    this->tableFormat->setIcon(QIcon(":/icons/source/icons/tableFormat.png"));

    this->aboutQtAction = new QAction(tr("about Qt"),NULL);      // 关于QT

    this->aboutAppAction = new QAction(tr("About App"),NULL);    // 关于本应用
    this->aboutAppAction->setStatusTip(tr("About this Application"));
    this->aboutAppAction->setIcon(QIcon(":/icons/source/icons/AboutQpp.png"));

    this->helpAciton = new QAction(tr("Help"),NULL);
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
    this->filesMenu->addAction(this->attributeAction);

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
    connect(this->newFileAction, SIGNAL(triggered(bool)),
            this,SLOT(createMdiChild()));   // 新建窗口

    connect(this->openFileAtcion, SIGNAL(triggered(bool)),
            this, SLOT(openFile()));  //打开文件

    connect(this->attributeAction, SIGNAL(triggered(bool)),
            this->connector, SLOT(showAttribute()));        // 显示文档属性

    connect(this->insertNewPageAction, SIGNAL(triggered(bool)),
            this->connector, SLOT(addNewPage()));    // 在文章尾部加入新的一页

//    connect(this->insertTextBlockAction, &QAction::triggered,
//            this->connector, &ActionConnector::addNewBlock);    // 插入新块

    //undo operation
    connect(this->undoAction,SIGNAL(triggered(bool)),this->connector,SLOT(undo()));

    //redo operation
    connect(this->redoAction,SIGNAL(triggered(bool)),this->connector,SLOT(redo()));

    connect(this->insertTextBlockAction, SIGNAL(triggered()),
            this->connector, SLOT(addTextBlock()));   // 插入文本框

    connect(this->insertImageAction, SIGNAL(triggered()),
            this->connector, SLOT(addImageBlock()));  // 插入图片

    connect(this->insertTableAction, SIGNAL(triggered()),
            this->connector, SLOT(addTableBlock()));  // 插入表格

    connect(this->textFormat,SIGNAL(triggered(bool)),
            this,SLOT(fontDialog()));                 // 修改字体

    connect(this->paragraphFormat,SIGNAL(triggered(bool)),
            this,SLOT(paragraphDialog()));            // 修改段落

    connect(this->area, SIGNAL(subWindowActivated(QMdiSubWindow*)),
            this->connector, SLOT(updateActivePassage(QMdiSubWindow*)));    // 检测ActivePassage更新
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
 * @brief  打开 *.ofd 文件
 * @param  void
 * @return void
 * @date   2017/05/22
 */
void PassageMainWindow::openFile()
{
    QFileDialog * fileDialog = new QFileDialog(this);           // 新建一个QFileDialog
//    fileDialog->setWindowIcon(QIcon(":/icon/source/open.png")); // 设置打开文件图标
    fileDialog->setAcceptMode(QFileDialog::AcceptOpen);         // 设置对话框为打开文件类型
    fileDialog->setFileMode(QFileDialog::ExistingFile);         // 设置文件对话框能够存在的文件
    fileDialog->setViewMode(QFileDialog::Detail);               // 文件以细节形式显示出来
    fileDialog->setNameFilter(tr("JSON files(*.ofd)"));            // 设置文件过滤器
    fileDialog->setWindowTitle(tr("Choose an ofd document file!"));

    if(fileDialog->exec() == QDialog::Accepted)
    {
        QString path = fileDialog->selectedFiles()[0];      // 用户选择文件名
        qDebug() << path;

        ZipTool zipTool;
        QString tempPath = zipTool.FilePathToFloderPath(path);
        qDebug() << "Temp path is :" << tempPath;

        ZipTool::extractDir(path,tempPath);     // 解压到临时文件夹

        // 解读文件
//        OFDParser ofdParser("C:\\Users\\User\\AppData\\Local\\Temp\\%表格.ofd%\\OFD.xml");
        OFDParser ofdParser(tempPath + "/OFD.xml");      // 新建临时路径
//        OFDParser ofdParser("C:/Users/User/Desktop/表格/OFD.xml");
        OFD* data = ofdParser.getData();    // 读取出OFD文件
        qDebug()<< "ofd file open";
        OFD_DocConvertor convert;
        DocPassage* passage = convert.ofd_to_doc(data);

        this->addDocPassage(passage);       // 添加文章

    }
}

/**
 * @Author Chaoqun
 * @brief  打开字体框
 * @param  void
 * @return void
 * @date   2017/06/23
 */
void PassageMainWindow::fontDialog()
{
    this->textBlock->customFontDialog();    // 用自定义窗口修改字体
}

/**
 * @Author Chaoqun
 * @brief  用自定义窗口修改段落格式
 * @param  void
 * @return void
 * @date   2017/06/23
 */
void PassageMainWindow::paragraphDialog()
{
    this->textBlock->textParagraph();       // 用自定义段落窗口修改段落
}

/**
 * @Author Chaoqun
 * @brief  接受当前处理的文字块的更新
 * @param  DocTextBlock *textBlock
 * @return void
 * @date   2017/06/23
 */
void PassageMainWindow::acceptTextBlock(DocTextBlock *textBlock)
{
    this->textBlock = textBlock;        // 修改引用
}

/**
 * @Author Chaoqun
 * @brief  接受当前处理的块格式
 * @param  QTextBlockFormat &blockFormat
 * @return void
 * @date   2017/06/23
 */
void PassageMainWindow::acceptTextBlockFormat(QTextBlockFormat &blockFormat)
{
    this->_currentBlockFormat = &blockFormat;   // 留下引用

    // 更新界面显示
}

/**
 * @Author Chaoqun
 * @brief  接受字符格式
 * @param  QTextCharFormat &charFormat
 * @return void
 * @date   2017/06/23
 */
void PassageMainWindow::acceptTextCharFormat(QTextCharFormat &charFormat)
{
    this->_currentCharFormat  = &charFormat;    // 留下引用

    // 更新界面显示
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

/**
 * @Author Chaoqun
 * @brief  添加一个文章
 * @param  DocPassage *passage
 * @return DocPassage *
 * @date   2017/05/23
 */
DocPassage *PassageMainWindow::addDocPassage(DocPassage *passage)
{
    if(passage == NULL)     // 如果参数为NULL 不执行并返回NULL
    {
        qDebug() << "DocPassage Pointer is NULL.";
        return NULL;
    }

    this->area->addSubWindow(passage);          // 插入子窗口
    this->connector->setDocPassage(passage);    // 设置引用

    passage->setVisible(true);            // 设置可见
    passage->showMaximized();

    // 处理变更的blockFormat
    this->connect(passage,SIGNAL(signals_currentBlockFormatChanged(QTextBlockFormat&)),
                  this,SLOT(acceptTextBlockFormat(QTextBlockFormat&)));
    // 处理变更的charFormat
    this->connect(passage,SIGNAL(signals_currentCharFormatChanged(QTextCharFormat&)),
                  this,SLOT(acceptTextCharFormat(QTextCharFormat&)));
    // 处理变更的textBlock
    this->connect(passage,SIGNAL(signals_currentTextBlock(DocTextBlock*)),
                  this,SLOT(acceptTextBlock(DocTextBlock*)));

    return passage;
}

