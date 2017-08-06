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

class MODELSHARED_EXPORT FindAndReplaceDock
        : public QDockWidget
{
    Q_OBJECT

public:
    static FindAndReplaceDock* getInstance();               // 获得实例
    static void DestoryInstance();                          // 销毁实例

    ~FindAndReplaceDock();

    void setCurrentPassage(DocPassage * current_passage);
private slots:
    void on_FindLineEdit_textChanged(const QString &arg1);      // 查找的内容变化了
    void on_CountLineEdit_textChanged(const QString &arg1);     // 查找到的内容数量变化了
    void locatePreviousPlace();         //定位到上一个位置
    void locateNextPlace();             //定位到下一个位置
    void replaceText();                 //切换当前文本
    void replaceAll();                  //切换所有文本

private:

    explicit FindAndReplaceDock(QWidget *parent = 0);
    void init();            // 初始化
    void initConnect();     // 初始化信号槽
    void findAllTargetTextBlock(const QString & str);
    void focusOnFoundPart();
    int findNextAvaibleBlockIndex(int cur_index);

    Ui::FindAndReplaceDock *ui;
    static FindAndReplaceDock* m_instance;      // 单例

    QVector<DocTextBlock *> blocks_found;
    QVector<bool> blocks_flag;              //记录blocks_find中的元素是否依然有效
    DocPassage * passage;
    int count;                              //找到的文本数量
    int current_block_index;            //当前选中的DocTextBlock的下边，-1表示将从头开始
    int current_block_pos;              //当前选中的DocTextBlock的当前位置，-1表示将从头开始

};

#endif // FINDANDREPLACEDOCK_H
