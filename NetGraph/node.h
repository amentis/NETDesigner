#ifndef NODE_H
#define NODE_H

#include "arrow.h"

namespace NetGraph {

  class Node
  {
  public:
    enum NodeType {OrdynaryNode, ProximityNode, CaseNode};
    Node();
    ~Node();
  protected:
    NodeType type;
  private:
    Arrow* arrowIn;
    Arrow* arrowOut;
  };
}

#endif // NODE_H
