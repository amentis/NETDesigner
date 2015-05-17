#include "base.h"

#include <QtCore>

Base::Base() : mName(nullptr)
{

}

Base::~Base()
{
    delete mName;
}


Base::Base(const QString& name)
{
    mName = new QString(name);
}

const QString* Base::getName()
{
    return mName;
}

const QVector<Primitive*>*Base::primitives()
{
    return mPrimitives;
}
