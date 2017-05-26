#ifndef OFDEXCEPTIONS_H
#define OFDEXCEPTIONS_H
#include <QString>
#include "ofd_global.h"

class OFDSHARED_EXPORT Exception
{
    QString msg;
public:
    Exception(QString _msg);
    virtual QString getMessage();
};

class OFDSHARED_EXPORT ParsingException : public Exception {
public:
    ParsingException(QString _msg);
    QString getMessage();
};


class OFDSHARED_EXPORT ParsingFileException : public ParsingException {

public:
    ParsingFileException(QString _msg);
    QString getMessage();
};

class OFDSHARED_EXPORT ParsingFormatException : public ParsingException {

public:
    ParsingFormatException(QString _msg);
    QString getMessage();
};

class OFDSHARED_EXPORT WritingException : public Exception {
public:
    WritingException(QString _msg);
    QString getMessage();
};

class OFDSHARED_EXPORT WritingFormatException : public WritingException {
public:
    WritingFormatException(QString _msg);
    QString getMessage();
};

class OFDSHARED_EXPORT WritingFileException : public WritingException {
public:
    WritingFileException(QString _msg);
    QString getMessage();
};

class OFDSHARED_EXPORT ParsingIDException : public ParsingException {
public:
    ParsingIDException(QString _msg);
    QString getMessage();
};

class OFDSHARED_EXPORT EditingException : public Exception {
public:
    EditingException(QString _msg);
    QString getMessage();
};

class OFDSHARED_EXPORT InvalidIDException : public EditingException {
public:
    InvalidIDException(QString _msg);
    QString getMessage();
};

class OFDSHARED_EXPORT InvalidValueException : public EditingException {
public:
    InvalidValueException(QString _msg);
    QString getMessage();
};

class OFDSHARED_EXPORT IllegalAccessException : public EditingException {
public:
    IllegalAccessException(QString _msg);
    QString getMessage();
};

#endif // OFDEXCEPTIONS_H
