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
    Node(NodeType type, QString* expression);
    Node(NodeType type, QString&& expression);
    ~Node();
    void setPosition(QPoint* pos);

    void paint(QPainter *painter);

    void setType(NodeType newType);
    void setExpression(QString* newExpression);

    NodeType type() const;
    QRect* rect();
private:
    void setRect();

    void paintStartNode(QPainter *painter);
    void paintEndNode(QPainter *painter);
    void paintOrdinaryNode(QPainter *painter);
    void paintCaseNode(QPainter *painter);
    void paintProximityNode(QPainter *painter);

    NodeType mType;
    QString* mExpression;
    QPoint* position;
    Arrow* arrowIn;
    Arrow* arrowOut;
    QRect* mRect;
};

#endif // NODE_H
