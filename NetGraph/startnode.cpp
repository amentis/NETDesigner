#include "startnode.h"

StartNode::StartNode()
{
    mType = NodeType::StartNode;
}

StartNode::~StartNode()
{

}

void StartNode::setRect()
{
    mRect = new QRect(position->x() - 40, position->y() - 40, 80, 80);
}

void StartNode::paint(QPainter *painter)
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
