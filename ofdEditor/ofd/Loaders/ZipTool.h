#ifndef ZIPTOOL_H
#define ZIPTOOL_H

#include <QString>
#include <QStringList>

#include "../ofd_global.h"  // 生成库文件需要



/**
 * @Author Chaoqun
 * @brief  压缩文件、解压文件默认接口
 * @date   2017/04/29
 */
class OFDSHARED_EXPORT ZipTool
{
public:
    ZipTool();

    static QStringList getFileList(QString fileCompressed);
        // 获取压缩文件中的所有文件名
    static void extractDir(QString fileCompressed, QString dir);
        // 解压文件到文件夹
    static bool compressDir(QString fileCompressed,
                            QString dir,bool deleteDir = false);
        // 压缩目录
    static void deleteFolder(const QString& folderFullPath);

    QString FilePathToFloderPath(const QString& filePath);




};

#endif // ZIPTOOL_H
