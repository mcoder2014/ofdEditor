#ifndef OFD_DOCCONVERTOR_H
#define OFD_DOCCONVERTOR_H

#include "model_global.h"
#include <QObject>

class MODELSHARED_EXPORT OFD_DocConvertor
        :public QObject
{
    Q_OBJECT
public:
    OFD_DocConvertor();
};

#endif // OFD_DOCCONVERTOR_H
