#ifndef TEXTFORMATWIDGET_H
#define TEXTFORMATWIDGET_H

#include <QWidget>

namespace Ui {
class TextFormatWidget;
}

class TextFormatWidget : public QWidget
{
    Q_OBJECT

public:
    explicit TextFormatWidget(QWidget *parent = 0);
    ~TextFormatWidget();

private:
    Ui::TextFormatWidget *ui;
};

#endif // TEXTFORMATWIDGET_H
