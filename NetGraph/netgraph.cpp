#include "netgraph.h"

NetGraph::NetGraph(QObject *parent) : QObject(parent)
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

void NetGraph::addNode(Node *newNode, QPoint *position)
{

    switch (newNode->type()) {
    case Node::NodeType::StartNode:
        if (hasStartNode())
            return;
        start = (dynamic_cast<StartNode*>(newNode));
        start->setPosition(position);
        break;
    case Node::NodeType::EndNode:
        if (hasEndNode())
            return;
        end = (dynamic_cast<EndNode*>(newNode));
        end->setPosition(position);
        break;
    case Node::NodeType::OrdinaryNode:
        (dynamic_cast<OrdinaryNode*>(newNode))->setPosition(position);
        break;
    case Node::NodeType::CaseNode:
        (dynamic_cast<CaseNode*>(newNode))->setPosition(position);
        break;
    case Node::NodeType::ProximityNode:
        (dynamic_cast<ProximityNode*>(newNode))->setPosition(position);
        break;
    }

    nodes->append(newNode);
    emit contentModified();
}

void NetGraph::removeNode(Node *node)
{
    if (node->type() == Node::NodeType::StartNode)
        start = nullptr;
    if (node->type() == Node::NodeType::EndNode)
        end = nullptr;
    if (nodes->removeAll(node))
        emit contentModified();
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
