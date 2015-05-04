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
    emit contentModified();
}

void NetGraph::removeNode(Node *node)
{
    if (node->type() == Node::NodeType::StartNode)
        start = nullptr;
    if (node->type() == Node::NodeType::EndNode)
        end = nullptr;

    for (const auto& arrow : *node->arrowsIn()) {
        arrows->removeAll(arrow);
    }
    for (const auto& arrow : *node->arrowsOut()) {
        arrows->removeAll(arrow);
    }
    nodes->removeAll(node);
    emit contentModified();
}

const QVector<Node*>* NetGraph::getNodes()
{
    return nodes;
}

void NetGraph::addArrow(Node *from, Node *to, QString *expression)
{
    arrows->append(new Arrow(from, to, expression));
    emit contentModified();
}

void NetGraph::editArrow(QVector<Primitive *> added, QVector<Primitive *> removed)
{

}

void NetGraph::removeArrow(Arrow *arrow)
{
    arrow->from()->removeArrowOut(arrow);
    arrow->to()->removeArrowIn(arrow);
    arrows->removeAll(arrow);
}

const QVector<Arrow *> *NetGraph::getArrows()
{
    return arrows;
}

bool NetGraph::hasStartNode()
{
    return (start);
}

bool NetGraph::hasEndNode()
{
    return (end);
}
