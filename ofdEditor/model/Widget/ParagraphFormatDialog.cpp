#include "ParagraphFormatDialog.h"
#include "ui_ParagraphFormatDialog.h"

#include "Doc/DocTextBlock.h"

ParagraphFormatDialog::ParagraphFormatDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ParagraphFormatDialog)
{


    // 设置默认选项
    QTextBlockFormat blockformat;
    this->init(blockformat);
}

ParagraphFormatDialog::~ParagraphFormatDialog()
{
    delete ui;
}

QTextBlockFormat ParagraphFormatDialog::getQTextBlockFormat()
{

    QTextBlockFormat blockFormat;           // 用来纪录格式的

    Qt::Alignment horizontalFlag;
//    Qt::Alignment verticalFlag;

    // 水平布局
    int comboHAlignIndex = ui->comboHAlignment->currentIndex();
    switch (comboHAlignIndex) {
    case 0:
//        horizontalFlag = (Qt::AlignLeft | Qt::AlignAbsolute);
         horizontalFlag = (Qt::AlignLeft);
        break;
    case 1:
//        horizontalFlag = (Qt::AlignRight | Qt::AlignAbsolute);
        horizontalFlag = (Qt::AlignRight);
        break;
    case 2:
        horizontalFlag = Qt::AlignHCenter;
        break;
    case 3:
        horizontalFlag = Qt::AlignJustify;
        break;
    default:
        break;

    }

//    // 竖直布局
//    int comboVAlignIndex = ui->comboVAlignment->currentIndex();
//    switch (comboVAlignIndex) {
//    case 0:
//        verticalFlag = Qt::AlignTop;
//        break;
//    case 1:
//        verticalFlag = Qt::AlignBottom;
//        break;
//    case 2:
//        verticalFlag = Qt::AlignVCenter;
//        break;
//    default:
//        break;
//    }

    // 设置布局规则
//    blockFormat.setAlignment(verticalFlag | horizontalFlag);
    blockFormat.setAlignment(horizontalFlag);

//    // 设置文字方向
    // 无法使用，仅对部分语言有效
//    Qt::LayoutDirection textDirection;
//    int textDirectionIndex = ui->comboTextDirection->currentIndex();
//    switch (textDirectionIndex) {
//    case 0:
//        textDirection = Qt::LeftToRight;
//        break;
//    case 1:
//        textDirection = Qt::RightToLeft;
//        break;
//    default:
//        textDirection = Qt::LeftToRight;
//        break;
//    }
//    blockFormat.setLayoutDirection(textDirection);

    // 缩进规则
    int paraIndent = ui->paraIndent->value();
    double firstIndent = ui->doubleFirstIndent->value();

    blockFormat.setIndent(paraIndent);
    blockFormat.setTextIndent(firstIndent);


    // 设置段前段后
    double spaceBefore = ui->doubleBefor->value();
    double spaceAfter = ui->doubleAfter->value();

    blockFormat.setTopMargin(spaceBefore);
    blockFormat.setBottomMargin(spaceAfter);

    // 设置行高
    int comboLineHeightIndex = ui->comboLineHeight->currentIndex(); // 判断行高类型
    double lineHeight = ui->doubleLineHight->value();               // 获得行高

    switch (comboLineHeightIndex) {
    case 0:
        blockFormat.setLineHeight(lineHeight,
                                  QTextBlockFormat::SingleHeight);
        break;
    case 1:
        blockFormat.setLineHeight(lineHeight,
                                   QTextBlockFormat::ProportionalHeight);
        break;
    case 2:
        blockFormat.setLineHeight(lineHeight,
                                  QTextBlockFormat::FixedHeight);
        break;
    case 3:
        blockFormat.setLineHeight(lineHeight,
                                  QTextBlockFormat::MinimumHeight);
    default:
        break;
    }

    return blockFormat;
}

ParagraphFormatDialog::ParagraphFormatDialog(
        const QTextBlockFormat &blockFormat,
        DocTextBlock *textBlock, QWidget *parent)
    :QDialog(parent),
    ui(new Ui::ParagraphFormatDialog)
{
    this->init(blockFormat);
    this->textBlock = textBlock;        // 留下引用，仅用来连接信号槽

    connect(this, SIGNAL(finished(int)),
            this->textBlock, SLOT(setTextBlockFormat(QTextBlockFormat))); // 连接信号和信号槽
}

