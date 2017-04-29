#include "ZipTool.h"
#include <QFile>

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
        QFile* files = new QFile(dir);
        if(!files->exists())
        {
            qDebug("Files don't exist! ",
                   "Maybe wrong input. ",
                   "Maybe they have been deleted.");
            return false;
        }
        else
        {
         return files->remove();
        }
    }
}


ZipTool::ZipTool()
{

}
