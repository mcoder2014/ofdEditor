#ifndef RECENTFILES_H
#define RECENTFILES_H

#include <QWidget>

namespace Ui {
class RecentFiles;
}

///
/// \brief The RecentFiles class
///     程序初始打开后的关于最近文档的窗口
///
class RecentFiles
        : public QWidget
{
    Q_OBJECT

public:
    explicit RecentFiles(QWidget *parent = 0);
    ~RecentFiles();

private:
    Ui::RecentFiles *ui;            // 界面文件

};

#endif // RECENTFILES_H
