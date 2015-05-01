#ifndef NODE_H
#define NODE_H

#include <QPoint>
#include <QPainter>
#include <QPaintEvent>
#include <QRect>
#include <QBrush>
#include <QColor>
#include <QPen>
#include <QVector>

#include "arrow.h"

class Arrow;

class Node
{
public:
    enum class NodeType {OrdinaryNode, ProximityNode, CaseNode, StartNode, EndNode};
    Node();
    Node(NodeType type, QString* expression);
    Node(NodeType type, QString&& expression);
    ~Node();
    void setPosition(QPoint* pos);

    void paint(QPainter *painter);

    void setType(NodeType newType);
    void setExpression(QString* newExpression);
    QString *expression();

    NodeType type() const;
    QRect* rect();
    QRect* tightRect();

    void addArrowIn(Arrow* newArrow);
    void addArrowOut(Arrow* newArrow);

private:
    void setRect();
    void setTightRect();

    void paintStartNode(QPainter *painter);
    void paintEndNode(QPainter *painter);
    void paintOrdinaryNode(QPainter *painter);
    void paintCaseNode(QPainter *painter);
    void paintProximityNode(QPainter *painter);

    NodeType mType;
    QString* mExpression;
    QPoint* position;
    QVector<Arrow*> *arrowsIn;
    QVector<Arrow*> *arrowsOut;
    QRect* mRect; // for placing arrowButton
    QRect* mTightRect; // for placing arrows
};

#endif // NODE_H
