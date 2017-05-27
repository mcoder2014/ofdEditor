#include "ofdexceptions.h"

Exception::Exception(QString _msg)
    : msg(_msg) {}

ParsingException::ParsingException(QString _msg)
    : Exception(_msg) {}

ParsingFileException::ParsingFileException(QString _msg)
    : ParsingException(_msg) {}

ParsingFormatException::ParsingFormatException(QString _msg)
    : ParsingException(_msg) {}

QString Exception::getMessage() {
    return msg;
}

QString ParsingException::getMessage() {
    return Exception::getMessage();
}

QString ParsingFileException::getMessage() {
    return "OFD Parsing File Exception:\n" + ParsingException::getMessage();
}

QString ParsingFormatException::getMessage() {
    return "OFD Parsing Format Exception:\n" + ParsingException::getMessage();
}

WritingException::WritingException(QString _msg)
    : Exception(_msg) {}

WritingFileException::WritingFileException(QString _msg)
    : WritingException(_msg) {}

WritingFormatException::WritingFormatException(QString _msg)
    : WritingException(_msg) {}

QString WritingException::getMessage() {
    return Exception::getMessage();
}

QString WritingFileException::getMessage() {
    return "OFD Writing File Exception:  " + WritingException::getMessage();
}

QString WritingFormatException::getMessage() {
    return "OFD Writing Format Exception:  " + WritingException::getMessage();
}

ParsingIDException::ParsingIDException(QString _msg)
    : ParsingException(_msg) {}

QString ParsingIDException::getMessage() {
    return "OFD Parsing ID Exception: " + ParsingException::getMessage();
}

EditingException::EditingException(QString _msg)
    : Exception(_msg) {}

QString EditingException::getMessage() {
    return Exception::getMessage();
}

InvalidIDException::InvalidIDException(QString _msg)
    : EditingException(_msg) {}

QString InvalidIDException::getMessage() {
    return "OFD Editing Exception : Invalid ID. \n " + EditingException::getMessage();
}

InvalidValueException::InvalidValueException(QString _msg)
    : EditingException(_msg) {}

QString InvalidValueException::getMessage() {
    return "OFD Editing Exception : Invalid Value. \n " + EditingException::getMessage();
}

IllegalAccessException::IllegalAccessException(QString _msg)
    : EditingException(_msg) {}
QString IllegalAccessException::getMessage() {
    return "OFD Illegal Access Exception: Try To Access Uninitialized Value. \n" + EditingException::getMessage();
}
