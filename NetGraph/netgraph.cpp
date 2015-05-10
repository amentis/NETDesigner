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

void NetGraph::saveToStream(QTextStream &stream)
{
    for (const auto& node: *nodes){
        stream << nodes->indexOf(node) << ":"
               << (int)node->type() << ":";
        if (node->expression()){
            stream << *node->expression();
        } else {
            stream << " ";
        }
        stream << ":"
               << node->getPosition()->x() << "," << node->getPosition()->y() << ";";
    }

    stream << "\n";

    for (const auto&arrow : *arrows){
        stream << nodes->indexOf(arrow->from()) << ":"
               << nodes->indexOf(arrow->to()) << ":";
        if (arrow->expression()){
            stream << *arrow->expression();
        } else {
            stream << " ";
        }
            stream << ";";
        //primitives
    }
}

bool NetGraph::loadFromStream(QTextStream &stream)
{
    QString nodesLine = stream.readLine();
    if (nodesLine.isNull())
        return false;
    QString arrowsLine = stream.readLine();
    if (arrowsLine.isNull())
        return false;
    for (const auto& string : nodesLine.split(";", QString::SkipEmptyParts)){
        QStringList element = string.split(":", QString::KeepEmptyParts);
        Node* node = new Node((Node::NodeType)element.at(1).toInt(),
                              new QString(element.at(2)));
        if (node->type() == Node::NodeType::StartNode)
            start = node;
        if (node->type() == Node::NodeType::EndNode)
            end = node;
        QStringList point = element.at(3).split(",", QString::SkipEmptyParts);
        node->setPosition(new QPoint(point.at(0).toInt(), point.at(1).toInt()));
        nodes->insert(element.at(0).toInt(), node);
    }
    for (const auto& string : arrowsLine.split(";", QString::SkipEmptyParts)){
        QStringList element = string.split(":", QString::KeepEmptyParts);
        arrows->append(new Arrow(nodes->at(element.at(0).toInt()),
                                 nodes->at(element.at(1).toInt()),
                                 new QString(element.at(1))));
    }
    return true;
}
