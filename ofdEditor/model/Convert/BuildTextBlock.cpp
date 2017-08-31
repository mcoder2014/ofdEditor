#include "BuildTextBlock.h"
#include "DataTypes/page/CT_Layer.h"

BuildTextBlock::BuildTextBlock()
{

}

void BuildTextBlock::buildText(
        DocTextBlock *textblock,
        CT_Layer *layer,
        ID_Table *table,
        Doc_OFDConvertor *convertor)
{
    this->textBlock = textblock;
    this->layer = layer;
    this->table = table;
    this->convertor = convertor;

    this->caculateLineHeight(); // 先计算行高
    caculateBlockLine();        //计算每块的行数
    caculateLineContentWidth();
    build();
}

///
/// \brief BuildTextBlock::caculateLineHeight
///
void BuildTextBlock::caculateLineHeight()
{
    int lineCount = 0;
    this->textBlock->moveCursor(QTextCursor::Start);
    QTextCursor cursor = this->textBlock->textCursor();


    // 一行一行的进行遍历-记录每行的开始位置和行数
    while(!cursor.atEnd())
    {
        lineCount ++;
        cursor.movePosition(QTextCursor::StartOfLine);  // 移动到每行开始的位置
        this->start_of_line_Pos.push_back(cursor.position());

//        cursor.select(QTextCursor::LineUnderCursor);
//        this->every_line_text.push_back(cursor.selectedText());
//        cursor.movePosition(QTextCursor::NoMove);

        cursor.movePosition(QTextCursor::EndOfLine);    // 移动到当前行尾，纪录位置
        this->end_of_line_pos.push_back(cursor.position());   // 将该行的结尾位置发出来


        cursor.movePosition(QTextCursor::Right);

        // 先用每块的字体初始化一下 lineHeight_Font
        QTextCharFormat charFormat = cursor.charFormat();
        QFontMetrics fontMetrics(charFormat.font());

        lineHeight_Font.push_back(
                    fontMetrics.ascent()
                    + fontMetrics.descent()
                    + fontMetrics.leading() + 1);

    }
    cursor.movePosition(QTextCursor::Start);
    while(!cursor.atEnd())
    {
        cursor.select(QTextCursor::LineUnderCursor);
        this->every_line_text.push_back(cursor.selectedText());
        cursor.movePosition(QTextCursor::Right);
    }

    // 计算每行的行高----------------------------------------------
    QTextFrame::iterator  it = textBlock->document()->rootFrame()->begin();     // 遍历rootFrame
    while (!it.atEnd())
    {
        // QTextBlock
        QTextBlock  block = it.currentBlock();  // 当前block
        qDebug() << "block "<< block.text();

        QTextBlock::iterator block_it = block.begin();
        while(!block_it.atEnd())
        {
            // 遍历QTextFragment
            QTextFragment fragment = block_it.fragment();
            start_of_fragment.push_back(fragment.position());

            // 判断 fragment 影响哪几行的高度
            int lineHeight_fragment = 0;
            QFontMetricsF fontMetrics(fragment.charFormat().font());
            lineHeight_fragment = fontMetrics.ascent() + fontMetrics.descent() + fontMetrics.leading() + 1;

            // 利用快的起点和重点，判断影响了哪几行
            int start_pos = fragment.position();
            int end_pos = fragment.position() + fragment.length() - 1;
            int start_line = 0;
            int end_line = 0;

            for(int line = 0; line < end_of_line_pos.size(); line ++)
            {
                if(start_pos > end_of_line_pos[line])
                {
                    start_line = line + 1;
                }

                if(end_pos < end_of_line_pos[line])
                {
                    end_line = line;
                    break;
                }
            }
            qDebug() << "fragment pos"<<fragment.position()
                     << start_line << end_line << lineHeight_fragment << fragment.text();

            for(int line = start_line; line <= end_line; line ++)
            {
                if(lineHeight_fragment > lineHeight_Font[line])
                {
                    lineHeight_Font[line] = lineHeight_fragment;
                }
            }

            block_it ++;
        }           // 遍历fragment
        it ++;
    }               // 遍历block

    // 结合行高策略，计算每行的行高---------------------------------------------
    lineHeight_Real = lineHeight_Font;      // 复制内容

    // 遍历每行，根据行高规则计算行高
    for(int line = 0 ; line < end_of_line_pos.size(); line ++)
    {
        cursor.setPosition(end_of_line_pos[line]);
        QTextBlockFormat blockFormat = cursor.blockFormat();

        int lineHeightType = blockFormat.lineHeightType();
        int lineHeightValue = blockFormat.lineHeight();

        switch (lineHeightType)
        {
        case QTextBlockFormat::SingleHeight:
            // 单倍行距不用改
            break;
        case QTextBlockFormat::MinimumHeight:
            // 最小值
            if(lineHeight_Font[line] < lineHeightValue)
            {
                lineHeight_Real[line] = lineHeightValue;
            }
            else
            {
                lineHeight_Real[line] = lineHeight_Font[line];
            }
            break;
        case QTextBlockFormat::FixedHeight:
            // 固定值
            lineHeight_Real[line] = lineHeightValue;
            break;
        case QTextBlockFormat::ProportionalHeight:
            // 多倍行距
            lineHeight_Real[line] = (int)(1.0 * lineHeightValue / 100 * lineHeight_Font[line]);
            break;
        default:
            break;
        }
    }

    qDebug() << "lineCount" << lineCount << endl
             << "end of line" << end_of_line_pos    << endl
             << " start of fragements" << start_of_fragment << endl
             << "line height font" << lineHeight_Font << endl
             << "line height real" << lineHeight_Real << endl
             << "every line text" << every_line_text << endl;

}

