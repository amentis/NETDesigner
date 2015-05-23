#ifndef PRIMITIVE_H
#define PRIMITIVE_H

#include "primitivesloader_global.h"

#include <QVector>

class QString;

class Base;

class PRIMITIVESLOADERSHARED_EXPORT Primitive
{
public:
    enum class ArgumentType {Int, Real, String, Bool};
    struct Argument{ ArgumentType type; QString* name; };

    Primitive();
    Primitive(Base* base, QString* name);
    ~Primitive();
    Base* base();
    QString* name();
    QString* definition();
    void addArgument(ArgumentType type, QString* name);
private:
    QVector<Argument*>* mArguments;
    Base* mBase;
    QString* mName;
};

#endif // PRIMITIVE_H
