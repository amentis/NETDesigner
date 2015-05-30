#include "graph.h"

#include <QtGui>

#include "arrow.h"

#include "primitive.h"
#include "primitivesloader.h"

Graph::Graph(QObject *parent) : QObject(parent),
    mNodes(new QVector<Node*>), mArrows(new QVector<Arrow*>), mStart(nullptr), mEnd(nullptr) {}

Graph::~Graph()
{
    delete mNodes;
    delete mArrows;
    delete mStart;
    delete mEnd;
}

void Graph::addNode(Node *newNode, QPoint *position)
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

void Graph::editNode(Node* target, Node::NodeType newType, QString *expression)
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

void Graph::removeNode(Node *node)
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

const QVector<Node*>* Graph::nodes()
{
    return mNodes;
}

void Graph::addArrow(Node *from, Node *to, QString *expression)
{
    mArrows->append(new Arrow(from, to, expression));
    emit contentModified();
}

void Graph::removeArrow(Arrow *arrow)
{
    arrow->from()->removeArrowOut(arrow);
    arrow->to()->removeArrowIn(arrow);
    mArrows->removeAll(arrow);
}

const QVector<Arrow *> *Graph::arrows()
{
    return mArrows;
}

bool Graph::hasStartNode()
{
    return (mStart);
}

Node*Graph::startNode()
{
    return mStart;
}

bool Graph::hasEndNode()
{
    return (mEnd);
}

Node*Graph::endNode()
{
    return mEnd;
}

void Graph::saveToStream(QTextStream &stream)
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

    for (Arrow* arrow : *mArrows){
        stream << mNodes->indexOf(arrow->from()) << ":"
               << mNodes->indexOf(arrow->to()) << ":";
        if (arrow->expression()){
            stream << *arrow->expression();
        } else {
            stream << " ";
        }
        stream << ":";
        QString* subnet;
        if (arrow->subnetCalled(subnet))
            stream << *subnet;
        else
            stream << "x";
        stream << ":";
        int loops;
        unsigned recursion;
        bool executeBack;
        arrow->options(loops, recursion, executeBack);
        stream << loops;
        stream << ":";
        stream << recursion;
        stream << ":";
        stream << (int)executeBack;
        stream << ":";
        bool first = true;
        for (const auto& primitive : *(arrow->primitives())){
            if (first)
                first = false;
            else
                stream << "&";
            stream << *(primitive->name());
            stream << "_";
            stream << *(arrow->argumentsForPrimitive(primitive->name()));
        }
            stream << ";";
    }
}

bool Graph::loadFromStream(QTextStream &stream, QVector<Base*>* bases)
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
        Arrow * arrow = new Arrow(mNodes->at(element.at(0).toInt()),
                                  mNodes->at(element.at(1).toInt()),
                                  new QString(element.at(2)),this);
        if (element.at(3) != "x")
            arrow->setCall(new QString(element.at(3)));
        arrow->setOptions(element.at(4).toInt(), element.at(5).toUInt(), (bool)(element.at(6).toInt()));
        if (element.at(7) != ""){
            QStringList primitives = element.at(7).split("&", QString::SkipEmptyParts);
            for (const auto& pr : primitives){
                QStringList def = pr.split("_", QString::SkipEmptyParts);
                Primitive* primitive = PrimitivesLoader::findPrimitiveByName(bases, new QString(def[0]));
                if (!primitive){
                    qCritical("Cannot find primitive in bases: %s", def[0].toLocal8Bit().constData());
                    return false;
                }
                arrow->addPrimitive(primitive);
                arrow->setArgumentsForPrimitive(new QString(def[0]), new QString(def[1]));
            }
        }
        mArrows->append(arrow);
    }
    return true;
}

unsigned Graph::internalNodeIndex(Node* node)
{
    return mNodes->indexOf(node);
}
