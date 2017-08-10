#ifndef IMAGEPROPERTIESDIALOG_H
#define IMAGEPROPERTIESDIALOG_H

#include <QDialog>


class DocImageBlock;
namespace Ui {
class ImagePropertiesDialog;
}

class ImagePropertiesDialog : public QDialog
{
    Q_OBJECT

public:
    static ImagePropertiesDialog* getInstance();    // 获得单例
    static void DestoryInstance();                       // 销毁单例
    void init(DocImageBlock* _block);               // 使用时初始化

    ~ImagePropertiesDialog();

private:
    ImagePropertiesDialog(QWidget *parent = 0);
    Ui::ImagePropertiesDialog *ui;
    DocImageBlock * block;
    static ImagePropertiesDialog* m_instance;

    // 使用毫米为单位
    double initial_width;
    double initial_height;
    double pos_x;
    double pos_y;
    bool ratio_locked;
    double page_width;
    double page_height;
    double image_width;
    double image_height;

    void initConnect();

private slots:
    void finished_slots(int value);             // 完成窗口后执行
    void lockRatioStateChanged(int locked);     // 当纵横比锁定发生改变时
//    void Width2HeightTrans(double value);       // 宽发生改变时作用于高
//    void Height2WidthTrans(double value);       // 高发生改变时作用于宽

//    void mm2Percentage(double value);           // 毫米单位到百分比
//    void Percentage2mm(double value);           // 百分比单位到毫米
    void editfinished_MM_x();
    void editfinished_MM_y();
    void editfinished_percentage_x();
    void editfinished_percentage_y();

    void slots_valueChanged_x(double x);            // 当x发生改变时处理
    void slots_valueChanged_y(double y);            // 当y发生改变时处理

signals:
    void changeImageProperties(double new_width,
                               double new_height,
                               double new_x,
                               double new_y,
                               bool ratio_locked);

    void valueChanged_x(double x);          // 图片位置被修改
    void valueChanged_y(double y);          // 图片位置被修改

    void valueChanged_width(double width);  // 图片大小被修改
    void valueChanged_height(double height);// 图片大小被修改
};

#endif // IMAGEPROPERTIESDIALOG_H
