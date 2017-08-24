#include "RecentFileList.h"
#include "RecentFileItem.h"


#include "jsoncpp.cpp"

#include <QFile>
#include <QDebug>
#include <QTextStream>
#include <string>

using std::string;


RecentFileList* RecentFileList::m_instance = NULL;

///
/// \brief RecentFileList::getInstance
///     获得实例
/// \return
///
RecentFileList *RecentFileList::getInstance()
{
    if(m_instance == NULL)
    {
        m_instance = new RecentFileList();
    }

    return m_instance;
}

///
/// \brief RecentFileList::qDebugFileList
///     通过qDebug方式输出文件列表
///
void RecentFileList::qDebugFileList()
{
    for(int i = 0 ; i < this->fileList.size(); i++)
    {
        this->fileList[i]->print();
    }
}

///
/// \brief RecentFileList::RecentFileList
///     默认初始化函数
/// \param parent
///
RecentFileList::RecentFileList(QObject *parent)
    : QObject(parent)
{
    this->filePath = "recent_log.json";
    this->loadRecentFileList(this->filePath);
}

///
/// \brief RecentFileList::loadRecentFileList
/// \param filepath 完整路径
///
void RecentFileList::loadRecentFileList(QString filepath)
{
    QFile qfile;
    qfile.setFileName(filepath);    // 设置文件路径
    this->fileList.clear();

    // 如果文件不存在
    if(!qfile.exists())
    {
        qDebug() << "Recent List is not exists.";

        this->setDefault();
        this->exportRecentFileList(filepath);
        return;
    }

    // 打开文件
    if(!qfile.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug() << "Recent list file opening failed";
        this->setDefault();
        this->exportRecentFileList(filepath);
        return;
    }

    QTextStream textInput(&qfile);      // 设置文件流
    QString qstr = textInput.readAll(); // 读取文件的全部内容
    string str = qstr.toStdString();    // 转换为标准字符串

    Reader reader;      // json 解析器
    Value value;        // 值

    if(!reader.parse(str,value))
    {
        // 如果解析失败
        this->setDefault();
        this->exportRecentFileList(filepath);
        return;
    }

    if(value.empty())
    {
        this->setDefault();
        return;
    }
    Value files = value["files"];
    for(int i = 0; i < files.size(); i++)
    {
        RecentFileItem* item = this->JsonToRecentFileItem(files[i]);
        if(item != NULL)
            this->fileList.append(item);
    }

    this->deleteEmptyItem();            // 清除已经失效的项

}

///
/// \brief RecentFileList::exportRecentFileList
///     将文件导出
/// \param filepath
///
void RecentFileList::exportRecentFileList(QString filepath)
{
    QFile qfile;                        // qt 文件支持
    qfile.setFileName(filepath);        // 设置文件路径

    // 检查文件是否可用
    if(!qfile.open(QIODevice::ReadWrite
                   | QIODevice::Text
                   | QIODevice::Truncate))
    {
        qDebug() << "Program open file failed.";
        return;
    }

    this->deleteEmptyItem();            // 删除已经失效的最近的文件

    QTextStream textOutput(&qfile);     // 用来写出文件
    Value root;         // 根节点
    Value files;        // 文件数组节点

    for(int i = 0; i < this->fileList.size(); i++)
    {
        Value item = this->recentFileItemToJson(
                    this->fileList.operator [](i));
        if(item.empty())
            continue;
        files.append(item);
    }

    root["files"] = files;      // 保存根节点
    textOutput << QString::fromStdString(
                      root.toStyledString())
               << endl;         // 写出文件
    textOutput.flush();
    qfile.close();
}

///
/// \brief RecentFileList::recentFileItemToJson
/// \param item
/// \return
///
Json::Value RecentFileList::recentFileItemToJson(RecentFileItem *item)
{
    Value value;
    if(item == NULL)
    {
        return value;
    }

    value["fileName"] = item->getFileName().toStdString();
    value["author"] = item->getAuthor().toStdString();
    value["recentOpenTime"] = item->getRecentOpenTime_str().toStdString();
    value["recentEditTime"] = item->getRecentEditTime_str().toStdString();
    value["filePath"] = item->getFilePath().toStdString();

    return value;

}

