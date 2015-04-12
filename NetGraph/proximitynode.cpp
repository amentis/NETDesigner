#include "proximitynode.h"

namespace NetGraph
{

ProximityNode::ProximityNode(string *expression)
{
    nodeExpression = expression;
}

ProximityNode::ProximityNode(string &expression) : ProximityNode(new string(expression)) {}
ProximityNode::ProximityNode(string &&expression) : ProximityNode(new string(expression)) {}
ProximityNode::~ProximityNode()
{

}


}
