#include "ParagraphFormatDialog.h"
#include "ui_ParagraphFormatDialog.h"

#include "Doc/DocTextBlock.h"

ParagraphFormatDialog* ParagraphFormatDialog::m_instance = NULL;    // 初始化静态单例

///
/// \brief ParagraphFormatDialog::getInstance
///     获得实例
/// \return
///
ParagraphFormatDialog *ParagraphFormatDialog::getInstance()
{
    if( m_instance != NULL)
    {
        return m_instance;
    }

    m_instance = new ParagraphFormatDialog();
    return m_instance;
}

void ParagraphFormatDialog::DestoryInstance()
{
    // 考虑到是QT 组件，暂时不去释放内存
    m_instance = NULL;
}

///
/// \brief ParagraphFormatDialog::init
/// 每次使用单例时都需要将界面初始化一次s
/// \param blockFormat      需要进行设置的段落格式
/// \param textBlock        留下文本框引用
///
void ParagraphFormatDialog::init(
        QTextBlockFormat &blockFormat,
        DocTextBlock *textBlock)
{

    this->init(blockFormat);
    this->textBlock = textBlock;        // 留下引用，仅用来连接信号槽

    connect(this,
            SIGNAL(finished(QTextBlockFormat&)),
            this->textBlock,
            SLOT(setTextBlockFormat(QTextBlockFormat&))); // 连接信号和信号槽


}

ParagraphFormatDialog::ParagraphFormatDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ParagraphFormatDialog)
{
    ui->setupUi(this);

    // 设置默认选项
    QTextBlockFormat blockformat;
    this->init(blockformat);

}

ParagraphFormatDialog::~ParagraphFormatDialog()
{
    delete ui;
}

///
/// \brief ParagraphFormatDialog::getQTextBlockFormat
///     从设置页面中获得设置结果
/// \return
///
QTextBlockFormat ParagraphFormatDialog::getQTextBlockFormat()
{

    QTextBlockFormat blockFormat;           // 用来纪录格式的

    Qt::Alignment horizontalFlag;

    // 水平布局
    int comboHAlignIndex = ui->comboHAlignment->currentIndex();
    switch (comboHAlignIndex) {
    case 0:
         horizontalFlag = (Qt::AlignLeft);
        break;
    case 1:
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

    blockFormat.setAlignment(horizontalFlag);

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


/**
 * @Author Chaoqun
 * @brief  初始化这个Dialog
 * @param  const QTextBlockFormat &blockFormat
 * @return void
 * @date   2017/05/22
 */
void ParagraphFormatDialog::init(const QTextBlockFormat &blockFormat)
{

    // 此信号槽用来将accepted信号接收，然后通过函数发送finished信号
    connect(this,SIGNAL(accepted()),
            this,SLOT(accept_slots()));

    // 设置对齐部分
    Qt::Alignment flag = blockFormat.alignment();       // 先获取对齐的样式
    Qt::Alignment horizontalFlag;

    horizontalFlag = flag & Qt::AlignHorizontal_Mask;

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

    disconnect(this,
            SIGNAL(finished(QTextBlockFormat&)),
            this->textBlock,
            SLOT(setTextBlockFormat(QTextBlockFormat&))); // 断开信号与槽链接
}
