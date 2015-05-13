#ifndef BASE_H
#define BASE_H

#include <string>

using namespace std;

class Base
{
public:
    Base();
    Base(const string& name);
    ~Base();
    const string *getName();

private:
    string* mName;
};

#endif // BASE_H
