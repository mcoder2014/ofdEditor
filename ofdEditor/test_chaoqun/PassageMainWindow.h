#ifndef PASSAGEMAINWINDOW_H
#define PASSAGEMAINWINDOW_H

#include <QMainWindow>

class DocPassage;       // 文档类型
class DocPage;          // 文档中的某一页

class PassageMainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit PassageMainWindow(QWidget *parent = 0);
    ~PassageMainWindow();
    void setPassage(DocPassage *passage);
    void addPage(DocPage* page);
signals:

public slots:

private:
    DocPassage* passage;        // 文章
};

#endif // PASSAGEMAINWINDOW_H
