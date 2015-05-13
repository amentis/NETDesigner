#include "base.h"

Base::Base() : mName(nullptr)
{

}

Base::~Base()
{
    delete mName;
}


Base::Base(const string& name)
{
    mName = new string(name);
}

const string* Base::getName()
{
    return mName;
}
