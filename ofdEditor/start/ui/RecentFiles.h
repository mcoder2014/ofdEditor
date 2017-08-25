#ifndef RECENTFILES_H
#define RECENTFILES_H

#include <QWidget>

namespace Ui {
class RecentFiles;
}

class RecentFileItem;                       // 最近文件项

///
/// \brief The RecentFiles class
///     程序初始打开后的关于最近文档的窗口
///
class RecentFiles
        : public QWidget
{
    Q_OBJECT

public:
    static RecentFiles* getInstance();      // 获得单例
    static void destroyInstance();          // 销毁实例
    void init();                            // 初始化

private:
    explicit RecentFiles(QWidget *parent = 0);
    ~RecentFiles();
    static RecentFiles* m_instance;         // 单例

    void appendItem(RecentFileItem* item);  // 在显示界面上插入一项
    void clear();                           // 清空列表

private:
    Ui::RecentFiles *ui;            // 界面文件

private slots:
    void selectOpenFile(int row, int column); // 双击某行能够打开最近文件
    void cleanRecentRecord();                 // 与按钮绑定，清空最近记录

signals:
    void openFile(QString filePath);
};

#endif // RECENTFILES_H