///
/// \brief RecentFileList::JsonToRecentFileItem
/// \return
///
RecentFileItem *RecentFileList::JsonToRecentFileItem(Json::Value value)
{
    if(value.empty())
    {
        return NULL;
    }

    RecentFileItem* item = new RecentFileItem();
    item->setFileName(
                QString::fromStdString(
                    value["fileName"].asString()));
    item->setAuthor(
                QString::fromStdString(
                    value["author"].asString()));
    item->setRecentOpenTime(
                QString::fromStdString(
                    value["recentOpenTime"].asString()));
    item->setRecentEditTime(
                QString::fromStdString(
                    value["recentEditTime"].asString()));
    item->setFilePath(
                QString::fromStdString(
                    value["filePath"].asString()));

    return item;

}

void RecentFileList::deleteEmptyItem()
{
    // 检查，然后将那些项先输出后删除
    QVector<RecentFileItem* >::Iterator iter;
    iter = this->fileList.begin();

    qDebug() << "Empty item begin:";
    while(iter != this->fileList.end())
    {
        RecentFileItem * item = *iter;
        if(item->isExist() == false)
        {
            item->print();
            iter = this->fileList.erase(iter);
        }
        else
        {
            iter ++;
        }
    }
    qDebug() << "Empty item end.";
}

bool RecentFileList::isExisting(QString filePath)
{
    for(int i = 0; i < this->fileList.size(); i++)
    {
        if(this->fileList[i]->getFilePath() == filePath)
            return true;
    }

    return false;
}

RecentFileItem *RecentFileList::item(QString filePath)
{
    for(int i = 0; i < this->fileList.size(); i++)
    {
        if(this->fileList[i]->getFilePath() == filePath)
            return this->fileList[i];
    }

    return NULL;
}

RecentFileItem *RecentFileList::remove(RecentFileItem *item)
{
    RecentFileItem* oldItem = this->item(item->getFilePath());
    this->fileList.remove(
                this->fileList.indexOf(oldItem));
    return oldItem;
}

RecentFileItem *RecentFileList::remove(QString filePath)
{
    RecentFileItem *oldItem = this->item(filePath);
    this->fileList.remove(
                this->fileList.indexOf(oldItem));
    return oldItem;
}

///
/// \brief RecentFileList::addItem
///     加入一项到队列中，加入后自动保存
/// \param item
///
void RecentFileList::addItem(RecentFileItem *item)
{

    if(this->isExisting(item->getFilePath()))
    {
        // 如果已经存在同样的项，先删除，后更新再插入
        this->remove(item);
        this->fileList.append(item);

    }
    else
    {
        // 如果是新项，则直接插入
        this->fileList.append(item);
    }
    this->exportRecentFileList(this->filePath); // 自动保存
}

int RecentFileList::indexOf(RecentFileItem *item)
{
    for(int i = 0; i < this->fileList.size(); i++)
    {
        if(this->fileList[i]->getFilePath() == filePath)
            return i;
    }

    return -1;

}

///
/// \brief RecentFileList::setDefault
///     当没有文件时，可以用来生成默认文件
///
void RecentFileList::setDefault()
{
    this->filePath = "recent_log.json";
    this->fileList.clear();

    // 以下部分为测试,实际使用时请删除
    RecentFileItem* item = new RecentFileItem(
                "hhhhhh.ofd",
                "chaoqun",
                "2017-8-23",
                "2017-8-23",
                "E:/test.ofd");

    RecentFileItem* item2 = new RecentFileItem(
                "aaaaaa.ofd",
                "chaoqun",
                "2017-8-23",
                "2017-8-23",
                "E:/test2.ofd");

    this->fileList.append(item);
    this->fileList.append(item2);

    // 以上部分是测试，实际使用时请删除
}
