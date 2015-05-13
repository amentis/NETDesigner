#ifndef PRIMITIVE_H
#define PRIMITIVE_H

#include <vector>
#include <string>

using namespace std;

class Base;

class Primitive
{
public:
    enum class ArgumentType {Int, Real, String, Bool};
    Primitive();
    ~Primitive();
private:
    vector<ArgumentType>* mArguments;
    Base* mBase;
    string* mName;
};

#endif // PRIMITIVE_H
