#ifndef DOCUMENT_PARSER_H
#define DOCUMENT_PARSER_H
#include "DataTypes/document/Document.h"
#include "DataTypes/basic_datatype.h"

class DocumentParser { //解析Document.xml
    Document * doc_data;
    ST_Loc current_path;
public:
    DocumentParser(ST_Loc _current_path);
    Document * getData() {
        return doc_data;
    }
};
#endif // DOCUMENT_PARSER_H
