#ifndef DOCUMENTCLASSES_H
#define DOCUMENTCLASSES_H
#include "basic_datatype.h"
#include "text_object_classes.h"
#include <vector>
#include <string>

using std::string;
using std::vector;

//class OFD { //主入口文件的根元素
//    //属性
//    string version; //版本号
//    string doc_type;    //文档子集类型
//};

//class CT_DocInfo {  //文档员数据信息
//    string doc_id;
//    string title;
//    string author;
//    string subject;
//    string abstract;
//    string creation_data;   //！！暂且以string表示
//    string mod_date;    //！！同上
//    string doc_usage;
//    ST_Loc cover;   //封面
//    //++关键字
//    string creator; //创建文档的应用程序
//    string creator_version; //创建文档的应用程序的版本信息
//    //++用户自定义信息
//};

//class DocBody { //文档对象入口，在一个文件中可以有多个文档
//    CT_DocInfo doc_info;    //文档元数据信息描述
//    ST_Loc doc_root;    //指向文档根节点
//};

//class CT_CommonData {
//    ST_ID max_unit_id;  //当前文档中所有对象使用的最大标识
//    CT_PageArea page_area;  //指定多个页面区域的大小和位置
//    vector<ST_Loc> public_res;  //公共资源序列，每个资源指向OFD包内部的一个XML文件
//    //vector<CT_TemplatePage> template_page; //模板页序列
//    ST_RefID default_cs;    //缺省颜色空间
//};

//class CT_OutlineElem {
//    string title;
//    int count;
//    bool expanded;
//    //++动作
//    vector<CT_OutlineElem> outline_elem;
//};

//class CT_Outlines {
//    vector<CT_OutlineElem> outline_elem;
//};

//class CT_PageBlock {
//public:
//    vector<CT_Text> text_objects;
//};

//class CT_Layer : CT_PageBlock {
//public:
//    enum LayerType {Body, Foreground, Background};
//    LayerType type;
//};

//class Page {
//public:
//    CT_PageArea area;   //该页页面区域大小
//    ST_Loc page_res;    //指向该页使用的资源文件
//    vector<CT_Layer> content;
//    //++动作
//};

//class CT_Pages {
//    vector<Page> page;
//};

//class Document {
//    CT_CommonData common_data;  //文档公共数据，定义了默认页面单位、页面区域定义、公共资源等数据
//    CT_Pages pages; //页树
//    CT_Outlines outlines;   //大纲树
//    ST_Loc custom_tags; //指向自定义标引文件
//    ST_Loc extensions;  //指向拓展文件
//    ST_Loc annotations; //指向注释文件
//};

#endif // DOCUMENTCLASSES_H
