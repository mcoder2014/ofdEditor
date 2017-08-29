#ifndef DOC_OFDCONVERTOR_H
#define DOC_OFDCONVERTOR_H

#include <QObject>
#include "model_global.h"

class DocPassage;       // 文章
class DocPage;          // 页
class DocLayer;         // 层
class DocBlock;         // 块
class DocTextBlock;     // 文本框

class OFD;              // OFD文件模型
class Res;              // 资源描述
class ID_Table;         // id table表
class CT_Font;          // ct_font
class CT_ColorSpace;    // 颜色空间
class Page;
class Document;
class CT_Layer;
class DocImageBlock;


/**
 * @Author Chaoqun
 * @brief  将文章转换成为一个 OFD文件，直至保存为.ofd文件
 * @date   2017/06/22
 */
class MODELSHARED_EXPORT Doc_OFDConvertor
        : public QObject
{
    Q_OBJECT
public:
    explicit Doc_OFDConvertor(QObject *parent = 0);
    OFD *doc_to_ofd(DocPassage* passage);           // 执行程序入口

signals:

public slots:

private:

    void buildDocBody();        // 生成DocBody
    void buildDocument();       // 生成Document
    void buildPages(Document *document);                            // 处理页面S

    void buildPage(Page* ctPage,
                   DocPage* docPage);                               // 处理其中某一页

    void buildLayer(CT_Layer* ctLayer,
                    DocLayer* layer);                               // 将一层中的信息存储出来
    void buildText(CT_Layer* ctLayer,DocTextBlock* textBlock);      // 将DocTextBlock中的信息转换处理
    void buildImage(CT_Layer* ctLayer, DocImageBlock *imageBlock);  // 处理图片


    int addFont(CT_Font* font);    // 添加字体类型到资源
    int addColorSpace(CT_ColorSpace* colorSpace);  // 添加颜色空间

    int checkFont(CT_Font* font);  // 检查publicRes中是否存在该字体
    int checkColorSpace(CT_ColorSpace* colorSpace);    // 检查是否存在该颜色空间

private:
    DocPassage* passage;        // 文章
    OFD* ofdFile;               // OFD 对象
    Res* public_res;            // Res 公共资源文件
    Res* document_res;          // 放图片等多媒体资源的res
    ID_Table* table;            // ID table


};

#endif // DOC_OFDCONVERTOR_H