void BuildTextBlock::caculateBlockLine()
{
    QTextFrame::iterator iter = this->textBlock->document()->rootFrame()->begin();  // 开始的位置
    QTextCursor cursor = this->textBlock->textCursor();
    cursor.movePosition(QTextCursor::Start);

    int blockNum = 0;
    while(!iter.atEnd())
    {
        QTextBlock block = iter.currentBlock();
        cursor.setPosition(block.position());
        int startPos = cursor.position();
        int startLine = this->start_of_line_Pos.indexOf(startPos);

        cursor.movePosition(QTextCursor::EndOfBlock);
        int endLine = this->end_of_line_pos.indexOf(cursor.position());

        qDebug() << "block" << blockNum
                 << "startLine" << startLine
                 << "endLine" << endLine
                 << "blockNum" << blockNum
                 << "blockNumber" << block.blockNumber();

        this->blockStartLineNum.push_back(startLine);
        this->blockEndLineNum.push_back(endLine);

        blockNum ++;    // 记录块数
        iter ++;
    }

    // 计算每一个块开始的位置
    int startY = 0;
    blockNum = 0;
    double margin = 0;
    iter = this->textBlock->document()->rootFrame()->begin();
    while(!iter.atEnd())
    {
        // 两个block直接的距离
        if(iter.currentBlock() != this->textBlock->document()->firstBlock())
        {
            // 比较Margin
            double topMargin = iter.currentBlock().blockFormat().topMargin();
            if(topMargin > margin)
            {
                margin = topMargin;
            }
            startY += margin;
        }

        this->blockStartY.push_back(startY);    // 第一个块从0开始
        this->blockStartX.push_back(
                    iter.currentBlock().blockFormat().indent() * this->textBlock->document()->indentWidth());

        double height = 0;
        for(int i = this->blockStartLineNum[blockNum]; i <= this->blockEndLineNum[blockNum]; i++)
        {
            height += lineHeight_Real[i];
        }

        startY += height;       // 将行的高度加入到 startY中

        blockNum ++;
        iter++;
    }

    qDebug() << "block start Y" << blockStartY
             << "block start X" << blockStartX;
}

