#ifndef CT_DOCINFO_H
#define CT_DOCINFO_H
#include "../basic_datatype.h"

#include "../../ofd_global.h"  // 生成库文件需要

class OFDSHARED_EXPORT CT_DocInfo {  //文档员数据信息

public:
    string doc_id;
    string title;
    string author;
    string subject;
    string abstract;
    string creation_data;   //！！暂且以string表示
    string mod_date;    //！！同上
    string doc_usage;
    ST_Loc cover;   //封面
    //++关键字
    string creator; //创建文档的应用程序
    string creator_version; //创建文档的应用程序的版本信息
    //++用户自定义信息

    CT_DocInfo();
    ~CT_DocInfo();
};

#endif // CT_DOCINFO_H
