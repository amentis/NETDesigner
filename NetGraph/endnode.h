#ifndef ENDNODE_H
#define ENDNODE_H

#include "node.h"

class EndNode : public Node
{
public:

    EndNode();
    ~EndNode();

    void setRect() Q_DECL_OVERRIDE;
    void paint(QPainter *painter) Q_DECL_OVERRIDE;
};

#endif // ENDNODE_H
