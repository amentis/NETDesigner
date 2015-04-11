#ifndef PROXIMITYNODE_H
#define PROXIMITYNODE_H

#include <string>
#include <vector>

using namespace std;

#include "node.h"

namespace NetGraph
{
class ProximityNode : public Node
{
public:
    ProximityNode();
    ~ProximityNode();
private:
    string nodeExpression;
    vector<string> arrowExpressions;
};
}

#endif // PROXIMITYNODE_H
