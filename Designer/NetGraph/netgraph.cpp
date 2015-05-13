#include "netgraph.h"

#include <QtGui>

#include "arrow.h"

//#include "primitive.h"

NetGraph::NetGraph(QObject *parent) : QObject(parent),
    mNodes(new QVector<Node*>), mArrows(new QVector<Arrow*>), mStart(nullptr), mEnd(nullptr) {}

NetGraph::~NetGraph()
{
    delete mNodes;
    delete mArrows;
    delete mStart;
    delete mEnd;
}

void NetGraph::addNode(Node *newNode, QPoint *position)
{

    if (newNode->type() == Node::NodeType::StartNode){
        if (hasStartNode())
            return;
        mStart = newNode;
    }

    if (newNode->type() == Node::NodeType::EndNode){
        if (hasEndNode())
            return;
        mEnd = newNode;
    }

    newNode->setPosition(position);

    mNodes->append(newNode);
    emit contentModified();
}

void NetGraph::editNode(Node* target, Node::NodeType newType, QString *expression)
{
    if (newType == Node::NodeType::StartNode){
        if (hasStartNode())
            return;
        mStart = target;
    }

    if (newType == Node::NodeType::EndNode){
        if (hasEndNode())
            return;
        mEnd = target;
    }
    target->setType(newType);
    target->setExpression(expression);
    emit contentModified();
}

void NetGraph::removeNode(Node *node)
{
    if (node->type() == Node::NodeType::StartNode)
        mStart = nullptr;
    if (node->type() == Node::NodeType::EndNode)
        mEnd = nullptr;

    for (const auto& arrow : *node->arrowsIn()) {
        mArrows->removeAll(arrow);
    }
    for (const auto& arrow : *node->arrowsOut()) {
        mArrows->removeAll(arrow);
    }
    mNodes->removeAll(node);
    emit contentModified();
}

const QVector<Node*>* NetGraph::nodes()
{
    return mNodes;
}

void NetGraph::addArrow(Node *from, Node *to, QString *expression)
{
    mArrows->append(new Arrow(from, to, expression));
    emit contentModified();
}

//void NetGraph::editArrow(QVector<Primitive *> added, QVector<Primitive *> removed)
//{
//    //TODO: implement
//    (void)added;
//    (void)removed;
//}

void NetGraph::removeArrow(Arrow *arrow)
{
    arrow->from()->removeArrowOut(arrow);
    arrow->to()->removeArrowIn(arrow);
    mArrows->removeAll(arrow);
}

const QVector<Arrow *> *NetGraph::arrows()
{
    return mArrows;
}

bool NetGraph::hasStartNode()
{
    return (mStart);
}

bool NetGraph::hasEndNode()
{
    return (mEnd);
}

void NetGraph::saveToStream(QTextStream &stream)
{
    for (const auto& node: *mNodes){
        stream << mNodes->indexOf(node) << ":"
               << (int)node->type() << ":";
        if (node->expression()){
            stream << *node->expression();
        } else {
            stream << " ";
        }
        stream << ":"
               << node->position()->x() << "," << node->position()->y() << ";";
    }

    stream << "\n";

    for (const auto&arrow : *mArrows){
        stream << mNodes->indexOf(arrow->from()) << ":"
               << mNodes->indexOf(arrow->to()) << ":";
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
            mStart = node;
        if (node->type() == Node::NodeType::EndNode)
            mEnd = node;
        QStringList point = element.at(3).split(",", QString::SkipEmptyParts);
        node->setPosition(new QPoint(point.at(0).toInt(), point.at(1).toInt()));
        mNodes->insert(element.at(0).toInt(), node);
    }
    for (const auto& string : arrowsLine.split(";", QString::SkipEmptyParts)){
        QStringList element = string.split(":", QString::KeepEmptyParts);
        mArrows->append(new Arrow(mNodes->at(element.at(0).toInt()),
                                 mNodes->at(element.at(1).toInt()),
                                 new QString(element.at(1))));
    }
    return true;
}
