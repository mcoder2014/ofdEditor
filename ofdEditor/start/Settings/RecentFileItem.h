#ifndef RECENTFILEITEM_H
#define RECENTFILEITEM_H

#include <QObject>
#include <QDateTime>

class RecentFileItem
        : public QObject
{
    Q_OBJECT
public:
    explicit RecentFileItem(QObject *parent = 0);
    RecentFileItem(
            QString fileName,
            QString author,
            QString recentEditTime,
            QString recentOpenTime,
            QString filePath);

    QString getFileName(){return this->fileName;}
    QString getAuthor(){return this->author;}
    QDateTime getRecentOpenTime(){return this->recentOpenTime;}
    QString getRecentOpenTime_str(){
        return this->recentOpenTime.toString(
                    "yyyy-MM-dd HH:mm:ss");}
    QDateTime getRecentEditTime(){return this->recentEditTime;}
    QString getRecentEditTime_str(){
        return this->recentEditTime.toString(
                    "yyyy-MM-dd");}
    QString getFilePath(){return this->filePath;}

    void setFileName(QString fileName){this->fileName = fileName;}
    void setAuthor(QString author){this->author = author;}
    void setRecentOpenTime(QString recentOpenTime);
    void setRecentOpenTime(QDateTime recentOpenTime);
    void setRecentEditTime(QString recentEditTime);
    void setRecentEditTime(QDateTime recentEditTime);
    void setFilePath(QString filePath);

    void init(
            QString fileName,
            QString author,
            QString recentEditTime,
            QString recentOpenTime,
            QString filePath);

    void print();               // 通过qDebug方式输出
    bool isExist();             // 检查该文件是否存在


private:
    QString fileName;           // 文件名
    QString author;             // 作者

    // 根据文件格式，资金修改的时间精确到天
    QDateTime recentOpenTime;   // 最近打开文件时间
    QDateTime recentEditTime;   // 最近编辑文件时间
    QString filePath;           // 文件路径

signals:

public slots:


};

#endif // RECENTFILEITEM_H
