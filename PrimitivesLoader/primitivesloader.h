#ifndef PRIMITIVESLOADER_H
#define PRIMITIVESLOADER_H

#include "primitivesloader_global.h"

class Base;
class Primitive;
class QString;

#include <QHash>

class PRIMITIVESLOADERSHARED_EXPORT PrimitivesLoader
{

public:
    PrimitivesLoader();

    Base* loadBase(QString* name, QString* infoFileDir, QString* srcFile);
    QString* srcFile(QString* baseName);
    static Primitive* findPrimitiveByName(QVector<Base*>* bases, QString* primitiveName);

private:
    QHash<QString, QString*> mSrcFiles;
};

#endif // PRIMITIVESLOADER_H
