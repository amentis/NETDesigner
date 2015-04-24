#ifndef SYSTEMNODES_H
#define SYSTEMNODES_H

#include <QPainter>
#include <QRect>

#include "node.h"

class StartNode : public Node
{
public:
    StartNode();
    ~StartNode();
    void setRect() Q_DECL_OVERRIDE;
    void paint(QPainter *painter) Q_DECL_OVERRIDE;
};

#endif // SYSTEMNODES_H
