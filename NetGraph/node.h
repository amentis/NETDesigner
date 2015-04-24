#ifndef NODE_H
#define NODE_H

#include <QPoint>
#include <QPainter>
#include <QPaintEvent>
#include <QRect>
#include <QBrush>
#include <QColor>
#include <QPen>

#include "arrow.h"

class Arrow;

class Node
{
public:
    enum class NodeType {OrdinaryNode, ProximityNode, CaseNode, StartNode, EndNode};
    Node();
    ~Node();
    void setPosition(QPoint* pos);

    virtual void paint(QPainter *painter);

    static Node* createNode(NodeType nodeType, QString* expression, bool proximityIsFloat);

    NodeType type() const;
    QRect* rect();
protected:
    virtual void setRect();

    NodeType mType;
    QPoint* position;
    Arrow* arrowIn;
    Arrow* arrowOut;
    QRect* mRect;
};

#endif // NODE_H
