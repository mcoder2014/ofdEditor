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
    void buildDocInfo();        // 存储DocInfo相关的信息
    void buildPage();           // 储存文本信息，顺便整理文件
    void buildLayer(DocLayer* layer);   // 将一层中的信息存储出来
    void buildText(DocTextBlock* textBlock);    // 将DocTextBlock中的信息转换处理

private:
    DocPassage* passage;        // 文章
    OFD* ofdFile;               // OFD对象

};

#endif // DOC_OFDCONVERTOR_H
