#ifndef NODE_H
#define NODE_H

#include "arrow.h"

namespace NetGraph
{

class Arrow;

class Node
{
public:
    enum NodeType {OrdinaryNode, ProximityNode, CaseNode, StartNode, EndNode};
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
