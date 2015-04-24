#ifndef ORDINARYNODE_H
#define ORDINARYNODE_H

#include "node.h"

class OrdinaryNode: public Node
{
public:
    OrdinaryNode();
    ~OrdinaryNode();

    void setRect() Q_DECL_OVERRIDE;
    void paint(QPainter *painter) Q_DECL_OVERRIDE;
};

#endif // ORDINARYNODE_H
