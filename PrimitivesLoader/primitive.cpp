#include "primitive.h"

#include <QtCore>

#include "base.h"

Primitive::Primitive() : mArguments(nullptr), mBase(nullptr), mName(nullptr)
{
    (void) mBase;
}

Primitive::Primitive(Base* base, QString* name) : mArguments(new QVector<Argument*>()), mBase(base), mName(name)
{
    mBase->addPrimitive(this);
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

QString*Primitive::definition()
{
    QString* result = new QString(*name());
    result->append("(");
    bool first = true;
    for (const auto& argument : *mArguments){
        if (first)
            first = false;
        else
            result->append(", ");
        switch (argument->type){
        case Primitive::ArgumentType::Int: result->append("int "); break;
        case Primitive::ArgumentType::Real: result->append("real "); break;
        case Primitive::ArgumentType::String: result->append("string "); break;
        case Primitive::ArgumentType::Bool: result->append("bool "); break;
        }
        result->append(argument->name);
    }
    result->append(")");
    return result;
}

bool Primitive::hasArguments()
{
    return (!(mArguments->size() == 0));
}

void Primitive::addArgument(Primitive::ArgumentType type, QString* name)
{
    mArguments->append(new Argument{type, name});
}
