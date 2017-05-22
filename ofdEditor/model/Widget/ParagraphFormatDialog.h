#ifndef PARAGRAPHFORMATDIALOG_H
#define PARAGRAPHFORMATDIALOG_H

#include "model_global.h"
#include <QDialog>
#include <QTextBlockFormat>
//#include "Doc/DocTextBlock.h"

class DocTextBlock;

namespace  Ui {
class MODELSHARED_EXPORT ParagraphFormatDialog;
}

class MODELSHARED_EXPORT ParagraphFormatDialog : public QDialog
{
    Q_OBJECT

public:
    // 构造函数
    explicit ParagraphFormatDialog(QWidget *parent = 0);  

    ParagraphFormatDialog(const QTextBlockFormat& blockFormat,
                          DocTextBlock * textBlock,
                          QWidget *parent = 0);

    ~ParagraphFormatDialog();

// 自定义信号
signals:
    // 信号，当选择ok的时候会发出该信号
    void finished(QTextBlockFormat& blockFotmat);

private:
    Ui::ParagraphFormatDialog *ui;
    DocTextBlock * textBlock;           // 引用，将该效果作用于哪个文字之上

    QTextBlockFormat getQTextBlockFormat(); // 从选项中得到QTextBlockFormat

    void init(const QTextBlockFormat & blockFormat);

private slots:
    void accept_slots();    // 用来绑定 QDialog的 accepted信号
};

#endif // PARAGRAPHFORMATDIALOG_H
