#ifndef BASE_H
#define BASE_H

#include "primitivesloader_global.h"

class QString;
#include <QVector>

class Primitive;

class PRIMITIVESLOADERSHARED_EXPORT Base
{
public:
    Base();
    Base(const QString& name);
    ~Base();
    const QString *getName();
    const QVector<Primitive*>* primitives();
    void addPrimitive(Primitive* newPrimitive);

private:
    QString* mName;
    QVector<Primitive*>* mPrimitives;

};

#endif // BASE_H
