#ifndef DOCBLOCK_H
#define DOCBLOCK_H

#include "model_global.h"       // 导出lib使用
#include "Doc/DocBasicTypes.h"
#include <QGraphicsProxyWidget>
#include <QGraphicsSceneMouseEvent>

class DocLayer;
class QPainter;


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

    enum RectAdjustStatus{blockNone , blockResize , blockMove};     // 对块的大小状态进行调整
    RectAdjustStatus rectAdjust;        // 用来标识当前修改尺寸的状态

    void setLayer(DocLayer * layer){this->layer = layer;}
    DocLayer * getLayer(){return this->layer;}

    void paintHandle(QPainter& painter);        // 绘制负责变换的控制器
    void resize(qreal w, qreal h);      // 继承函数
    void resize(const QSizeF &size);    // 继承函数

protected:

    void paint(QPainter *painter,
               const QStyleOptionGraphicsItem *option,
               QWidget *widget) override;
    void focusInEvent(QFocusEvent *event) override;         // 焦点关注
    void focusOutEvent(QFocusEvent *event) override;        // 焦点离开

    void hoverMoveEvent(QGraphicsSceneHoverEvent *event);
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) override;
    void mousePressEvent(QGraphicsSceneMouseEvent * event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent * event) override;

    void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);
private:
    DocBox boundary;        // 该块的外包矩形
    DocLayer * layer;       // 该块在哪一个层之中

    QSizeF blockSize;       // 用来纪录大小
    QPointF blockOldPos;    // 用来记录旧的位置

    bool isFocused;         // 是否被聚焦
    bool blockIsResizing;     // 是否正在改变大小
    bool isInResizeArea(const QPointF& pos);    // 检查鼠标是否在重置大小区域


    RectAdjustStatus currentStatus(const QPointF& pos);     // 鼠标所在位置可以进行什么调整
};

#endif // DOCBLOCK_H
