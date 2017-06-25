#ifndef IMAGEPROPERTIESDIALOG_H
#define IMAGEPROPERTIESDIALOG_H

#include <QDialog>
#include "../Doc/DocImageBlock.h"

class DocImageBlock;
namespace Ui {
class ImagePropertiesDialog;
}

class ImagePropertiesDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ImagePropertiesDialog(DocImageBlock * _block, QWidget *parent = 0);
    ~ImagePropertiesDialog();

private:
    Ui::ImagePropertiesDialog *ui;
    DocImageBlock * block;
    double initial_width;
    double initial_height;
    bool ratio_locked;
    double page_width;
    double page_height;
private slots:
    void emitMessage();
    void lockRatioStateChanged(int locked);
    void Width2HeightTrans(double value);
    void Height2WidthTrans(double value);
    void Pixel2Percentage(double value);
    void Percentage2Pixel(double value);
public slots:
    void receiveImageInfo(double image_width,
                          double image_height,
                          double image_x,
                          double image_y,
                          double page_width,
                          double page_height,
                          bool ratio_is_locked);
signals:
    void changeImageProperties(double new_width,
                               double new_height,
                               double new_x,
                               double new_y,
                               bool ratio_locked);
};

#endif // IMAGEPROPERTIESDIALOG_H
