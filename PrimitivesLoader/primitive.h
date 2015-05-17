#ifndef PRIMITIVE_H
#define PRIMITIVE_H

#include <QVector>

class QString;

class Base;

class Primitive
{
public:
    enum class ArgumentType {Int, Real, String, Bool};
    Primitive();
    ~Primitive();
    Base* base();
    QString* name();
private:
    QVector<ArgumentType>* mArguments;
    Base* mBase;
    QString* mName;
};

#endif // PRIMITIVE_H
