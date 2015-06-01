#ifndef PRIMITIVESLOADER_H
#define PRIMITIVESLOADER_H

#include "primitivesloader_global.h"

class QString;
template <typename T1, typename T2>class QHash;

class Base;
class Primitive;

class PRIMITIVESLOADERSHARED_EXPORT PrimitivesLoader
{

public:
    PrimitivesLoader();
    ~PrimitivesLoader();

    Base* loadBase(QString* name, QString* infoFileDir, QString* srcFile);
    QString* srcFile(QString* baseName);
    static Primitive* findPrimitiveByName(QVector<Base*>* bases, QString* primitiveName);

private:
    QHash<QString, QString*>* mSrcFiles;
};

#endif // PRIMITIVESLOADER_H
