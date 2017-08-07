#ifndef PARAGRAPHFORMATDIALOG_H
#define PARAGRAPHFORMATDIALOG_H

#include "model_global.h"
#include <QDialog>
#include <QTextBlockFormat>

class DocTextBlock;

namespace  Ui {
class MODELSHARED_EXPORT ParagraphFormatDialog;
}

///
/// \brief The ParagraphFormatDialog class
///
class MODELSHARED_EXPORT ParagraphFormatDialog
        : public QDialog
{
    Q_OBJECT

public:
    static ParagraphFormatDialog* getInstance();    // 获得实例
    static void DestoryInstance();                  // 销毁实例
    void init(QTextBlockFormat &blockFormat,
              DocTextBlock* textBlock);             // 每次使用时的初始化

    // 构造函数
    explicit ParagraphFormatDialog(QWidget *parent = 0);  

    ~ParagraphFormatDialog();

// 自定义信号
signals:
    // 信号，当选择ok的时候会发出该信号
    void finished(QTextBlockFormat& blockFotmat);

private:
    Ui::ParagraphFormatDialog *ui;
    DocTextBlock * textBlock;           // 引用，将该效果作用于哪个文字之上
    static ParagraphFormatDialog* m_instance;    // 实例

    QTextBlockFormat getQTextBlockFormat(); // 从选项中得到QTextBlockFormat

    void init(const QTextBlockFormat & blockFormat);

private slots:
    void accept_slots();    // 用来绑定 QDialog的 accepted信号
    void finished_slots(int value);     // 用来做窗口关闭后的最后处理
};

#endif // PARAGRAPHFORMATDIALOG_H
