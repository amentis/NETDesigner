#include "node.h"


Node::Node()
{

}

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
    (void)painter;
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

}

