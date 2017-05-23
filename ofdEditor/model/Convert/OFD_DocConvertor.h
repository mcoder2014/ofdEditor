#ifndef OFD_DOCCONVERTOR_H
#define OFD_DOCCONVERTOR_H

#include "model_global.h"
#include <QObject>

class DocPassage;
class OFD;
class Document;
class DocPage;
class Page;


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

    DocPassage * ofd_to_doc(OFD * ofd);         // 生成文档

private:
    DocPage * buildDocPage(Page * ct_page);     // 生成页

};

#endif // OFD_DOCCONVERTOR_H
