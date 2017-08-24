#include "RecentFileItem.h"
#include <QDebug>
#include <QFile>

RecentFileItem::RecentFileItem(QObject *parent) : QObject(parent)
{

}

RecentFileItem::RecentFileItem(
        QString fileName,
        QString author,
        QString recentEditTime,
        QString recentOpenTime,
        QString filePath):
    QObject(0)
{
    this->fileName = fileName;
    this->author = author;
    this->recentEditTime = QDateTime::fromString(
                recentEditTime,
                "yyyy-MM-dd");
    this->recentOpenTime = QDateTime::fromString(
                recentOpenTime,
                "yyyy-MM-dd");
    this->filePath = filePath;
}

void RecentFileItem::setRecentOpenTime(QString recentOpenTime)
{
    this->recentOpenTime = QDateTime::fromString(
                recentOpenTime,
                "yyyy-MM-dd");
}

void RecentFileItem::setRecentOpenTime(QDateTime recentOpenTime)
{
    this->recentOpenTime = recentOpenTime;
}

void RecentFileItem::setRecentEditTime(QString recentEditTime)
{
    this->recentEditTime = QDateTime::fromString(
                recentEditTime,
                "yyyy-MM-dd");
}

void RecentFileItem::setRecentEditTime(QDateTime recentEditTime)
{
    this->recentEditTime = recentEditTime;
}

void RecentFileItem::setFilePath(QString filePath)
{
    this->filePath = filePath;
}

void RecentFileItem::init(
        QString fileName,
        QString author,
        QString recentEditTime,
        QString recentOpenTime,
        QString filePath)
{
    this->fileName = fileName;
    this->author = author;
    this->recentOpenTime = QDateTime::fromString(
                recentOpenTime,
                "yyyy-MM-dd");
    this->recentOpenTime = QDateTime::fromString(
                recentEditTime,
                "yyyy-MM-dd");
    this->filePath = filePath;
}

void RecentFileItem::print()
{
    qDebug() << "fileName " << this->fileName
             << " author " << this->author
             << " recent open time " << this->recentOpenTime.toString("yyyy-MM-dd")
             << " recent edit time" << this->recentEditTime.toString("yyyy-MM-dd")
             << " filePath" << this->filePath;
}

bool RecentFileItem::isExist()
{
    QFile qfile;
    qfile.setFileName(this->filePath);
    return qfile.exists();
}
