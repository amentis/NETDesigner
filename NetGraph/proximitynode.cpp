#include "proximitynode.h"

ProximityNode::ProximityNode(QString *expression)
{
    mType = NodeType::ProximityNode;
    nodeExpression = expression;
}

ProximityNode::ProximityNode(QString &expression) : ProximityNode(new QString(expression)) {}
ProximityNode::ProximityNode(QString &&expression) : ProximityNode(new QString(expression)) {}
ProximityNode::~ProximityNode()
{

}

void ProximityNode::paint(QPainter *painter)
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

    painter->drawText(QRect(position->x() - 50, position->y() - 20, 100, 40), Qt::AlignCenter, *nodeExpression);
}
