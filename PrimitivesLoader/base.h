#ifndef BASE_H
#define BASE_H

#include <string>

using namespace std;

namespace PrimitivesLoader
{
  class Base
  {
  public:
    Base();
    ~Base();
  private:
    string name;
    string path;
  };
}

#endif // BASE_H
