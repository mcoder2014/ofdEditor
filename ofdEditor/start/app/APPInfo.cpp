#include "APPInfo.h"
#include <QFile>
#include <QIODevice>
#include <QTextStream>
#include <QDebug>

APPInfo::APPInfo(QObject *parent) : QObject(parent)
{

}

QString APPInfo::GetAppName()
{
    return this->appName;
}

QString APPInfo::GetAppVersion()
{
    return this->appVersion;
}

void APPInfo::initFromFile()
{
    QFile file(":/appInfo/source/appInfo.json");        // 将文件存在resource内
    if(!file.open(QIODevice::ReadOnly))
    {
        qDebug() << "open app information file failed";
        return;
    }

    QTextStream txtInput(&file);        // 设置文件流
    QString str = txtInput.readAll();   // 读全部文件
    qDebug() << str;

}

void APPInfo::setAppName(QString &name)
{
    this->appName = name;
}

void APPInfo::setAppVersion(QString &version)
{
    this->appVersion = version;
}
