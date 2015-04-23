#ifndef SYSTEMNODES_H
#define SYSTEMNODES_H

#include "node.h"

class StartNode : public Node
{
public:
    StartNode();
    ~StartNode();
    void paint(QPainter *painter) Q_DECL_OVERRIDE;
};

#endif // SYSTEMNODES_H
