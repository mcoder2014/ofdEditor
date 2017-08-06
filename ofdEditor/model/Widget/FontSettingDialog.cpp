#include "FontSettingDialog.h"
#include "ui_FontSettingDialog.h"

#include <QTextCharFormat>
#include <QFont>
#include <QDebug>
#include "Doc/DocTextBlock.h"
#include <QTextBrowser>

FontSettingDialog* FontSettingDialog::m_instance = NULL;       // 初始化静态变量

FontSettingDialog::FontSettingDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FontSettingDialog)
{
    ui->setupUi(this);
    init();
}

///
/// \brief FontSettingDialog::getInstance
/// 获得单例的实例
/// \return
///
FontSettingDialog *FontSettingDialog::getInstance()
{
    if( m_instance != NULL)
    {
        return m_instance;
    }

    m_instance = new FontSettingDialog();   // 新建字体设置窗口实例
    return m_instance;
}

///
/// \brief FontSettingDialog::DestoryInstance
/// 用来销毁实例
void FontSettingDialog::DestoryInstance()
{
    // Font窗口暂不释放，交由Qt机制处理
    m_instance = NULL;
}

///
/// \brief FontSettingDialog::init
///     每次调出单例后可执行
/// \param textBlock
///
void FontSettingDialog::init(DocTextBlock *textBlock)
{
    this->textBlock = textBlock;            // 纪录下当前操作的对象
    // 当用完后，记得将对象释放
    connect(this,SIGNAL(sendFont(QTextCharFormat&)),
            this->textBlock,SLOT(setCharFormatOnSelection(QTextCharFormat&)));    // 发送字体

    QTextCursor cursor = this->textBlock->textCursor();     // 获得文本的光标
    QTextCharFormat charFormat = cursor.charFormat();       // 获得光标的charFormat

    this->changeCharFormat(charFormat);                     // 更改字体窗口的当前字体
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
//    int index = this->pointSizeTable.key(pointSizeF, -1);
//    return index;

    QMapIterator<int,double> iter(this->pointSizeTable);        // 迭代器
    int key = -1;

    while(iter.hasNext())
    {
       iter.next();
       if((iter.value() - pointSizeF) > -0.01
               && (iter.value() - pointSizeF) < 0.01)   // 当两个差值绝对值小于0.01即可判断相等
       {
           // 判断相等
           key = iter.key();    // 赋值
           break;
       }
    }

    return key;

}

/**
 * @Author Chaoqun
 * @brief  初始化链接 - 只需要执行一次
 * @param  void
 * @return void
 * @date   2017/05/26
 */
void FontSettingDialog::initConnect()
{
    // 链接字体
    this->connect(this->ui->comboFont,
                  SIGNAL(currentFontChanged(QFont)),
                  this,SLOT(updateFontFamily(QFont)));

    // 链接字号
    this->connect(this->ui->comboFontSize,
                  SIGNAL(currentIndexChanged(int)),
                  this, SLOT(updateFontSizeF(int)));                // 链接字体大小
    // 链接粗体
    connect(this->ui->checkBold, SIGNAL(stateChanged(int)),
            this,SLOT(updateBold(int)));                             // 链接加粗

    // 链接斜体
    connect(this->ui->checkItalic, SIGNAL(stateChanged(int)),
            this,SLOT(updateItalic(int)));                            // 链接斜体

    // 链接下划线
    connect(this->ui->checkUnderline, SIGNAL(stateChanged(int)),
            this,SLOT(updateUnderline(int)));                          // 链接下划线

    // 链接字宽
    connect(this->ui->checkFixedWidth, SIGNAL(stateChanged(int)),
            this,SLOT(updatefixedPitch(int)));                          // 固定字宽

    // 链接字粗
    connect(this->ui->intFontWeight,
            SIGNAL(valueChanged(int)),
            this,SLOT(updateWeight(int)));                         // 字体粗细

    // 字间距
    connect(this->ui->doubleFontSpace,
            SIGNAL(valueChanged(double)),
            this,SLOT(updateWordSpacing(double)));                    // 字间距

    // 链接预览窗口
    connect(this, SIGNAL(signal_updatePreview(QTextCharFormat)),
            this,SLOT(updatePreview(QTextCharFormat)));

    // 当用户点击确认后，执行的函数
    connect(this, SIGNAL(accepted()),
            this, SLOT(accept_slots()));                        // 窗口接受确认的信号

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
    QTextCursor cursor = this->ui->textBrowser->textCursor();
    cursor.select(QTextCursor::Document);                       // 选择这篇文章
    cursor.setCharFormat(charFormat);                           // 设置格式
}

