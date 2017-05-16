#ifndef ACTIONCONNECTOR_H
#define ACTIONCONNECTOR_H

#include <QObject>
#include "Doc/DocPage.h"

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

    void setMainWindow(PassageMainWindow * mainWindow);     // 设置主窗口
    void addNewPage();                                      // 添加一个新页面

    void addNewBlock(InsertBlockInfo &blockInfo);           // 插入一个块

    void addTextBlock();                    // 插入文本框
    void addImageBlock();                   // 插入图片框
    void addTableBlock();                   // 插入表格

    void setDefaultLayer(DocPage::Layer layer){this->defaultLayer = layer;}
    DocPage::Layer getDefaultLayer(){return this->defaultLayer;}

private:
    PassageMainWindow * mainWindow;        // 主窗口
    DocPassage * passage;                  // 当前活跃文档
    DocPage::Layer defaultLayer;           // 当前默认插入的层

    void updateActivePassage();             // 更新，确保当前操作的对象是活动的窗口

    void init();                            // 初始化



};

#endif // ACTIONCONNECTOR_H
