#ifndef BUILDTEXTBLOCK_H
#define BUILDTEXTBLOCK_H

#include "model_global.h"
#include "DataTypes/page/CT_Layer.h"
#include "DataTypes/basic_datatype.h"
#include "DataTypes/text/ct_text.h"
#include "DataTypes/document/Res.h"
#include "DataTypes/text/CT_Font.h"
#include "DataTypes/text/TextCode.h"
#include "Doc/DocTextBlock.h"
#include "Doc/DocBlock.h"
#include "Tool/UnitTool.h"
#include "Doc_OFDConvertor.h"

#include <QVector>
#include <QTextCursor>
#include <QTextCharFormat>
#include <QTextBlockFormat>
#include <QTextdocument>
#include <QTextBlock>
#include <QFontMetricsF>

class MODELSHARED_EXPORT BuildTextBlock
{
public:
    BuildTextBlock();

    void buildText(
            DocTextBlock* textblock,
            CT_Layer    *layer,
            ID_Table *table,
            Doc_OFDConvertor *convertor);

    void caculateLineHeight();
    void caculateBlockLine();               // 计算每个块开始于第几行，结束于第几行
    void caculateLineContentWidth();        // 计算每行文本所占的文字的宽度
    void build();                           // 开始构建真正的内容
    double calculateTemp_x(int lineNum, int align, double indent);  // 根据现有信息，计算出temp_x
    double buildSmall_cttext(QString content, QFont font, double x, double y, int lineNum, Qt::Alignment align);

    DocTextBlock *textBlock;
    CT_Layer *layer;
    ID_Table *table;
    Doc_OFDConvertor* convertor;

    QVector<int> lineHeight_Font;                   // 每个字体的实际高度
    QVector<int> lineHeight_Real;                   // 经过段落格式后的高度
    QVector<int> end_of_line_pos;                   // 纪录每一行结尾的地方
    QVector<int> start_of_line_Pos;                 // 记录每行开始的位置
    QVector<int> start_of_fragment;                 // 每一个QTextFragment开始的位置
    QVector<QString> every_line_text;               // 记录每行的文字的内容

    QVector<int> blockStartLineNum;                 // 块开始于第几行
    QVector<int> blockEndLineNum;                   // 块结束于第几行

    QVector<double > blockStartY;                   // 每个块从哪个y值开始
    QVector<double > blockStartX;                   // 每个块从哪个x值开始

    QVector<double> lineWidth_content;              // 某行内容的宽度--通过QFontMetricsF 计算
};

#endif // BUILDTEXTBLOCK_H
