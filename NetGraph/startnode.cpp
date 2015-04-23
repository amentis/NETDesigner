#include "startnode.h"

StartNode::StartNode()
{
    mType = NodeType::StartNode;
}

StartNode::~StartNode()
{

}

void StartNode::paint(QPainter *painter)
{
    QBrush outline(Qt::black);
    QPen outer(outline, 2);

    QBrush fill(QColor(94, 94, 255));

    painter->setBrush(fill);

    painter->setPen(outer);
    painter->drawRect(position->x() - 25, position->y() - 25, 50, 50);

    QPen inner(outline, 1);
    painter->setPen(inner);
    painter->drawRect(position->x() - 20, position->y() - 20, 40, 40);
}
