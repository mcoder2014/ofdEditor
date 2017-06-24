#ifndef APPINFO_H
#define APPINFO_H

#include <QObject>
#include <QString>

class APPInfo
        : public QObject
{
    Q_OBJECT
public:
    explicit APPInfo(QObject *parent = 0);

    QString GetAppName();           // 获得应用名称
    QString GetAppVersion();        // 获得应用版本号

    void initFromFile();            // 从文件中获取版本信息

signals:

public slots:
    void setAppName(QString &name);         // 设置名称
    void setAppVersion(QString &version);   // 设置版本号

private:
    QString appName;        // 应用名称
    QString appVersion;     // app版本
};

#endif // APPINFO_H
