#ifndef SYSTEMNODES_H
#define SYSTEMNODES_H

#include "node.h"

namespace NetGraph
{
class StartNode : public Node
{
public:
    StartNode();
    ~StartNode();
};
class EndNode : public Node
{
public:
    EndNode();
    ~EndNode();
};
}

#endif // SYSTEMNODES_H
