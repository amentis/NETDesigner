#ifndef NODE_H
#define NODE_H

#include <QPoint>

#include "arrow.h"

namespace NetGraph
{

class Arrow;

class Node
{
public:
    enum class NodeType {OrdinaryNode, ProximityNode, CaseNode, StartNode, EndNode};
    Node();
    ~Node();
    void setPosition(QPoint* pos);

    static Node* createNode(NodeType nodeType, string* expression, bool proximityIsFloat);
protected:
    NodeType type;
private:
    QPoint* position;
    Arrow* arrowIn;
    Arrow* arrowOut;
};

}

#endif // NODE_H
