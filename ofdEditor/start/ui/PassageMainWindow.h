#ifndef PASSAGEMAINWINDOW_H
#define PASSAGEMAINWINDOW_H

#include <QMainWindow>
#include <QResizeEvent>
#include <QLabel>

class QAction;
class QMenu;
class QMdiArea;
class DocPassage;

// 编辑窗口的主界面
class PassageMainWindow
        :public QMainWindow
{
    Q_OBJECT
public:
    explicit PassageMainWindow(QWidget *parent = 0);
    ~PassageMainWindow();

    DocPassage *createMdiChild();

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

    // 编辑
    QAction * undoAction;               // 撤销
    QAction * redoAction;               // 恢复操作
    QAction * copyAction;               // 复制
    QAction * cutAction;               // 剪切
    QAction * pasteAction;              // 粘贴

    // 插入
    QAction * insertTextBlockAction;    // 插入文本框
    QAction * insertImageAction;        // 插入图片
    QAction * insertTableAction;        // 插入表格

    // 格式
    QAction * textFormat;               // 文字格式调整
    QAction * paragraphFormat;          // 段落格式调整
    QAction * imageFormat;              // 图形格式调整
    QAction * tableFormat;              // 表格格式调整

    // 关于
    QAction * aboutQtAction;            // 关于Qt
    QAction * aboutAppAction;           // 关于本工程的介绍
    QAction * helpAciton;               // 帮助文档，如何使用本软件

    QMdiArea * area;            // 多窗口区域


    void init();                // 初始化
    void initAction();          // 初始化QAction
    void connectAction();       // 链接QAction的相应事件
    void disconnectAction();    // 断开事件响应

    DocPassage *activeMdiChild();



};

#endif // PASSAGEMAINWINDOW_H
