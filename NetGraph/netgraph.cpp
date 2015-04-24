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

    if (newNode->type() == Node::NodeType::StartNode){
        if (hasStartNode())
            return;
        start = newNode;
    }

    if (newNode->type() == Node::NodeType::EndNode){
        if (hasEndNode())
            return;
        end = newNode;
    }

    newNode->setPosition(position);

    nodes->append(newNode);
    emit contentModified();
}

void NetGraph::editNode(Node* target, Node::NodeType newType, QString *expression)
{
    if (newType == Node::NodeType::StartNode){
        if (hasStartNode())
            return;
        start = target;
    }

    if (newType == Node::NodeType::EndNode){
        if (hasEndNode())
            return;
        end = target;
    }
    target->setType(newType);
    target->setExpression(expression);
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
