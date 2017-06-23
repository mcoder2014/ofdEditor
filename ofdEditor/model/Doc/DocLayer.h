#ifndef DOCLAYER_H
#define DOCLAYER_H

#include "model_global.h"       // 导出lib使用
#include "Doc/DocPage.h"

#include <QVector>

class DocBlock;     // 块
class DocTable;     // 表格
class DocDrawParam; // 默认绘画模式
class DocPassage;   // 文章

/**
 * @Author Chaoqun
 * @brief  层概念，只负责作为一个管理集合，并无实际用途
 * @date   2017/04/30
 */
class MODELSHARED_EXPORT DocLayer
{
public:
    DocLayer();
    DocLayer(DocPage::Layer layer);
    ~DocLayer();


    qreal getZValue(){return this->zValue;}  // 获取本层的 ZValue的值

    QVector<DocBlock *> *getBlocks();       // 获得所有Block
    DocPage::Layer getLayer(){return type;}
    DocPage* getPage();                     // 找到层所在的Page
    DocPassage *getPassage();               // 找到文章

public slots:

void setZValue(qreal z);             // 设置本层的 ZValue的值
void addBlock(DocBlock *block);     // 添加Block
void removeBlock(DocBlock* block);  // 移除Block
void setLayer(DocPage::Layer layer){this->type = layer;}
void setPage(DocPage* page);        // 设置page


private:
    QVector<DocBlock* > blocks;         // 块    -文字块图形块的父类
    QVector<DocTable* > tables;         // 表格

    DocDrawParam* drawParam;            // 该层的默认绘画模式

    DocPage::Layer type;                // 共三层
    DocPage* parent;                    // 表明此层是哪一个页面的

    qreal zValue;               // 该层 QGraphicsItem默认的zValue;

};

#endif // DOCLAYER_H
