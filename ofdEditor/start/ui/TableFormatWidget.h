#ifndef TABLEFORMATWIDGET_H
#define TABLEFORMATWIDGET_H

#include <QWidget>

namespace Ui {
class TableFormatWidget;
}

class TableFormatWidget : public QWidget
{
    Q_OBJECT

public:
    explicit TableFormatWidget(QWidget *parent = 0);
    ~TableFormatWidget();

private:
    Ui::TableFormatWidget *ui;
};

#endif // TABLEFORMATWIDGET_H