///
/// \brief FontSettingDialog::init
/// 改成单例后，部分界面的初始化只需要做一次
void FontSettingDialog::init()
{
    // 字体大小框
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
    this->pointSizeTable.insert(28,18);
    this->pointSizeTable.insert(29,20);
    this->pointSizeTable.insert(30,22);
    this->pointSizeTable.insert(31,24);
    this->pointSizeTable.insert(32,26);
    this->pointSizeTable.insert(33,28);
    this->pointSizeTable.insert(34,36);
    this->pointSizeTable.insert(35,48);
    this->pointSizeTable.insert(36,72);

    initConnect();                      // 做链接


}

/**
 * @Author Chaoqun
 * @brief  初始化界面
 * @param  const QTextCharFormat &charFormat
 * @return void
 * @date   2017/05/25
 */
void FontSettingDialog::changeCharFormat(const QTextCharFormat &charFormat)
{
    this->charFormat = new QTextCharFormat(charFormat);         // 存下这个charFormat，后面可以使用

    // 字体框
    QFont font = charFormat.font();                 // 获得字体
    this->ui->comboFont->setCurrentFont(font);      // 设置显示

    int fontsizeIndex = this->comboIndex(font.pointSizeF());        // 设置对应的字体大小
    this->ui->comboFontSize->setCurrentIndex(fontsizeIndex);

    // 粗体
    if(font.bold())
    {
        this->ui->checkBold->setChecked(true);
    }
    else
    {
        this->ui->checkBold->setChecked(false);
    }

    // 斜体
    if(font.italic())
    {
        this->ui->checkItalic->setChecked(true);
    }
    else
    {
        this->ui->checkItalic->setChecked(false);
    }

    // 下划线
    if(font.underline())
    {
        this->ui->checkUnderline->setChecked(true);
    }
    else
    {
        this->ui->checkUnderline->setChecked(false);
    }

    // 字间距
//    double wordSpcing = font.wordSpacing();
    double wordSpcing = font.letterSpacing();
    this->ui->doubleFontSpace->setValue(wordSpcing);

    // 拉伸字体
//    int wordStrech = font.stretch();
//    this->ui->doubleFontStretch->setValue(wordStrech);
    this->ui->label_5->setVisible(false);
    this->ui->doubleFontStretch->setVisible(false);
    this->ui->checkFixedWidth->setVisible(false);

    // weight
    int weight = font.weight();
    this->ui->intFontWeight->setValue(weight);

    // 等宽字体
    if(font.fixedPitch())
    {
        this->ui->checkFixedWidth->setChecked(true);
    }
    else
    {
        this->ui->checkFixedWidth->setChecked(false);
    }

    emit signal_updatePreview(*this->charFormat);           // 更新预览窗口

}

/**
 * @Author Chaoqun
 * @brief  槽函数，更新字体类别
 * @param  const QFont &font
 * @return void
 * @date   2017/05/26
 */
void FontSettingDialog::updateFontFamily(const QFont &font)
{
    this->charFormat->setFontFamily(font.family());         // 设置字体

    emit this->signal_updatePreview(*this->charFormat);     // 发送信号

}

/**
 * @Author Chaoqun
 * @brief  槽函数，更新字体大小
 * @param  double pointSizef
 * @return void
 * @date   2017/05/26
 */
void FontSettingDialog::updateFontSizeF(int index)
{
    QFont font = this->charFormat->font();      // 获得字体

    double pointsizef = this->pointSizeF(index);// 计算出点大小
    font.setPointSizeF(pointsizef);             // 设置字体大小
    this->charFormat->setFont(font);            // 更新字体

    emit this->signal_updatePreview(*this->charFormat);     // 发射更新预览信号
}

