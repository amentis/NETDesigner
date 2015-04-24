#include "endnode.h"


EndNode::EndNode()
{
    mType = NodeType::EndNode;
}

EndNode::~EndNode()
{

}

void EndNode::setRect()
{
    mRect = new QRect(position->x() - 35, position->y() - 35, 70, 70);
}


void EndNode::paint(QPainter *painter)
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
