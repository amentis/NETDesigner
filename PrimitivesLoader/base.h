#ifndef BASE_H
#define BASE_H

class QString;
#include <QVector>

class Primitive;

class Base
{
public:
    Base();
    Base(const QString& name);
    ~Base();
    const QString *getName();
    const QVector<Primitive*>* primitives();

private:
    QString* mName;
    QVector<Primitive*>* mPrimitives;

};

#endif // BASE_H
