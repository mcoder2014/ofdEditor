#ifndef FONTSETTINGDIALOG_H
#define FONTSETTINGDIALOG_H

#include "model_global.h"
#include <QMap>
#include <QPair>
#include <QDialog>
#include <QTextCharFormat>

class QTextCharFormat;
class QTextBlockFormat;
class QFont;
class DocTextBlock;

namespace Ui {
class FontSettingDialog;
}

class MODELSHARED_EXPORT FontSettingDialog : public QDialog
{
    Q_OBJECT

public:

    FontSettingDialog(DocTextBlock* textBlock,QWidget *parent = 0);
    ~FontSettingDialog();

signals:
    void sendFont(const QTextCharFormat& charFormat);   // 返回结果给DocTextBlock的信号
    void signal_updatePreview(const QTextCharFormat& charFormat);   // 更新预览框的信号

private:
    explicit FontSettingDialog(QWidget *parent = 0);
    Ui::FontSettingDialog *ui;

    DocTextBlock * textBlock;                   // 指向待调整文本框
    QTextCharFormat* charFormat;                // 预览框的格式

    QTextCharFormat getQTextCharFormat();       // 获得更新后的字体


    QMap<int,double> pointSizeTable;                            // 字号与Index对应表
    double pointSizeF(int comboIndex);              // 获取字号大小与Index的对应关系
    int comboIndex(double pointSizeF);              // 获得Index与pointSize的对应关系
    void initConnect();                             // 初始化各种函数的链接


private slots:
    void updatePreview(const QTextCharFormat& charFormat);      // 设置为预览，用来更新预览文字的效果

    void init(const QTextCharFormat& charFormat);

    void updateFontFamily(const QFont &font);       // 更新字体
    void updateFontSizeF(int index);        // 更新字体大小
    void updateBold(int state);             // 加粗
    void updateItalic(int state);           // 斜体
    void updateUnderline(int state);        // 下划线


};

#endif // FONTSETTINGDIALOG_H
