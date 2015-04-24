#include "node.h"


Node::Node(NodeType type, QString *expression):
    mType(type), mExpression(expression)
{
    switch (mType) {
    case NodeType::StartNode: break;
    case NodeType::EndNode: break;
    case NodeType::CaseNode: break;
    case NodeType::OrdinaryNode: break;
    case NodeType::ProximityNode: break;
    }
}

Node::Node(Node::NodeType type, QString &&expression) :
    Node(type, new QString(expression)) {}

Node::~Node()
{
    delete mRect;
    delete position;
}

void Node::setPosition(QPoint *pos)
{
    position = pos;
    setRect();
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

Node::NodeType Node::type() const
{
    return mType;
}

QRect *Node::rect()
{
    return mRect;
}

void Node::setRect()
{
    switch (mType) {
    case NodeType::StartNode:
        mRect = new QRect(position->x() - 40, position->y() - 40, 80, 80);
        break;
    case NodeType::EndNode:
        mRect = new QRect(position->x() - 35, position->y() - 35, 70, 70);
        break;
    case NodeType::CaseNode:
        mRect = new QRect(position->x() - 60, position->y() - 25, 120, 50);
        break;
    case NodeType::OrdinaryNode:
        mRect = new QRect(position->x() - 50, position->y() - 35, 100, 70);
        break;
    case NodeType::ProximityNode:
        mRect = new QRect(position->x() - 45, position->y() - 35, 90, 70);
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
    painter->drawRect(position->x() - 20, position->y() - 20, 40, 40);

    QPen inner(outline, 1);
    painter->setPen(inner);
    painter->drawRect(position->x() - 15, position->y() - 15, 30, 30);
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

    painter->drawEllipse(position->x() - 20, position->y() - 20, 40, 40);

    painter->setPen(penInner);
    painter->setBrush(fillInner);

    painter->drawEllipse(position->x() - 10, position->y() - 10, 20, 20);
}

void Node::paintOrdinaryNode(QPainter *painter)
{
    QBrush outline(Qt::black);

    QPen out(outline,1);

    QBrush fill(QColor(255, 255, 128));

    painter->setPen(out);
    painter->setBrush(fill);

    painter->drawEllipse(position->x() - 40, position->y() - 25, 80, 50);
}

void Node::paintCaseNode(QPainter *painter)
{
    QBrush outline(Qt::black);
    QPen out(outline, 1);

    QBrush fill(QColor(173, 214, 255));

    painter->setPen(out);
    painter->setBrush(fill);

    QPoint points[] = {QPoint(position->x() - 60, position->y() - 25),
                       QPoint(position->x() + 20, position->y() - 25),
                       QPoint(position->x() + 60, position->y() + 25),
                       QPoint(position->x() - 20, position->y() + 25)};

    painter->drawPolygon(points, 4);

    painter->drawText(QRect(position->x() - 50, position->y() - 20, 100, 40), Qt::AlignCenter, *mExpression);
}

void Node::paintProximityNode(QPainter *painter)
{
    QBrush outline(Qt::black);
    QPen out(outline, 1);

    QBrush fill(QColor(112,255,112));

    painter->setPen(out);
    painter->setBrush(fill);

    QPoint points[] = {QPoint(position->x(), position->y() - 35),
                       QPoint(position->x() + 45, position->y()),
                       QPoint(position->x(), position->y() + 35),
                       QPoint(position->x() - 45, position->y())};

    painter->drawPolygon(points, 4);

    painter->drawText(QRect(position->x() - 50, position->y() - 20, 100, 40), Qt::AlignCenter, *mExpression);
}
