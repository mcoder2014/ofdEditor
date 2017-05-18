#ifndef PARAGRAPHFORMATWIDGET_H
#define PARAGRAPHFORMATWIDGET_H

#include <QWidget>

namespace Ui {
class ParagraphFormatWidget;
}

class ParagraphFormatWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ParagraphFormatWidget(QWidget *parent = 0);
    ~ParagraphFormatWidget();

private:
    Ui::ParagraphFormatWidget *ui;
};

#endif // PARAGRAPHFORMATWIDGET_H
