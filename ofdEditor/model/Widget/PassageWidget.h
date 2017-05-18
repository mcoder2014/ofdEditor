#ifndef PASSAGEWIDGET_H
#define PASSAGEWIDGET_H

#include <QWidget>
#include <QScrollArea>

class DocPassage;       // 文章


/**
 * @Author Chaoqun
 * @brief  显示文章的widget
 * @param  参数
 * @return 返回值
 * @date   2017/05/01
 */
class PassageWidget
        : public QWidget
{
    Q_OBJECT
public:
    explicit PassageWidget(QWidget *parent = 0);
private:
    DocPassage * passage;            // 一篇文章
    QScrollArea * scrollArea;        // 滚动条区




signals:

public slots:

};

#endif // PASSAGEWIDGET_H
