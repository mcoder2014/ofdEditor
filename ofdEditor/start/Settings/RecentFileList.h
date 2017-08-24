#ifndef RECENTFILELIST_H
#define RECENTFILELIST_H

#include <QObject>
#include <QVector>

#include "json/json.h"
#include "json/json-forwards.h"
using Json::Reader;
using Json::Value;

class RecentFileItem;

class RecentFileList
        : public QObject
{
    Q_OBJECT

public:
    static RecentFileList* getInstance();   // 获得实例
    static void destoryInstance();          // 销毁实例

    QString getFilePath(){return this->filePath;}
    void qDebugFileList();                  // 输出文件列表
    void deleteEmptyItem();                 // 检查是否有的最近打开文件已经被删除
    void clear(){this->fileList.clear();}   // 嫌麻烦可以清除所有最近记录
    bool isExisting(QString filePath);      // 检查某文件是否存在
    RecentFileItem *item(QString filePath); // 检查文件是否存在,返回该文件
    RecentFileItem *remove(RecentFileItem* item);   // 从队列中清除某一项
    RecentFileItem *remove(QString filePath);       // 从队列中清除某一项
    void addItem(RecentFileItem* item);     // 添加新的项
    int indexOf(RecentFileItem* item);      // item的位置

    void save(){this->exportRecentFileList(this->filePath);}    // 保存到文件
    void load(){this->loadRecentFileList(this->filePath);}      // 从文件中更新

private:
    explicit RecentFileList(QObject *parent = 0);
    static RecentFileList* m_instance;      // 静态实例
    QString filePath;                       // 文件存储路径
    QVector<RecentFileItem*> fileList;      // 最近的文档列表

    void loadRecentFileList(QString filepath);      // 读取文件
    void exportRecentFileList(QString filepath);    // 导出文件

    Value recentFileItemToJson(RecentFileItem* item);   // 将元素转换为value
    RecentFileItem* JsonToRecentFileItem(Value value);    // 将value转换为item

    void setDefault();                      // 设置默认值

signals:

public slots:

};

#endif // RECENTFILELIST_H
