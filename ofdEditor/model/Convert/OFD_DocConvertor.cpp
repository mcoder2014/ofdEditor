#include "OFD_DocConvertor.h"

#include "Doc/DocPassage.h"
#include "DataTypes/document/ofd.h"
#include "DataTypes/document/document.h"
#include "DataTypes/page/ct_pages.h"
#include "DataTypes/page/page.h"


OFD_DocConvertor::OFD_DocConvertor()
{

}

/**
 * @Author Chaoqun
 * @brief  将ofd格式转换成doc格式
 * @param  OFD * ofd
 * @return DocPassage *
 * @date   2017/05/23
 */
DocPassage *OFD_DocConvertor::ofd_to_doc(OFD *ofd)
{
    DocPassage * passage = new DocPassage();

    // version
    passage->setVersion(ofd->version);
    passage->setDocType(ofd->doc_type);
    passage->setDocInfo((*(ofd->docbodys))[0]->doc_info);

    CT_Pages pages = (*(ofd->docs))[0]->pages;      // 获得页数



    return NULL;
}

DocPage *OFD_DocConvertor::buildDocPage(Page *ct_page)
{
    return NULL;
}
