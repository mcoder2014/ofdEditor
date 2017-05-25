#include "FontSettingDialog.h"
#include "ui_FontSettingDialog.h"

#include <QTextCharFormat>
#include <QFont>
#include "Doc/DocTextBlock.h"
#include <QTextBrowser>

FontSettingDialog::FontSettingDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FontSettingDialog)
{
    ui->setupUi(this);
}

FontSettingDialog::FontSettingDialog(DocTextBlock *textBlock
                                     , QWidget *parent)
    :QDialog(parent),
    ui(new Ui::FontSettingDialog)
{
    ui->setupUi(this);
    this->textBlock = textBlock;
}

FontSettingDialog::~FontSettingDialog()
{
    delete ui;
}

/**
 * @Author Chaoqun
 * @brief  根据ComboBox的Index来查找字体的点大小
 * @param  int comboIndex
 * @return double
 * @date   2017/05/25
 */
double FontSettingDialog::pointSizeF(int comboIndex)
{
    QMap<int,double>::iterator iter  = this->pointSizeTable.find(comboIndex);
    return *iter;
}

/**
 * @Author Chaoqun
 * @brief  根据pointSizeF来查找
 * @param  参数
 * @return 返回值
 * @date   2017/05/25
 */
int FontSettingDialog::comboIndex(double pointSizeF)
{
    // 如果查询不到，则返回-1
    int index = this->pointSizeTable.key(pointSizeF, -1);
    return index;
}

/**
 * @Author Chaoqun
 * @brief  用来更新预览窗口中文字的效果
 * @param  const QTextCharFormat &charFormat
 * @return void
 * @date   2017/05/25
 */
void FontSettingDialog::updatePreview(const QTextCharFormat &charFormat)
{

}

/**
 * @Author Chaoqun
 * @brief  初始化界面
 * @param  const QTextCharFormat &charFormat
 * @return void
 * @date   2017/05/25
 */
void FontSettingDialog::init(const QTextCharFormat &charFormat)
{
    this->charFormat = new QTextCharFormat(charFormat);         // 存下这个charFormat，后面可以使用

    QFont font = charFormat.font();     // 获得字体

    // 点大小与Index的对应关系
    this->pointSizeTable.insert(0,42);      // 初号
    this->pointSizeTable.insert(1,36);      // 小初
    this->pointSizeTable.insert(2,27.5);    // 一号
    this->pointSizeTable.insert(3,24);      // 小一
    this->pointSizeTable.insert(4,21);      // 二号
    this->pointSizeTable.insert(5,18);      // 小二
    this->pointSizeTable.insert(6,16);      // 三号
    this->pointSizeTable.insert(7,15);      // 小三
    this->pointSizeTable.insert(8,14);      // 四号
    this->pointSizeTable.insert(9,12);      // 小四
    this->pointSizeTable.insert(10,10.5);   // 五号
    this->pointSizeTable.insert(11,9);      // 小五
    this->pointSizeTable.insert(12,7.5);    // 六号
    this->pointSizeTable.insert(13,6.5);    // 小六
    this->pointSizeTable.insert(14,5.25);   // 七号
    this->pointSizeTable.insert(15,4.5);    // 八号
    this->pointSizeTable.insert(16,5);
    this->pointSizeTable.insert(17,5.5);
    this->pointSizeTable.insert(18,6.5);
    this->pointSizeTable.insert(19,7.5);
    this->pointSizeTable.insert(20,8);
    this->pointSizeTable.insert(21,9);
    this->pointSizeTable.insert(22,10);
    this->pointSizeTable.insert(23,10.5);
    this->pointSizeTable.insert(24,11);
    this->pointSizeTable.insert(25,12);
    this->pointSizeTable.insert(26,14);
    this->pointSizeTable.insert(27,16);
    this->pointSizeTable.insert(29,18);
    this->pointSizeTable.insert(30,20);
    this->pointSizeTable.insert(31,22);
    this->pointSizeTable.insert(32,24);
    this->pointSizeTable.insert(33,26);
    this->pointSizeTable.insert(34,28);
    this->pointSizeTable.insert(35,36);
    this->pointSizeTable.insert(36,48);
    this->pointSizeTable.insert(37,72);



    // 设置字体栏
    this->ui->comboFont->setCurrentFont(font);



}