/**
 * @Author Chaoqun
 * @brief  初始化这个Dialog
 * @param  const QTextBlockFormat &blockFormat
 * @return void
 * @date   2017/05/22
 */
void ParagraphFormatDialog::init(const QTextBlockFormat &blockFormat)
{
    ui->setupUi(this);

    // 此信号槽用来将accepted信号接收，然后通过函数发送finished信号
    connect(this,SIGNAL(accepted()),
            this,SLOT(accept_slots()));

    // 设置对齐部分
    Qt::Alignment flag = blockFormat.alignment();       // 先获取对齐的样式
    Qt::Alignment horizontalFlag;
//    Qt::Alignment verticalFlag;

    horizontalFlag = flag & Qt::AlignHorizontal_Mask;
//    verticalFlag = flag & Qt::AlignVertical_Mask;

    // 水平对齐
    switch (horizontalFlag) {
    case Qt::AlignLeft:
        ui->comboHAlignment->setCurrentIndex(0);
        break;
    case Qt::AlignRight:
        ui->comboHAlignment->setCurrentIndex(1);
        break;
    case Qt::AlignHCenter:
        ui->comboHAlignment->setCurrentIndex(2);
        break;
    case Qt::AlignJustify:
        ui->comboHAlignment->setCurrentIndex(3);
        break;
    default:
        ui->comboHAlignment->setCurrentIndex(-1);
        break;
    }

//    // 竖直对齐
//    switch (verticalFlag) {
//    case Qt::AlignTop:
//        ui->comboVAlignment->setCurrentIndex(0);
//        break;
//    case Qt::AlignBottom:
//        ui->comboVAlignment->setCurrentIndex(1);
//        break;
//    case Qt::AlignCenter:
//        ui->comboVAlignment->setCurrentIndex(2);
//        break;
//    default:
//        ui->comboVAlignment->setCurrentIndex(-1);
//        break;
//    }

    // 因竖直对齐的调整并没有效果，这里选择去掉
    this->ui->comboVAlignment->setVisible(false);
    this->ui->label_12->setVisible(false);

// 因为文字方向无法使用，所以将其隐藏掉
//    //文字方向
//    Qt::LayoutDirection textDirection = blockFormat.layoutDirection();
//    switch (textDirection) {
//    case Qt::LeftToRight:
//        ui->comboTextDirection->setCurrentIndex(0);
//        break;
//    case Qt::RightToLeft:
//        ui->comboTextDirection->setCurrentIndex(1);
//        break;
//    default:
//        ui->comboTextDirection->setCurrentIndex(0);
//        break;
//    }
    this->ui->comboTextDirection->setVisible(false);
    this->ui->label_13->setVisible(false);

    // 整段缩进
    int indent = blockFormat.indent();
    ui->paraIndent->setValue(indent);

    // 首行缩进
    ui->doubleFirstIndent->setValue(blockFormat.textIndent());

    /*间距*/

    // 段前
    double topMargin = blockFormat.topMargin();
    ui->doubleBefor->setValue(topMargin);

    // 段后
    double bottomMargin = blockFormat.bottomMargin();
    ui->doubleAfter->setValue(bottomMargin);

    // 行高
//    QTextBlockFormat::LineHeightTypes lineHeightTypes;
    int lineHeightTypes;
    lineHeightTypes = blockFormat.lineHeightType();

    switch (lineHeightTypes) {
    case QTextBlockFormat::SingleHeight:
        ui->comboLineHeight->setCurrentIndex(0);
        break;
    case QTextBlockFormat::ProportionalHeight:
        ui->comboLineHeight->setCurrentIndex(1);
        break;
    case QTextBlockFormat::FixedHeight:
        ui->comboLineHeight->setCurrentIndex(2);
        break;
    case QTextBlockFormat::MinimumHeight:
        ui->comboLineHeight->setCurrentIndex(3);
        break;
    default:
        ui->comboLineHeight->setCurrentIndex(-1);
        break;
    }

    // 行高值
    double lineHeightValue = blockFormat.lineHeight();
    ui->doubleLineHight->setValue(lineHeightValue);

}

/**
 * @Author Chaoqun
 * @brief  绑定accepted信号，用来向外界发送信号
 * @param  void
 * @return void
 * @date   2017/05/22
 */
void ParagraphFormatDialog::accept_slots()
{
    QTextBlockFormat blockFormat = this->getQTextBlockFormat();
    emit this->finished(blockFormat);
}
