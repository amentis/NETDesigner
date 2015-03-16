#ifndef NETGRAPH_H
#define NETGRAPH_H

#include "startnode.h"

namespace NetGraph {
  class NetGraph
  {
  private:
    StartNode* start;
  public:
    NetGraph();
    ~NetGraph();
  };
}

#endif // NETGRAPH_H
