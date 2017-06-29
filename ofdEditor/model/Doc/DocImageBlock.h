#ifndef DOCIMAGEBLOCK_H
#define DOCIMAGEBLOCK_H

#include "model_global.h"
#include "Doc/DocBlock.h"
#include "../Widget/imagepropertiesdialog.h"
#include <QLabel>           //存放图片的QWidget
#include <QMenu>
#include <QAction>

// 本类型用来表述文章中的图形、图片之类的
class DocBlock;
class ImagePropertiesDialog;

class MODELSHARED_EXPORT DocImageBlock
        : public QLabel
{
    Q_OBJECT
public:
    DocImageBlock(QWidget *parent = NULL);
    void setImage(QPixmap & pixmap);
    void setBlock(DocBlock * _block);
    bool isWidthHeightRatioLocked();
    double getWidthHeightRatio();
    DocBlock * getBlock();          //获取代理它的DocBlock
public slots:
    void imagePropertiesChanged(double new_width,
                                double new_height,
                                double new_x,
                                double new_y,
                                bool ratio_locked);
    void changeImage();
    void setImageProperties();
    void removeImage();

protected:
    void focusInEvent(QFocusEvent *ev);
    void focusOutEvent(QFocusEvent *ev);
    void contextMenuEvent(QContextMenuEvent *ev);

private:
    DocBlock * block;               //对代理它的DocBlock的引用
    QMenu * context_menu;           //右键菜单
    QAction * change_image;         //更改图片
    QAction * set_image_properties; //更改图片的位置和尺寸
    QAction * remove_image;         //删除图片
    ImagePropertiesDialog * properties_dialog;
    bool width_height_ratio_locked;
    double width_height_ratio;


signals:
    void sendImageInfo(double image_width,
                       double image_height,
                       double image_x,
                       double image_y,
                       double page_width,
                       double page_height,
                       bool ratio_is_locked);
    void signals_currrentImageBlock(DocImageBlock * textBlock);   //当前操作的imageBlock
};

#endif // DOCIMAGEBLOCK_H

