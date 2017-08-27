#ifndef ACTIONCONNECTOR_H
#define ACTIONCONNECTOR_H

#include <QObject>
#include "Doc/DocPage.h"
#include <QMdiSubWindow>


class DocPassage;
class PassageMainWindow;


/*******************************************************
 * 因为如果采用了多窗口模式，那么应用需要首先获取当前的活跃子窗口，
 * 然后在子窗口中进行操作,为了不让MainWindow显得过于累赘，
 * 故分出此类型的功能延伸
 ******************************************************/
class ActionConnector
        :public QObject
{
    Q_OBJECT
public:
    ActionConnector();
    ActionConnector(PassageMainWindow * mainWindow);

    DocPage::Layer getDefaultLayer(){return this->defaultLayer;}
    DocPassage * getActivePassage();        //获取当前操作的文档
    DocPage *getActivePage();              //获取当前操作的页面
public slots:
    void showAttribute();           // 显示文档元信息
    void setMainWindow(PassageMainWindow * mainWindow);     // 设置主窗口
    void addNewPage();                                      // 添加一个新页面
    void addNewBlock(InsertBlockInfo blockInfo);           // 插入一个块
    void addTextBlock();                    // 插入文本框
    void addImageBlock();                   // 插入图片框
    void addTableBlock();                   // 插入表格
    void undo();                            // undo
    void redo();                            // redo
    void setDocPassage(DocPassage* passage);    // 设置passage

    void setDefaultLayer(DocPage::Layer layer){this->defaultLayer = layer;}

    void updateActivePassage(DocPassage* passage);          // 更新当前操作的文章

    void startFindAndReplace();         //打开查找和替换界面

private:
    PassageMainWindow * mainWindow;        // 主窗口
    DocPassage * passage;                  // 当前活跃文档
    DocPage::Layer defaultLayer;           // 当前默认插入的层

    void init();                            // 初始化



};

#endif // ACTIONCONNECTOR_H
