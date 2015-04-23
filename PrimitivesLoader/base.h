#ifndef BASE_H
#define BASE_H

#include <string>

using namespace std;

class Base
{
public:
    Base();
    Base(const string& name);
    const string *getName();
    ~Base();
private:
    string* _name;
};

#endif // BASE_H
