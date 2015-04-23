#include "node.h"


Node::Node()
{

}

Node::~Node()
{

}

void Node::setPosition(QPoint *pos)
{
    position = pos;
}

void Node::paint(QPainter *painter)
{
    (void)painter;
}

Node::NodeType Node::type() const
{
    return mType;
}