/**
 * @Author Chaoqun
 * @brief  槽函数，处理加粗
 * @param  int state 0 uncheck 2checked
 * @return 返回值
 * @date   2017/05/26
 */
void FontSettingDialog::updateBold(int state)
{
    QFont font = this->charFormat->font();  // 获得字体

    if(state == 0)
    {
        if(font.bold())
            font.setBold(false);
    }
    else if(state == 2)
    {
        if(!font.bold())
            font.setBold(true);
    }
    else
    {
        font.setBold(false);
    }

    this->charFormat->setFont(font);

    // 更新字重
    int weight = font.weight();
    this->ui->intFontWeight->setValue(weight);

    emit this->signal_updatePreview(*this->charFormat);
}

/**
 * @Author Chaoqun
 * @brief  槽函数，检测斜体
 * @param  int state 0 uncheck 2 checked
 * @return void
 * @date   2017/05/26
 */
void FontSettingDialog::updateItalic(int state)
{

    if(state == 0)
    {
        this->charFormat->setFontItalic(false);
    }
    else if(state == 2)
    {
        this->charFormat->setFontItalic(true);
    }
    else
    {
        this->charFormat->setFontItalic(false);
    }

    emit this->signal_updatePreview(*this->charFormat);
}


/**
 * @Author Chaoqun
 * @brief  检测下划线选项
 * @param  int state
 * @return void
 * @date   2017/05/26
 */
void FontSettingDialog::updateUnderline(int state)
{

    if(state == 0)
    {
        this->charFormat->setFontUnderline(false);
    }
    else if(state == 2)
    {
        this->charFormat->setFontUnderline(true);
    }
    else
    {
        this->charFormat->setFontUnderline(false);
    }

    emit this->signal_updatePreview(*this->charFormat);
}

/**
 * @Author Chaoqun
 * @brief  更新字间距
 * @param  double d
 * @return void
 * @date   2017/05/26
 */
void FontSettingDialog::updateWordSpacing(double d)
{
    QFont font = this->charFormat->font();
//    font.setWordSpacing(d);
    font.setLetterSpacing(QFont::PercentageSpacing,d);
    this->charFormat->setFont(font);

    emit this->signal_updatePreview(*this->charFormat);
}

/**
 * @Author Chaoqun
 * @brief  设置字符比例
 * @param  int i
 * @return void
 * @date   2017/05/26
 */
void FontSettingDialog::updateFontStrech(int i)
{
    QFont font = this->charFormat->font();
    font.setStretch(i);
    this->charFormat->setFont(font);

    emit this->signal_updatePreview(*this->charFormat);
}


/**
 * @Author Chaoqun
 * @brief  是否选择了固定字宽
 * @param  int state
 * @return void
 * @date   2017/05/26
 */
void FontSettingDialog::updatefixedPitch(int state)
{
    if(state == 0)
        this->charFormat->setFontFixedPitch(false);
    else if(state == 2)
        this->charFormat->setFontFixedPitch(true);
    else
        this->charFormat->setFontFixedPitch(false);

    emit this->signal_updatePreview(*this->charFormat);
}

/**
 * @Author Chaoqun
 * @brief  对字体粗细做出反应
 * @param  int i
 * @return void
 * @date   2017/05/26
 */
void FontSettingDialog::updateWeight(int i)
{
    this->charFormat->setFontWeight(i);

    // 更新加粗框
    QFont font = this->charFormat->font();
    if(font.bold())
        this->ui->checkBold->setChecked(true);
    else
        this->ui->checkBold->setChecked(false);

    emit this->signal_updatePreview(*this->charFormat);
}

/**
 * @Author Chaoqun
 * @brief  用户确定事件
 * @param  void
 * @return void
* @date   2017/05/26
 */
void FontSettingDialog::accept_slots()
{ 
    emit this->sendFont(*this->charFormat);

    // 释放链接
    disconnect(this,SIGNAL(sendFont(QTextCharFormat&)),
               this->textBlock,SLOT(setCharFormatOnSelection(QTextCharFormat&)));

}
