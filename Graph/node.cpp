#include "node.h"

#include <QtGui>

#include "arrow.h"

Node::Node(QObject *parent) : QObject(parent), mType(NodeType::OrdinaryNode) , mExpression(nullptr),
    mPosition(nullptr), mArrowsIn(new QVector<Arrow*>()), mArrowsOut(new QVector<Arrow*>()),
    mRect(nullptr), mTightRect(nullptr) {}

Node::Node(NodeType type, QString *expression, QObject *parent): QObject(parent),
    mType(type), mExpression(expression), mPosition(nullptr),
    mArrowsIn(new QVector<Arrow*>()), mArrowsOut(new QVector<Arrow*>()),
    mRect(nullptr), mTightRect(nullptr) {}

Node::Node(Node::NodeType type, QString &&expression, QObject *parent) :
    Node(type, new QString(expression), parent) {}

Node::~Node()
{
    delete mExpression;
    delete mPosition;
    delete arrowsIn();
    delete arrowsOut();
    delete mRect;
    delete mTightRect;
}

void Node::setPosition(QPoint *pos)
{
    mPosition = pos;
    setRect();
    setTightRect();
}

void Node::paint(QPainter *painter)
{
    switch (mType) {
    case NodeType::StartNode: paintStartNode(painter); break;
    case NodeType::EndNode: paintEndNode(painter); break;
    case NodeType::CaseNode: paintCaseNode(painter); break;
    case NodeType::OrdinaryNode: paintOrdinaryNode(painter); break;
    case NodeType::ProximityNode: paintProximityNode(painter); break;
    }
}

void Node::setType(Node::NodeType newType)
{
    mType = newType;
}

void Node::setExpression(QString *newExpression)
{
    delete mExpression;
    mExpression = newExpression;
}

QString *Node::expression()
{
    return mExpression;
}

Node::NodeType Node::type() const
{
    return mType;
}

QRect *Node::rect()
{
    return mRect;
}

QRect *Node::tightRect()
{
    return mTightRect;
}

void Node::removeArrowIn(Arrow *arrow) const
{
    mArrowsIn->removeAll(arrow);
}

void Node::removeArrowOut(Arrow *arrow) const
{
    mArrowsOut->removeAll(arrow);
}


void Node::addArrowIn(Arrow *newArrow)
{
    mArrowsIn->append(newArrow);
}

void Node::addArrowOut(Arrow *newArrow)
{
    mArrowsOut->append(newArrow);
}

const QVector<Arrow *> *Node::arrowsIn()
{
    return mArrowsIn;
}

const QVector<Arrow *> *Node::arrowsOut()
{
    return mArrowsOut;
}

void Node::setRect()
{
    switch (mType) {
    case NodeType::StartNode:
        mRect = new QRect(mPosition->x() - 40, mPosition->y() - 40, 80, 80);
        break;
    case NodeType::EndNode:
        mRect = new QRect(mPosition->x() - 35, mPosition->y() - 35, 70, 70);
        break;
    case NodeType::CaseNode:
        mRect = new QRect(mPosition->x() - 60, mPosition->y() - 25, 120, 50);
        break;
    case NodeType::OrdinaryNode:
        mRect = new QRect(mPosition->x() - 50, mPosition->y() - 35, 100, 70);
        break;
    case NodeType::ProximityNode:
        mRect = new QRect(mPosition->x() - 45, mPosition->y() - 35, 90, 70);
        break;
    }
}

void Node::setTightRect()
{
    switch(mType){
    case NodeType::StartNode:
        mTightRect = new QRect(mPosition->x() - 20, mPosition->y() - 20, 40, 40);
        break;
    case NodeType::EndNode:
        mTightRect = new QRect(mPosition->x() - 20, mPosition->y() - 20, 40, 40);
        break;
    case NodeType::CaseNode:
        mTightRect = new QRect(mPosition->x() - 40, mPosition->y() - 25, 80, 50);
        break;
    case NodeType::OrdinaryNode:
        mTightRect = new QRect(mPosition->x() - 40, mPosition->y() - 25, 80, 50);
        break;
    case NodeType::ProximityNode:
        mTightRect = new QRect(mPosition->x() - 45, mPosition->y() - 35, 90, 70);
        break;
    }
}

void Node::paintStartNode(QPainter *painter)
{
    QBrush outline(Qt::black);
    QPen outer(outline, 2);

    QBrush fill(QColor(94, 94, 255));

    painter->setBrush(fill);

    painter->setPen(outer);
    painter->drawRect(mPosition->x() - 20, mPosition->y() - 20, 40, 40);

    QPen inner(outline, 1);
    painter->setPen(inner);
    painter->drawRect(mPosition->x() - 15, mPosition->y() - 15, 30, 30);
}

void Node::paintEndNode(QPainter *painter)
{
    QBrush outline(Qt::black);
    QPen pen(outline, 1);

    QBrush fillOuter(QColor(94, 94, 255));

    QBrush fillInner(QColor(224, 235, 255));

    QPen penInner(fillInner, 1);

    painter->setPen(pen);
    painter->setBrush(fillOuter);

    painter->drawEllipse(mPosition->x() - 20, mPosition->y() - 20, 40, 40);

    painter->setPen(penInner);
    painter->setBrush(fillInner);

    painter->drawEllipse(mPosition->x() - 10, mPosition->y() - 10, 20, 20);
}

void Node::paintOrdinaryNode(QPainter *painter)
{
    QBrush outline(Qt::black);

    QPen out(outline,1);

    QBrush fill(QColor(255, 255, 128));

    painter->setPen(out);
    painter->setBrush(fill);

    painter->drawEllipse(mPosition->x() - 40, mPosition->y() - 25, 80, 50);
}

void Node::paintCaseNode(QPainter *painter)
{
    QBrush outline(Qt::black);
    QPen out(outline, 1);

    QBrush fill(QColor(173, 214, 255));

    painter->setPen(out);
    painter->setBrush(fill);

    QPoint points[] = {QPoint(mPosition->x() - 60, mPosition->y() - 25),
                       QPoint(mPosition->x() + 20, mPosition->y() - 25),
                       QPoint(mPosition->x() + 60, mPosition->y() + 25),
                       QPoint(mPosition->x() - 20, mPosition->y() + 25)};

    painter->drawPolygon(points, 4);

    painter->drawText(QRect(mPosition->x() - 50, mPosition->y() - 20, 100, 40), Qt::AlignCenter, *mExpression);
}

void Node::paintProximityNode(QPainter *painter)
{
    QBrush outline(Qt::black);
    QPen out(outline, 1);

    QBrush fill(QColor(112,255,112));

    painter->setPen(out);
    painter->setBrush(fill);

    QPoint points[] = {QPoint(mPosition->x(), mPosition->y() - 35),
                       QPoint(mPosition->x() + 45, mPosition->y()),
                       QPoint(mPosition->x(), mPosition->y() + 35),
                       QPoint(mPosition->x() - 45, mPosition->y())};

    painter->drawPolygon(points, 4);

    painter->drawText(QRect(mPosition->x() - 50, mPosition->y() - 20, 100, 40), Qt::AlignCenter, *mExpression);
}
QPoint *Node::position()
{
    return mPosition;
}

