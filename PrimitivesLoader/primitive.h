#ifndef PRIMITIVE_H
#define PRIMITIVE_H

#include <vector>
#include <string>

#include "base.h"

using namespace std;

namespace PrimitivesLoader
{
class Primitive
{
public:
    enum class ArgumentType {Int, Real, String, Bool};
    Primitive();
    ~Primitive();
private:
    vector<ArgumentType> arguments;
    Base base;
    string name;
};
}

#endif // PRIMITIVE_H
