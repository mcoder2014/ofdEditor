#ifndef FINDANDREPLACEDOCK_H
#define FINDANDREPLACEDOCK_H

#include <QDockWidget>
#include <model_global.h>
#include <QVector>

class DocTextBlock;
class DocPassage;

namespace Ui {
class FindAndReplaceDock;
}

class MODELSHARED_EXPORT FindAndReplaceDock : public QDockWidget
{
    Q_OBJECT

public:
    explicit FindAndReplaceDock(QWidget *parent = 0);
    ~FindAndReplaceDock();

    void setCurrentPassage(DocPassage * current_passage);
private slots:
    void on_FindLineEdit_textChanged(const QString &arg1);

    void on_CountLineEdit_textChanged(const QString &arg1);
    void locatePreviousPlace();         //定位到上一个位置
    void locateNextPlace();             //定位到下一个位置
    void replaceText();                 //切换当前文本
    void replaceAll();                  //切换所有文本

private:
    Ui::FindAndReplaceDock *ui;
    QVector<DocTextBlock *> blocks_found;
    QVector<bool> blocks_flag;              //记录blocks_find中的元素是否依然有效
    DocPassage * passage;
    int count;                              //找到的文本数量

    void findAllTargetTextBlock(const QString & str);
    void focusOnFoundPart();
    int findNextAvaibleBlockIndex(int cur_index);

    int current_block_index;            //当前选中的DocTextBlock的下边，-1表示将从头开始
    int current_block_pos;              //当前选中的DocTextBlock的当前位置，-1表示将从头开始

};

#endif // FINDANDREPLACEDOCK_H
