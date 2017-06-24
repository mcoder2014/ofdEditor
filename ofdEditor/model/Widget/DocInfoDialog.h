#ifndef DOCINFODIALOG_H
#define DOCINFODIALOG_H

#include <QDialog>
#include "model_global.h"
class CT_DocInfo;       // 文档元信息

namespace Ui {
class DocInfoDialog;
}

class MODELSHARED_EXPORT DocInfoDialog : public QDialog
{
    Q_OBJECT

public:
    explicit DocInfoDialog(QWidget *parent = 0);

    DocInfoDialog(CT_DocInfo* docInfo,QWidget *parent);

    ~DocInfoDialog();

private:
    Ui::DocInfoDialog *ui;
    CT_DocInfo* docInfo;        // 文档元信息
    void init();                // 初始化
    void initUi();              // 初始化ui

public slots:
    void editTitle(const QString & text);       // 修改标题
    void editAuthor(const QString & text);      // 修改作者
    void editSubject(const QString & text);     // 修改主题
    void editAbstract(const QString & text);    // 修改摘要
    void editFileType(const QString & text);    // 文档类型
    void editFileCover(const QString & text);   // 文档封面
    void resetDocId();                          // 重置UUID

};

#endif // DOCINFODIALOG_H
