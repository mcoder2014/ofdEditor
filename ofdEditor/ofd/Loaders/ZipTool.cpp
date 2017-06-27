#include "ZipTool.h"
#include <QFile>
#include <QDir>
#include <QFileInfoList>
#include <QDebug>

#include "JlCompress.h"     // 压缩文件库

/**
 * @Author Chaoqun
 * @brief  获取压缩文件内所有文件名称，不解压
 * @param  QString fileCompressed
 * @return QStringList
 * @date   2017/04/29
 */
QStringList ZipTool::getFileList(QString fileCompressed)
{
    return JlCompress::getFileList(fileCompressed);
}

/**
 * @Author Chaoqun
 * @brief  解压文件
 * @param  QString fileCompressed
 * @param  QString dir
 * @date   2017/04/29
 */
void ZipTool::extractDir(QString fileCompressed, QString dir)
{
    JlCompress::extractDir(fileCompressed,dir);
}

/**
 * @Author Chaoqun
 * @brief  压缩文件
 * @param  QString fileCompressed   生成压缩文件的文件名
 * @param  QString dir              生成压缩文件的路径
 * @param  bool deleteDir           压缩完成后是否删除源文件,默认不删除
 * @return 返回值
 * @date   2017/04/29
 */
bool ZipTool::compressDir(QString fileCompressed,
                          QString dir, bool deleteDir)
{
    QDir dirs;
    if(dirs.exists(fileCompressed))
        deleteFolder(fileCompressed);

    bool flag = false;
    flag = JlCompress::compressDir(fileCompressed,dir,true);    // 递归
    if(!flag)
    {
        // 如果压缩失败
        qDebug("Compress Files Failed");
        return false;
    }

    if(deleteDir == true)
    {
        // 如果需要删除源文件夹，比如保存并关闭文件
        //QFile* files = new QFile(dir);
        QDir * files = new QDir(dir);

        if(!files->exists())
        {
            qDebug("Files don't exist! ",
                   "Maybe wrong input. ",
                   "Maybe they have been deleted.");
            return false;
        }
        else
        {
            //return files->remove();
            ZipTool::deleteFolder(dir);
        }
        return true;
    }
}

/**
 * @Author Chaoqun
 * @brief  彻底删除某个文件夹
 * @param  const QString &folderFullPath
 * @return void
 * @date   2017/04/29
 */
void ZipTool::deleteFolder(const QString &folderFullPath)
{
    QDir             dir(folderFullPath);
    QFileInfoList    fileList;
    QFileInfo        curFile;
    QFileInfoList    fileListTemp;
    int            infoNum;
    int            i;
    int            j;
    /* 首先获取目标文件夹内所有文件及文件夹信息  */
    fileList=dir.entryInfoList(QDir::Dirs|QDir::Files
                               |QDir::Readable|QDir::Writable
                               |QDir::Hidden|QDir::NoDotAndDotDot
                               ,QDir::Name);

    while(fileList.size() > 0)
    {
        infoNum = fileList.size();

        for(i = infoNum - 1; i >= 0; i--)
        {
            curFile = fileList[i];
            if(curFile.isFile()) /* 如果是文件，删除文件  */
            {
                QFile fileTemp(curFile.filePath());
                fileTemp.remove();
                fileList.removeAt(i);
            }

            if(curFile.isDir()) /* 如果是文件夹  */
            {
                QDir dirTemp(curFile.filePath());
                fileListTemp = dirTemp.entryInfoList(QDir::Dirs | QDir::Files
                                                     | QDir::Readable | QDir::Writable
                                                     | QDir::Hidden | QDir::NoDotAndDotDot
                                                     , QDir::Name);
                if(fileListTemp.size() == 0)
                    /* 下层没有文件或文件夹 则直接删除  */
                {
                    dirTemp.rmdir(".");
                    fileList.removeAt(i);
                }
                else /* 下层有文件夹或文件 则将信息添加到列表  */
                {
                    for(j = 0; j < fileListTemp.size(); j++)
                    {
                        if(!(fileList.contains(fileListTemp[j])))
                        {
                            fileList.append(fileListTemp[j]);
                        }
                    }
                }
            }
        }
    }
    dir.rmdir(".");
    /*删除目标文件夹,
     * 如果只是清空文件夹folderFullPath的内容
     * 而不删除folderFullPath本身,则删掉本行即可  */
}

/**
 * @Author Chaoqun
 * @brief  将文件名按照一定规则转换成文件夹名
 * @param  const QString &filePath
 * @return QString
 * @date   2017/05/22
 */
QString ZipTool::FilePathToFloderPath(const QString &filePath)
{

    QString fileName = filePath.section('/',-1);  // 依据分隔符获得文件名
    fileName = fileName.replace(QString("."),QString("_")); // 将 . 替换成　＿
//    qDebug() << fileName;
    QDir temp = QDir::temp();
    temp.mkdir(fileName );          // 创建临时文件夹
    temp.cd(fileName );             // 进入临时文件夹
//    qDebug() << temp.cd("%"+fileName +"%");
//    qDebug() << temp.path();    // 测试临时目录
    return temp.path();

}


ZipTool::ZipTool()
{

}
