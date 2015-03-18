#include "base.h"

namespace PrimitivesLoader
{
  Base::Base()
  {

  }

  Base::Base(string name)
  {
    _name = name;
  }

  string Base::getName()
  {
    return _name;
  }

  Base::~Base()
  {

  }


}
