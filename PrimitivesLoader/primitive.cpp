#include "primitive.h"

#include <QtCore>

#include "base.h"

Primitive::Primitive() : mArguments(nullptr), mBase(nullptr), mName(nullptr)
{
    (void) mBase;
}

Primitive::~Primitive()
{
    delete mArguments;
    delete mName;
}

Base*Primitive::base()
{
    return mBase;
}

QString* Primitive::name()
{
    return mName;
}
