#ifndef ARROW_H
#define ARROW_H

#include <vector>

using namespace std;

#include "node.h"
#include "../PrimitivesLoader/primitive.h"

namespace NetGraph {
  class Arrow
  {
  public:
    Arrow();
    ~Arrow();
  private:
    vector<PrimitivesLoader::Primitive> primitives;
    bool leadsToSubnet;
    Node* source;
    Node* destination;
  };

}

#endif // ARROW_H
