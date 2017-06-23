#ifndef OFD_DOCCONVERTOR_H
#define OFD_DOCCONVERTOR_H

#include "model_global.h"
#include <QObject>
#include "Doc/DocPage.h"

class DocPassage;
class OFD;
class Document;
class Page;
class CT_Layer;
class CT_PageBlock;
class CT_Text;
class CT_Path;
class CT_Image;
class CT_Color;

/**
 * @Author Chaoqun
 * @brief  将OFD格式转换为Doc类型
 * @date   2017/05/23
 */
class MODELSHARED_EXPORT OFD_DocConvertor
        :public QObject
{
    Q_OBJECT
public:
    OFD_DocConvertor();

    DocPassage * ofd_to_doc(OFD * ofd);             // 生成文档

private:
    OFD * ofdFile;  // 当前处理时使用的ofd文件-用来方便获取到公用的特征

    DocPage * buildDocPage(Page * ct_page);         // 生成页

    void insertLayer(DocPage * page,
                     CT_Layer * layer );            // 将每一层加入到页中

    void insertPageBlock(DocPage * page,
                         CT_Layer * layer,
                         CT_PageBlock* pageBlock);  // 用于针对块可能嵌套的情况

    void insertCT_Text(DocPage * page,
                       DocPage::Layer layer,
                       CT_Text *text);              // 插入CT_Text类

    void insertCT_Path(DocPage * page,
                       CT_Layer * layer,
                       CT_Path *path);              // 插入CT_Path

    void insertCT_Image(DocPage * page,
                        DocPage::Layer layer,
                        CT_Image *image);           // 插入CT_Image

    QColor ctColorToQColor(CT_Color* ct_color);     // 颜色转换

};

#endif // OFD_DOCCONVERTOR_H
