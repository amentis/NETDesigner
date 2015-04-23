#ifndef ENDNODE_H
#define ENDNODE_H

#include "node.h"

class EndNode : public Node
{
public:

    void paint(QPainter *painter) Q_DECL_OVERRIDE;

    EndNode();
    ~EndNode();
};

#endif // ENDNODE_H
