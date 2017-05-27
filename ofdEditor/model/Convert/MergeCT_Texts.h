#ifndef MERGECT_TEXTS_H
#define MERGECT_TEXTS_H

#include <QVector>
#include <QObject>

#include "DataTypes/text/CT_Text.h"
#include "Doc/DocTextBlock.h"
#include "Doc/DocBlock.h"

/**
 * @Author Chaoqun
 * @brief  本类型用来进行分割、合并CT_Text框框
 * @date   2017/05/27
 */
class MergeCT_Texts
        :public QObject
{
    Q_OBJECT
public:
    MergeCT_Texts();

    QVector<DocBlock*>* execute(QVector<CT_Text*> *texts); // 执行函数，整个流程的入口


private:
    void splitTextCode();   // 将CT_Text  处理成一个CT_Text对应一个TextCode
    void minSize();         // 将CT_Text 压缩成最小包围框
    void mergeLine();       // 将多个合并成一行
    void mergeParagraph();  // 将多行合并成为一段

    QVector<CT_Text*>* data_CT_Text;        // 获得的参数
    QVector<DocBlock*>* data_DocBlock;


};

#endif // MERGECT_TEXTS_H