///
/// \brief BuildTextBlock::caculateLineContentWidth
///
void BuildTextBlock::caculateLineContentWidth()
{
    QTextFrame::iterator iter = this->textBlock->document()->rootFrame()->begin();  // 开始的位置
    int blockNum = 0;
    // 循环每个块，循环处理每个语句，来生成这个行宽度的内容
    while(!iter.atEnd())
    {
        int currentLine = blockStartLineNum[blockNum];      // 从这行开始
        QString lineContent;                // 临时的行内容
        QString fragmentContent;            // 临时的片段内容
        QTextBlock::iterator it_fragment = iter.currentBlock().begin();
        QTextFragment fragment = it_fragment.fragment();
        fragmentContent = fragment.text();

        for(int i = currentLine; i <= blockEndLineNum[blockNum] && !it_fragment.atEnd(); i++)
        {
            lineContent = every_line_text[i];
            double width = 0;
            while(fragmentContent.size() < lineContent.size())
            {
                qDebug() << "fragmentContent " << fragmentContent;

                QFontMetricsF fontMetric(fragment.charFormat().font());
                width += fontMetric.boundingRect(fragmentContent).width();
                it_fragment ++;
                lineContent.remove(0,fragmentContent.size());
                fragment = it_fragment.fragment();
                fragmentContent = fragment.text();

            }

            qDebug() << "lineContent" << lineContent;

            QFontMetricsF fontMetirc(fragment.charFormat().font());
            width += fontMetirc.boundingRect(lineContent).width();
            lineWidth_content.push_back(width);
            fragmentContent.remove(0,lineContent.size());
        }

        iter ++;
        blockNum ++;
    }

    qDebug() << "line Width" << lineWidth_content;
}

///
/// \brief BuildTextBlock::build
///
void BuildTextBlock::build()
{
    QTextFrame::iterator iter = this->textBlock->document()->rootFrame()->begin();  // 开始的位置
    int blockNum = 0;
    // 循环每个块，循环处理每个语句，来生成这个行宽度的内容
    while(!iter.atEnd())
    {
        QTextBlock block = iter.currentBlock();
        QTextBlockFormat blockFormat = block.blockFormat();
        QTextBlock::iterator it_fragment = block.begin();
        QTextFragment fragment = it_fragment.fragment();
        QString lineContent;
        QString fragmentContent;
        double temp_x = 0;
        double temp_y = 0;
        int currentLine = blockStartLineNum[blockNum];

        while(currentLine < blockEndLineNum[blockNum]
              || !it_fragment.atEnd()
              || fragmentContent.size() != 0)
        {
            if(lineContent.size() == 0)
            {
                // 如果当前行没内容了
                lineContent = every_line_text[currentLine];
                temp_y += lineHeight_Real[currentLine];
                if(currentLine == blockStartLineNum[blockNum])
                {
                    int indent = blockFormat.indent() * this->textBlock->document()->indentWidth()
                            + blockFormat.textIndent();

                    temp_x = calculateTemp_x(currentLine,blockFormat.alignment(),indent);
                }
                else
                    temp_x = calculateTemp_x(currentLine, blockFormat.alignment(),
                                             blockFormat.indent() * this->textBlock->document()->indentWidth());
                currentLine ++;
                continue;
            }
            if(fragmentContent.size() == 0)
            {
                // 如果当前处理的块内容用完了
                fragment = it_fragment.fragment();
                fragmentContent = fragment.text();
                it_fragment ++;
                continue;
            }

            QString editContent;
            if(lineContent.size() <= fragmentContent.size())
            {
                editContent = lineContent;
                fragmentContent.remove(0,lineContent.size());
                lineContent = "";
            }
            else
            {
                editContent = fragmentContent;
                lineContent.remove(0,fragmentContent.size());
                fragmentContent = "";
            }

            double width =buildSmall_cttext(
                        editContent,
                        fragment.charFormat().font(),
                        this->textBlock->getBlock()->x() + temp_x,
                        this->textBlock->getBlock()->y() + blockStartY[currentLine] + temp_y,
                        currentLine,
                        blockFormat.alignment());

            temp_x = temp_x + width;
        }


        iter++;
        blockNum++;
    }

    qDebug() << "line Width" << lineWidth_content;
}

double BuildTextBlock::calculateTemp_x(int lineNum, int align, double indent)
{
    double contentWidth = lineWidth_content[lineNum];
    double blockwidth = this->textBlock->getBlock()->size().width();

    switch (align) {
    case Qt::AlignLeft:
        // 左对齐的开始位置
        return indent;
        break;
    case Qt::AlignRight:
        return blockwidth - contentWidth;
        break;
    case Qt::AlignHCenter:
        return (blockwidth - contentWidth) / 2.0;
        break;
    case Qt::AlignJustify:
        return indent;
        break;
    default:
        break;
    }
}

