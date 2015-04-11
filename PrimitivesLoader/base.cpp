#include "base.h"

namespace PrimitivesLoader
{
Base::Base()
{

}

Base::Base(const string& name)
{
    _name = new string(name);
}

const string* Base::getName()
{
    return _name;
}

Base::~Base()
{

}



}
