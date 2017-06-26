#ifndef PASSAGEMAINWINDOW_H
#define PASSAGEMAINWINDOW_H

#include <QMainWindow>
#include <QResizeEvent>
#include <QLabel>
#include <QFontComboBox>        // 字体框
#include <QComboBox>            // 选择框

class QAction;
class QMenu;
class QMdiArea;
class DocPassage;
class DocTextBlock;
class QTextCharFormat;
class QTextBlockFormat;
class ActionConnector;      // 函数功能的中间件
class DocImageBlock;

// 编辑窗口的主界面
class PassageMainWindow
        :public QMainWindow
{
    Q_OBJECT
public:
    explicit PassageMainWindow(QWidget *parent = 0);
    ~PassageMainWindow();
public slots:
    DocPassage *createMdiChild();       // 创建一个新文档
    DocPassage *activeMdiChild();       // 获取活动的窗口
    DocPassage *addDocPassage(DocPassage * passage);

private:

    // 菜单栏
    QMenu * filesMenu;          // 文件
    QMenu * editMenu;           // 编辑
    QMenu * formatMenu;         // 格式
    QMenu * insertMenu;         // 插入
    QMenu * aboutMenu;          // 关于

    QToolBar* file_toolBar;      // 文件
    QToolBar* edit_toolBar;      // 编辑
    QToolBar* format_toolBar;    // 格式
    QToolBar* insert_toolBar;    // 插入


    // QAction
    // 文件
    QAction * newFileAction;            // 新建文件
    QAction * openFileAtcion;           // 打开文件
    QAction * saveAction;               // 保存
    QAction * saveAsAction;             // 另存为
    QAction * printAction;              // 打印
    QAction * attributeAction;          // 文档属性

    // 编辑
    QAction * undoAction;               // 撤销
    QAction * redoAction;               // 恢复操作
    QAction * copyAction;               // 复制
    QAction * cutAction;               // 剪切
    QAction * pasteAction;              // 粘贴

    // 插入
    QAction * insertNewPageAction;      // 插入新页面
    QAction * insertTextBlockAction;    // 插入文本框
    QAction * insertImageAction;        // 插入图片
    QAction * insertTableAction;        // 插入表格

    // 格式
    QAction * pageFormat;               // 页面格式调整
    QAction * textFormat;               // 文字格式调整
    QAction * paragraphFormat;          // 段落格式调整
    QAction * imageFormat;              // 图形格式调整
    QAction * tableFormat;              // 表格格式调整

    // 关于
    QAction * aboutQtAction;            // 关于Qt
    QAction * aboutAppAction;           // 关于本工程的介绍
    QAction * helpAciton;               // 帮助文档，如何使用本软件

    QMdiArea * area;            // 多窗口区域
    QVector<DocPassage* >passages;      // 存储所有的passage

    ActionConnector* connector; // 功能连接中间件


    void init();                // 初始化
    void initAction();          // 初始化QAction
    void connectAction();       // 链接QAction的相应事件
    void disconnectAction();    // 断开事件响应

    DocTextBlock *textBlock;    // 文字块
    QTextCharFormat* _currentCharFormat;    // 当前字符格式
    QTextBlockFormat* _currentBlockFormat;  // 当前块格式

    DocImageBlock *imageBlock;  //图片块



private slots:
    void openFile();            // 打开新文件
    void fontDialog();          // 打开字体框
    void paragraphDialog();     // 打开段落框
    void imageDialog();         // 打开图片框
    void pageDialog();          // 打开页面框

    void acceptTextBlock(DocTextBlock* textBlock);              // 接受当前处理的文字块的更新
    void acceptTextBlockFormat(QTextBlockFormat& blockFormat);  // 接受当前处理的块格式
    void acceptTextCharFormat(QTextCharFormat& charFormat);     // 接受当前处理的字符格式
    void acceptImageBlock(DocImageBlock * imageBlock);          //接受当前处理的图片块
};

#endif // PASSAGEMAINWINDOW_H
