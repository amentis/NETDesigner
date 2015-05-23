#include "base.h"

#include <QtCore>

Base::Base() : mName(nullptr) {
    mPrimitives = new QVector<Primitive*>();
}

Base::~Base()
{
    delete mName;
    delete mPrimitives;
}


Base::Base(const QString& name) : mName(new QString(name)) {
    mPrimitives = new QVector<Primitive*>();
}

const QString* Base::getName()
{
    return mName;
}

const QVector<Primitive*>*Base::primitives()
{
    return mPrimitives;
}

void Base::addPrimitive(Primitive* newPrimitive)
{
    mPrimitives->append(newPrimitive);
}