///
/// \brief BuildTextBlock::buildSmall_cttext
/// \param content
/// \param font
/// \param x
/// \param y
/// \param lineNum
/// \param align
/// \return
///
double BuildTextBlock::buildSmall_cttext(
        QString content, QFont font, double x, double y, int lineNum, Qt::Alignment align)
{
    qDebug() <<"build small cttext " <<content;
    // 新建CT_Text
    CT_Text *ct_text = new CT_Text();
    ct_text->setID(this->table->size() +1, this->table);
    this->layer->getTextObject()->append(ct_text);

    CT_Font* ctfont = new CT_Font();
    ctfont->setFamilyName(font.family());
    ctfont->setFontName(font.family());
    int font_id = this->convertor->addFont(ctfont);
    ct_text->setFont(font_id, this->table);

    // 设置字体
    if(font.italic())
    {
        ct_text->setItalic(true);
    }
    if(font.weight() != 50)
    {
        ct_text->setWeight(font.weight() * 8);
    }

    QFontMetricsF fontMetrics(font);

    ct_text->setSize(
                UnitTool::pixelToMM(fontMetrics.boundingRect(content).height()));


    double width = 0;

    switch (align)
    {
    case Qt::AlignLeft:
    case Qt::AlignRight:
    case Qt::AlignHCenter:
        // 左对齐右对齐，居中，字间距不会变
        ct_text->setBoundary(
                    UnitTool::pixelToMM(x),
                    UnitTool::pixelToMM(y),
                    UnitTool::pixelToMM(fontMetrics.boundingRect(content).width() + 10),
                    UnitTool::pixelToMM(lineHeight_Real[lineNum] + 5));

        TextCode *txtCode = new TextCode();        // 文字内容
        txtCode->setText(content);
        txtCode->setX(
                    UnitTool::pixelToMM(1));
        txtCode->setY(
                    UnitTool::pixelToMM(lineHeight_Font[lineNum]));
        if(content.size() > 1)
        {
            QString delta_x;
            for(int delta = 0; delta <content.size() -1; delta ++)
            {
                delta_x  = delta_x + " "
                        + QString::number(
                            UnitTool::pixelToMM(
                                fontMetrics.boundingRect(content.mid(delta,1)).width()));
            }
            txtCode->setDeltaX(delta_x);
        }
        width = fontMetrics.boundingRect(content).width();

        ct_text->getTextCode()->append(txtCode);

        break;

//    case Qt::AlignJustify:

//        double delta_delta_x = 1.0 *
//                (this->textBlock->getBlock()->size().width() - lineWidth_content[lineNum])
//                /( every_line_text[lineNum].size() -1);
//        ct_text->setBoundary(
//                    UnitTool::pixelToMM(x),
//                    UnitTool::pixelToMM(y),
//                    UnitTool::pixelToMM(fontMetrics.boundingRect(content).width()
//                                        + content.length() * delta_delta_x),
//                    UnitTool::pixelToMM(lineHeight_Real[lineNum] + 5));

//        TextCode *textCode = new TextCode();        // 文字内容
//        textCode->setText(content);
//        textCode->setX(
//                    UnitTool::pixelToMM(1));
//        textCode->setY(
//                    UnitTool::pixelToMM(lineHeight_Font[lineNum]));

//        if(content.size() > 1)
//        {
//            QString delta_x;
//            for(int delta = 0; delta <content.size() -1; delta ++)
//            {
//                delta_x  = delta_x + " "
//                        + QString::number(
//                            UnitTool::pixelToMM(
//                                fontMetrics.boundingRect(content.mid(delta,1)).width() + delta_delta_x));
//            }
//            txtCode->setDeltaX(delta_x);
//        }
//        ct_text->getTextCode()->append(txtCode);

//        width = fontMetrics.boundingRect(content).width() + delta_delta_x * content.size();

//        break;
    }

    qDebug() <<"build small cttext " <<content;
    return width;
}


