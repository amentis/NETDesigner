#include "primitive.h"

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
