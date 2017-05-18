#ifndef IMAGEFORMATWIDGET_H
#define IMAGEFORMATWIDGET_H

#include <QWidget>

namespace Ui {
class ImageFormatWidget;
}

class ImageFormatWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ImageFormatWidget(QWidget *parent = 0);
    ~ImageFormatWidget();

private:
    Ui::ImageFormatWidget *ui;
};

#endif // IMAGEFORMATWIDGET_H
