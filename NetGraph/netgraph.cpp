#include "netgraph.h"

NetGraph::NetGraph()
{

    nodes = new QVector<Node*>;
    arrows = new QVector<Arrow*>;

    start = nullptr;
    end = nullptr;
}

NetGraph::~NetGraph()
{
    delete nodes;
    delete arrows;
    delete start;
    delete end;
}

void NetGraph::addNode(Node *newNode)
{
    if (newNode->type() == Node::NodeType::StartNode){
        if (hasStartNode())
            return;
        start = dynamic_cast<StartNode*>(newNode);
    } else if (newNode->type() == Node::NodeType::EndNode){
        if (hasEndNode())
            return;
        end = dynamic_cast<EndNode*>(newNode);
    }
    nodes->append(newNode);
}

const QVector<Node*>* NetGraph::getNodes()
{
    return nodes;
}

bool NetGraph::hasStartNode()
{
    return (start);
}

bool NetGraph::hasEndNode()
{
    return (end);
}
