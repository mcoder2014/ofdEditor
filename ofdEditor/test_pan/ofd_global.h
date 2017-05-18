#ifndef OFD_GLOBAL_H
#define OFD_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(OFD_LIBRARY)
#  define OFDSHARED_EXPORT Q_DECL_EXPORT
#else
#  define OFDSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // OFD_GLOBAL_H
