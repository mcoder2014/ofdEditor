#ifndef PARAGRAPHFORMATWIDGET_H
#define PARAGRAPHFORMATWIDGET_H

#include <QWidget>

#include <QTextBlockFormat>

namespace Ui {
class ParagraphFormatWidget;
}

class ParagraphFormatWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ParagraphFormatWidget(QWidget *parent = 0);
    ~ParagraphFormatWidget();
    ParagraphFormatWidget(const QTextBlockFormat& blockFormat,
                          QWidget *parent = 0);

signals:
    void finished(int ok, QTextBlockFormat& blockFormat);   // 信号

private:
    Ui::ParagraphFormatWidget *ui;  // 界面函数
    void init();                    // 初始化函数
    void init( const QTextBlockFormat& blockFormat);    // 初始化函数

private slots:
    void closeWidget();
};

#endif // PARAGRAPHFORMATWIDGET_H
