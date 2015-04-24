#include "ordinarynode.h"

OrdinaryNode::OrdinaryNode()
{
    mType = NodeType::OrdinaryNode;
}

OrdinaryNode::~OrdinaryNode()
{

}

void OrdinaryNode::setRect()
{
    mRect = new QRect(position->x() - 50, position->y() - 35, 100, 70);
}

void OrdinaryNode::paint(QPainter *painter)
{
    QBrush outline(Qt::black);

    QPen out(outline,1);

    QBrush fill(QColor(255, 255, 128));

    painter->setPen(out);
    painter->setBrush(fill);

    painter->drawEllipse(position->x() - 40, position->y() - 25, 80, 50);
}
