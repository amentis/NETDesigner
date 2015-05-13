#ifndef PRIMITIVESLOADER_GLOBAL_H
#define PRIMITIVESLOADER_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(PRIMITIVESLOADER_LIBRARY)
#  define PRIMITIVESLOADERSHARED_EXPORT Q_DECL_EXPORT
#else
#  define PRIMITIVESLOADERSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // PRIMITIVESLOADER_GLOBAL_H