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

    static FontSettingDialog* getInstance();              // 获得单例--需要是静态的
    static void DestoryInstance();                        // 销毁实例
    void init(DocTextBlock* textBlock);                   // 将窗口初始化

    ~FontSettingDialog();

signals:
    void sendFont(QTextCharFormat& charFormat);   // 返回结果给DocTextBlock的信号
    void signal_updatePreview(const QTextCharFormat& charFormat);   // 更新预览框的信号

private:
    explicit FontSettingDialog(QWidget *parent = 0);
    Ui::FontSettingDialog *ui;                  // 界面元素

    static FontSettingDialog* m_instance;       // 单例

    DocTextBlock * textBlock;                   // 指向待调整文本框
    QTextCharFormat* charFormat;                // 预览框的格式

    QTextCharFormat getQTextCharFormat();       // 获得更新后的字体

    QMap<int,double> pointSizeTable;                // 字号与Index对应表
    double pointSizeF(int comboIndex);              // 获取字号大小与Index的对应关系
    int comboIndex(double pointSizeF);              // 获得Index与pointSize的对应关系
    void initConnect();                             // 初始化各种函数的链接


private slots:
    void updatePreview(const QTextCharFormat& charFormat);      // 设置为预览，用来更新预览文字的效果
    void init();                           // 初始化界面
    void changeCharFormat(const QTextCharFormat& charFormat);   // 根据字体更新界面选项

    void updateFontFamily(const QFont &font);       // 更新字体类别
    void updateFontSizeF(int index);        // 更新字体大小
    void updateBold(int state);             // 加粗
    void updateItalic(int state);           // 斜体
    void updateUnderline(int state);        // 下划线
    void updateWordSpacing(double d);       // 更新字间距s
    void updateFontStrech(int i);           // 更新字比例
    void updatefixedPitch(int state);       // 固定字宽
    void updateWeight(int i);               // 字体粗细

    void accept_slots();                    // 向DocTextBlock传递成功事件

};

#endif // FONTSETTINGDIALOG_H
