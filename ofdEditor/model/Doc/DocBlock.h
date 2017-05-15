#ifndef DOCBLOCK_H
#define DOCBLOCK_H

#include "model_global.h"       // 导出lib使用
#include "Doc/DocBasicTypes.h"
#include <QGraphicsProxyWidget>
#include <QGraphicsSceneMouseEvent>

class DocLayer;


/**
 * @Author Chaoqun
 * @brief  一个块
 * @param  参数
 * @return 返回值
 * @date   2017/05/03
 */
class MODELSHARED_EXPORT DocBlock
        :public QGraphicsProxyWidget
{
    Q_OBJECT
public:
    DocBlock(QGraphicsItem *parent = 0, Qt::WindowFlags wFlags = 0);

    void setLayer(DocLayer * layer){this->layer = layer;}
    DocLayer * getLayer(){return this->layer;}


protected:

    void focusInEvent(QFocusEvent *event) override;         // 焦点关注
    void focusOutEvent(QFocusEvent *event) override;        // 焦点离开

    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) override;
    void mousePressEvent(QGraphicsSceneMouseEvent * event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent * event) override;

    void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);
private:
    DocBox boundary;        // 该块的外包矩形
    DocLayer * layer;       // 该块在哪一个层之中


};

#endif // DOCBLOCK_H
